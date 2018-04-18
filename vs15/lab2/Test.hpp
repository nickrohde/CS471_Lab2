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

					results_t* temp2 = f(costFunctions[i], ui_length, da_ranges[i][0], da_ranges[i][1] , args...);
					compute_end = highRes_Clock::now();

					if (temp->d_bestValue > temp2->d_bestValue)
					{
						temp->d_bestValue = temp2->d_bestValue;
						temp->bestValues = std::move(temp2->bestValues);
					} // end if

					time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

					// statistics
					temp->d_range += getRange(temp2->data);
					temp->d_stdDev += getStandardDeviation(temp2->data, temp2->d_avgValue);
					temp->d_median += getMedian(temp2->data);
					temp->d_avgValue += temp2->d_bestValue;
					temp->d_avgTime += time_to_compute.count();
				} // end for j

				// calculate averages
				temp->d_avgValue /= ui_iterations;
				temp->d_range /= ui_iterations;
				temp->d_stdDev /= ui_iterations;
				temp->d_avgTime /= ui_iterations;
			} // end for i

			storeResults(fileNames.at((ui_length / 10) - 1), res);
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

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;

	std::vector<costFunction> costFunctions;

	std::vector<std::string> fileNames;

	// Private Functions:
	void storeResults(std::string&, std::vector<results_t>&);
	void makeMatrix(double**&);
	void makeRanges(double**&);	
};

#endif // !_TEST_H
