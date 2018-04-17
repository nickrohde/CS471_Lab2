#pragma once

#ifndef _TEST_H
#define _TEST_H
#include <initializer_list>
#include "functions.hpp"


#define SHEKEL_OUTER_SIZE 30
#define SHEKEL_INNER_SIZE 10
#define NUMBER_FUNCTIONS  15


class results
{
public:

	results(void)
	{
		d_range = d_stdDev = d_avgTime = d_avgValue = d_bestValue = d_median = 0.0;
		bestValues = nullptr;
		data = nullptr;
	} // end Constructor

	friend std::ostream& operator<<(std::ostream& stream, results& res);


	double d_range, d_stdDev, d_avgTime, d_avgValue, d_bestValue, d_median;
	std::vector<double>* bestValues;
	std::vector<double>* data;
};



class Test
{
public:
	Test(void);
	~Test(void);

	template <typename F, typename... Args>
	void runTest(F f, const std::size_t ui_ITERATIONS, Args... args)
	{
		for (size_t ui_length = 10; ui_length <= 30; ui_length += 10)
		{
			vector<results> res; // for statistics

			for (int i = 0; i < costFunctions.size(); i++)
			{
				if (i == i_SHEKEL_INDEX && ui_length > 10) // only execute shekel with 10 dimensions
				{
					continue;
				} // end if

				res.push_back(results());

				// pointer to current results
				results * temp = &res.at(i);

				// run each function n times
				for (size_t j = 0; j < ui_ITERATIONS; j++)
				{
					compute_start = highRes_Clock::now();

					results* temp2 = f(costFunctions[i], ui_length, ui_ITERATIONS, da_ranges[i][0], da_ranges[i][1] , args...);
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
				temp->d_avgValue /= ui_ITERATIONS;
				temp->d_range /= ui_ITERATIONS;
				temp->d_stdDev /= ui_ITERATIONS;
				temp->d_avgTime /= ui_ITERATIONS;
			} // end for i

			storeResults(fileNames.at((ui_length / 10) - 1), res);
		} // end for length
	} // end template runTest



private:
	const std::size_t ui_SHEKEL_ITERATIONS = 10;

	double ** da_ranges,
		   ** da_A;

	std::vector<costFunction> costFunctions;

	std::vector<std::string> fileNames;

	void storeResults(std::string&, std::vector<results>&);
	void makeMatrix(double**&);
	void makeRanges(double**&);

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;
	
};

#endif // !_TEST_H
