#pragma once

#ifndef _TEST_H
#define _TEST_H
#include <initializer_list> // variadic template
#include "functions.hpp"    // cost functions
#include "results.hpp"      // results_t


#define SHEKEL_OUTER_SIZE 30 // size of outer array used by shekel's foxholes
#define SHEKEL_INNER_SIZE 10 // size of inner array used by shekel's foxholes
#define NUMBER_FUNCTIONS  15 // number of cost function

/// <summary>Class that handles testing of the optimization strategies.</summary>
class Test
{
public:
	/// <summary>Default constructor, sets all variables to a default.</summary>
	Test(void);

	/// <summary>Constructor taking 5 arguements.</summary>
	/// <param name="lsd">Pointer to a vector containing the delta values for local search.</param>
	/// <param name="ui_dimMin">Starting dimension for tests (inclusive; must be positive integer and strictly smaller than <paramref name="ui_dimMax"/>).</param>
	/// <param name="ui_dimMax">Ending dimension for tests (inclusive; must be positive integer and strictly greater than <paramref name="ui_dimMin"/>).</param>
	/// <param name="ui_dimDelta">Dimension increase step size (must be positive integer).</param>
	/// <param name="b_storeData">Whether or not to generate data files (Note: these files tend to be very large).</param>
	Test(std::vector<double>* lsd, std::size_t ui_dimMin, std::size_t ui_dimMax, std::size_t ui_dimDelta, bool b_storeData);

	/// <summary>Destructor. Frees all dynamic memory beloging to this object.</summary>
	~Test(void);

	/// <summary>This function runs the test of the given strategy. The test is run <paramref name="ui_iterations"/> times for each cost function, and each dimension.</summary>
	/// <typeparam name="F">A callabable type taking <typeparamref name="Args"/> as parameter list.</typeparam>
	/// <typeparam name="Args">The parameter list for callabable type <typeparamref name="F"/>.</typeparam>
	/// <param name="f">The optimization strategy for the test (e.g. randomwalk, LS, ILS, etc.)</param>
	/// <param name="ui_iterations">The number of iterations to run per cost function.</param>
	/// <param name="args">Argument list passed on to <paramref name="f"/></param>
	/// <returns>All output is placed into the file(s) named "results_[dimension]_f[function number].csv". Output is only generated if the variable b_storeData is set to true.</returns>
	/// <remarks>This function is defined here because it is a template.</remarks>
	template <typename F, typename... Args>
	void runTest(F f, std::size_t ui_iterations, Args... args)
	{
		for (size_t ui_length = ui_minDimensions; ui_length <= ui_maxDimensions; ui_length += ui_dimensionDelta)
		{
			vector<results_t> res; // for statistics

			for (int i = 0; i < costFunctions.size(); i++)
			{
				if (i == i_SHEKEL_INDEX && ui_length > 10) // only execute shekel with 10 dimensions
				{
					continue;
				} // end if

				res.push_back(results_t());

				// pointer to current results
				results_t * temp = &res.at(i);

				// run each function n times
				for (size_t j = 0; j < ui_iterations; j++)
				{
					compute_start = highRes_Clock::now();

					results_t* temp2 = f(costFunctions[i], ui_length, da_ranges[i][0], da_ranges[i][1], LS_delta.at(i), args...);
					compute_end = highRes_Clock::now();

					if (temp->d_bestValue > temp2->d_bestValue)
					{
						temp->d_bestValue = temp2->d_bestValue;
						temp->bestValues = temp2->bestValues;
					} // end if

					time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

					temp->d_avgTime += time_to_compute.count();
					temp->d_finalDeltaX += temp2->d_finalDeltaX;
					temp->ui_functionCalls += temp2->ui_functionCalls;

					if (b_storeData)
					{
						dumpDataToFile(makeFileName(ui_length, i), temp2); // dump the data vector to file if user requested it
					} // end if

					delete temp2;
				} // end for j

				temp->d_avgTime /= static_cast<double>(ui_iterations); // calculate avg time
				temp->d_finalDeltaX /= static_cast<double>(ui_iterations); // calculate avg deltaX found
				temp->ui_functionCalls /= static_cast<double>(ui_iterations); // calculate avg function calls

				writeResultsToFile(temp); // write best point and average time to file

			} // end for i

			if (!b_storeData) // ouput results to stdout if user choses not to use file
			{
				int i = 1;
				for (auto& r : res)
				{
					cout << "F" << i << " results: " << endl;
					cout << r;
				} // end for
			} // end if
		} // end for length
	} // end template runTest



private:
	// Member Constants:
	/// <summary>Number of iterations that the shekel's foxhole inner loop will execute.</summary>
	const std::size_t ui_SHEKEL_M = 10;

	// Member Variables:
	/// <summary>Matrix containing the ranges of all 15 cost functions.</summary>
	double ** da_ranges,
	/// <summary>Shekel's foxholes arguements.</summary>
		   ** da_A;

	/// <summary>Dimension info.</summary>
	std::size_t	ui_minDimensions,
				ui_maxDimensions,
				ui_dimensionDelta;

	/// <summary>Whether or not to generate data file.</summary>
	bool b_storeData;

	/// <summary>Used for timing tests.</summary>
	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;

	/// <summary>Vector containing pointers to the cost functions.</summary>
	std::vector<costFunction> costFunctions;

	/// <summary>Vector containing the delta x values for local search.</summary>
	std::vector<double> LS_delta;


	// Private Functions:
	/// <summary>Generates the matrix for shekel's foxholes.</summary>
	/// <param name="da_A">A valid pointer to an array of 30 uninitialized double pointers.</param>
	/// <remarks>This function is only there to remove some code from the constructor, it is inline and will simply be placed in the constructor by the compiler.</remarks>
	void makeMatrix(double**& da_A);

	/// <summary>Generates the matrix containing the ranges for all cost functions.</summary>
	/// <param name="da_A">A valid pointer to an array of 15 uninitialized double pointers.</param>
	/// <remarks>This function is only there to remove some code from the constructor, it is inline and will simply be placed in the constructor by the compiler.</remarks>
	void makeRanges(double**& da_ranges);

	/// <summary>Generates data file with test data.</summary>
	/// <param name="s_name">Name of the file to dump the data into.</param>
	/// <param name="res">A pointer to a results_t object containing the data to dump.</param>
	/// <remarks>This function is only called if b_storeData is set to true. The files created tend to be very large.</remarks>
	void dumpDataToFile(std::string s_name, results_t* res);

	/// <summary>Generates results file with test results.</summary>
	/// <param name="res">A pointer to a results_t object containing the test results.</param>
	/// <remarks>This function only writes the optimal point and solution to the file, all other data is ignored.</remarks>
	void writeResultsToFile(results_t* res);

	/// <summary>Generates a file name. Name will be generated as "data_[<paramref name="ui_dim"/>]_f[<paramref name="i_functionNumber"/>]".</summary>
	/// <param name="ui_dim">Dimension to insert into the name.</param>
	/// <param name="i_functionNumber">Function number to insert into the name.</param>
	std::string makeFileName(std::size_t ui_dim, int i_functionNumber);
}; // end Class Test

#endif // !_TEST_H
