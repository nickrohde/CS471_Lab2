#pragma once

#ifndef _RAND_WALK_H
#define _RAND_WALK_H
#include "Test.hpp"
#include <limits>

typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function < results*(costFunction, const std::size_t, const std::size_t, double, double)> randWlk;

results* randomWalk(costFunction f, const std::size_t ui_SIZE, const std::size_t ui_ITERATIONS, double d_min, double d_max)
{
	double	d_best = std::numeric_limits<double>::max(),
		d_result = 0,
		d_avgValue = 0;

	std::vector<double>* bestArgs = nullptr;
	std::vector<double>* data = new std::vector<double>();

	for (std::size_t i = 0; i < ui_ITERATIONS; i++)
	{
		std::vector<double>* args = getRandomVector(ui_SIZE, &d_min, &d_max);

		d_result = f(args);

		d_avgValue += d_result;

		if (d_result < d_best)
		{
			if (bestArgs != nullptr)
			{
				delete bestArgs;
			} // end if

			bestArgs = args;

			d_best = d_result;

			// shrink search space whenever we find a better solution
			d_min = minValueInVector(args);
			d_max = maxValueInVector(args);
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for

	d_avgValue /= ui_ITERATIONS;

	// statistical results
	results* res = new results();
	res->bestValues = std::move(bestArgs);
	res->d_avgValue = d_avgValue;
	res->d_bestValue = d_best;
	res->data = std::move(data);

	return res;
} // end method randomWalk

#endif // !_RAND_WALK_H