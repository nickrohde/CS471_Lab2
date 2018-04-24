#include "randomWalk.hpp"

using namespace std;

results_t* randomWalk(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double unused, bool b_storeData, std::size_t ui_ITERATIONS)
{
	results_t* res = new results_t();

	if (b_storeData)
	{
		res->data = new std::vector<double>();
	} // end if

	// pointers for convenience
	double*	d_best = &(res->d_bestValue),
			d_result = 0;

	vector<double>** bestArgs = &(res->bestValues);
	vector<double>** data	  = &(res->data);

	for (size_t i = 0; i < ui_ITERATIONS; i++)
	{
		vector<double>* args = getRandomVector(ui_SIZE, &d_min, &d_max);

		d_result = f(args);

		if (b_storeData)
		{
			(*data)->push_back(d_result);
		} // end if

		if (d_result < *d_best)
		{
			if (*bestArgs != nullptr)
			{
				delete (*bestArgs);
			} // end if

			*bestArgs = std::move(args);

			*d_best = d_result;

			// shrink search space whenever we find a better solution
			d_min = minValueInVector(args);
			d_max = maxValueInVector(args);
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for

	bestArgs = nullptr;
	data = nullptr;

	return res;
} // end method randomWalk