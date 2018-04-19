#include "iterativeLocalSearch.hpp"
#include "localSearch.hpp"
#include <limits>

using namespace std;

results_t * iterativeLocalSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_deltaX, size_t ui_iterations)
{
	results_t * res = nullptr;

	res = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX);

	for (size_t i = 1; i < ui_iterations; i++)
	{
		results_t * temp = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX);

		if (temp->d_bestValue < res->d_bestValue)
		{
			delete res;
			res = temp;
			temp = nullptr;
		} // end if
	} // end for

	return res;
} // end method iterativeLocalSearch
