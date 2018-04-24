#include "iterativeLocalSearch.hpp"
#include "localSearch.hpp"
#include <limits>

using namespace std;

results_t * iterativeLocalSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_deltaX, bool b_storeData, size_t ui_iterations)
{
	results_t * p_res = nullptr;

	// initialize best value
	p_res = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX, b_storeData);

	for (size_t i = 1; i < ui_iterations; i++)
	{
		results_t * p_temp = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX, b_storeData);

		if (p_temp->d_bestValue < p_res->d_bestValue)
		{
			delete p_res;
			p_res = p_temp;
			p_temp = nullptr;
		} // end if
		else
		{
			delete p_temp;
		} // end else
	} // end for

	return p_res;
} // end method iterativeLocalSearch
