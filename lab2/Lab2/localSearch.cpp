#include "localSearch.hpp"
#include <limits>

#define DELTA_X 0.21
#define PRECISION 0.001

using namespace std;

results_t* localSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_DELTA_X)
{
	results_t* res = new results_t(); // result

	res->data = new vector<double>(); 

	vector<double>* data = res->data;				// pointer to data vector to make access faster
	vector<double>* x_local = new vector<double>(); // local solution

	// potential solutions
	double	d_added = numeric_limits<double>::max(),
			d_subtracted = numeric_limits<double>::max();

	double * d_best = &(res->d_bestValue); // pointer to best value to make access faster

	bool b_continue = true; // iteration variable

	res->bestValues = getRandomVector(ui_SIZE, &d_min, &d_max);
	
	res->d_bestValue = f(res->bestValues);

	while (b_continue)
	{
		x_local->clear();

		std::copy(res->bestValues->begin(), res->bestValues->end(), back_inserter(*(x_local)));

		b_continue = false;
		
		for (size_t i = 0; i < x_local->size(); i++)
		{
			double * current = &(x_local->at(i));

			// try adding dx
			*current += d_DELTA_X;

			d_added = f(x_local);

			data->push_back(d_added);

			// try subtracting dx
			*current -= 2.0 * d_DELTA_X;

			d_subtracted = f(x_local);

			data->push_back(d_subtracted);

			// revert changes
			*current += d_DELTA_X;

			// f(x_loc1) < f(x*) ^ f(x_loc1) < f(x_loc2) ^ dist(f(x_loc1),f(x*)) >= 0.001
			if (d_added < *d_best && d_added < d_subtracted && (getDistance(*d_best, d_added) >= PRECISION))
			{
				b_continue = true;

				res->bestValues->clear();

				std::copy(x_local->begin(), x_local->end(), back_inserter(*(res->bestValues)));

				res->bestValues->at(i) += d_DELTA_X;
				*d_best = d_added;
			} // end if
			else if (d_subtracted < *d_best && ( getDistance(*d_best, d_subtracted) >= PRECISION))
			{
				b_continue = true;

				res->bestValues->clear();

				std::copy(x_local->begin(), x_local->end(), back_inserter(*(res->bestValues)));

				res->bestValues->at(i) -= d_DELTA_X;
				*d_best = d_subtracted;
			} // end elif
		} // end for		
	} // end while

	delete x_local;

	return res;
} // end method localSearch