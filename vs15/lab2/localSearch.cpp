#include "localSearch.hpp"
#include <limits>

#define DELTA_X 0.21

using namespace std;

results_t* localSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_DELTA_X)
{
	vector<double>* data = new vector<double>();

	bool b_continue = true;
	
	results_t* res = new results_t();

	res->bestValues = getRandomVector(ui_SIZE, &d_min, &d_max);
	
	res->d_bestValue = f(res->bestValues);

	while (b_continue)
	{
		vector<double>* x_local = new vector<double>();
		
		std::copy(res->bestValues->begin(), res->bestValues->end(), back_inserter(*(x_local)));

		double	d_added		 = numeric_limits<double>::max(),
				d_subtracted = numeric_limits<double>::max();

		double * d_best = &(res->d_bestValue);

		b_continue = false;
		
		for (size_t i = 0; i < x_local->size(); i++)
		{
			// try adding dx
			x_local->at(i) += d_DELTA_X;

			d_added = f(x_local);

			// try subtracting dx
			x_local->at(i) -= 2.0 * d_DELTA_X;

			d_subtracted = f(x_local);

			// revert changes
			x_local->at(i) += d_DELTA_X;

			if (d_added < *d_best && d_added <= d_subtracted)
			{
				b_continue = true;

				res->bestValues->clear();

				std::copy(x_local->begin(), x_local->end(), back_inserter(*(res->bestValues)));

				res->bestValues->at(i) += d_DELTA_X;
				*d_best = d_added;
			} // end if
			else if (d_subtracted < *d_best)
			{
				b_continue = true;

				res->bestValues->clear();

				std::copy(x_local->begin(), x_local->end(), back_inserter(*(res->bestValues)));

				res->bestValues->at(i) -= d_DELTA_X;
				*d_best = d_subtracted;
			} // end elif

		} // end for

		
		
		
		
		
	} // end while



	return res;
}