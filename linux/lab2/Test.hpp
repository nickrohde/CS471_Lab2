#pragma once

#ifndef _TEST_H
#define _TEST_H
#include <initializer_list>
#include "functions.hpp"
#include "results.hpp"


#define SHEKEL_OUTER_SIZE 30
#define SHEKEL_INNER_SIZE 10
#define NUMBER_FUNCTIONS  15


class Test
{
public:
	Test(void);
	Test(std::vector<double>* lsd, std::size_t ui_dimMin, std::size_t ui_dimMax, std::size_t ui_dimDelta, bool b_storeData);
	~Test(void);

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
						temp->bestValues = std::move(temp2->bestValues);
					} // end if

					time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

					temp2->d_avgTime = time_to_compute.count();

					if (b_storeData)
					{
						dumpDataToFile(makeFileName(ui_length, i), temp2);
					} // end if

					delete temp2;
				} // end for j
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
	const std::size_t ui_SHEKEL_M = 10;

	// Member Variables:
	double ** da_ranges,
		   ** da_A;

	std::size_t	ui_minDimensions,
				ui_maxDimensions,
				ui_dimensionDelta;

	bool b_storeData;

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;

	std::vector<costFunction> costFunctions;
	std::vector<double> LS_delta;


	// Private Functions:
	void makeMatrix(double**&);
	void makeRanges(double**&);	
	void dumpDataToFile(std::string, results_t* res);

	std::string makeFileName(std::size_t, int);
};

#endif // !_TEST_H
