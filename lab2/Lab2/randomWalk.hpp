#pragma once

#include "utility.hpp"
#include "Test.hpp"
#include <chrono>





results& randomWalk(double (*f) (const std::vector<double>*), const std::size_t ui_SIZE, const uint ui_ITERATIONS, double* d_min, double* d_max)
{
	double	d_best	 = 0x7FFFFFFFFFFFFFFF,
			d_result = 0,
			d_avgValue = 0,
			d_avgTime = 0;

	std::vector<double>* bestArgs = NULL;
	std::vector<double>* data = new std::vector<double>();

	std::chrono::high_resolution_clock::time_point	compute_start = std::chrono::high_resolution_clock::now(),
													compute_end   = std::chrono::high_resolution_clock::now();

	for (std::size_t i = 0; i < ui_ITERATIONS; i++)
	{
		std::vector<double>* args = getRandomVector(ui_SIZE, d_min, d_max);

		compute_start = std::chrono::high_resolution_clock::now();

		d_result = f(args);

		compute_end = std::chrono::high_resolution_clock::now();

		d_avgValue += d_result;

		if (d_result < d_best)
		{
			if (bestArgs != NULL)
			{
				delete bestArgs;
			} // end if

			bestArgs = args;

			d_best = d_result;

			// shrink search space whenever we find a better solution
			*d_min = minValueInVector(args);
			*d_max = maxValueInVector(args);
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for

	d_avgValue /= ui_ITERATIONS;

	// statistical results
	results res;
	res.bestValues = std::move(bestArgs);
	res.d_avgValue = d_avgValue;
	res.d_bestValue = d_best;
	res.d_avgTime = d_avgTime;
	res.d_range = getRange(data);
	res.d_stdDev = getStandardDeviation(data, d_avgValue);

	return res;
} // end method randomWalk