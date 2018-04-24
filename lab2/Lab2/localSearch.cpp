#include "localSearch.hpp"
#include <limits>

#define PRECISION 0.001

using namespace std;

results_t* localSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_DELTA_X, bool b_storeData)
{
	results_t* res = new results_t(); // result

	if (b_storeData)
	{
		res->data = new vector<double>();
	} // end if

	// pointers for convenience
	vector<double>** p_data = &(res->data);
	vector<double>** p_best = &(res->bestValues);

	vector<double>* x_local = new vector<double>(); // local solution

	// potential solutions
	double	d_added      = numeric_limits<double>::max(),
			d_subtracted = numeric_limits<double>::max();

	double* d_best = &(res->d_bestValue); // pointer for convenience

	bool b_continue = true; // iteration variable

	// get initial point
	*p_best = getRandomVector(ui_SIZE, &d_min, &d_max);	
	*d_best = f(*p_best);

	while (b_continue)
	{
		x_local->clear(); // get rid of local point
		std::copy((*p_best)->begin(), (*p_best)->end(), back_inserter(*(x_local))); // continue with best point (still need best solution, can't use efficient std::move)

		b_continue = false;
		
		for (size_t i = 0; i < x_local->size(); i++)
		{
			double* current = &(x_local->at(i)); // pointer for convenience

			// try adding dx
			*current += d_DELTA_X;
			d_added = f(x_local);

			if (b_storeData)
			{
				(*p_data)->push_back(d_added);
			} // end if

			// try subtracting dx
			*current -= 2.0 * d_DELTA_X;
			d_subtracted = f(x_local);

			if (b_storeData)
			{
				(*p_data)->push_back(d_subtracted);
			} // end if

			// revert changes
			*current += d_DELTA_X;

			// f(x_loc1) < f(x*) ^ f(x_loc1) < f(x_loc2) ^ dist(f(x_loc1),f(x*)) >= 0.001
			if (d_added < *d_best && d_added < d_subtracted && (getDistance(*d_best, d_added) >= PRECISION))
			{
				b_continue = true;

				(*p_best)->clear();

				// still need x_local, so can't use the efficient std::move
				std::copy(x_local->begin(), x_local->end(), back_inserter(*(*p_best)));

				(*p_best)->at(i) += d_DELTA_X;
				*d_best = d_added;
			} // end if
			else if (d_subtracted < *d_best && (getDistance(*d_best, d_subtracted) >= PRECISION))
			{
				b_continue = true;

				(*p_best)->clear();

				// still need x_local, so can't use the efficient std::move
				std::copy(x_local->begin(), x_local->end(), back_inserter(*(*p_best)));

				(*p_best)->at(i) -= d_DELTA_X;
				*d_best = d_subtracted;
			} // end elif
		} // end for		
	} // end while

	// null local pointers
	p_best = nullptr;
	p_data = nullptr;
	d_best = nullptr;

	delete x_local;

	return res;
} // end method localSearch