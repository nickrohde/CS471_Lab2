#include "iterativeLocalSearch.hpp"
#include "localSearch.hpp"
#include <limits>


using namespace std;

results_t * iterativeLocalSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_deltaX, bool b_storeData, size_t ui_iterations)
{
	results_t * p_res = nullptr;

	// initialize best value
	p_res = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX, b_storeData);

	size_t ui_functionCalls = 0; // total # of function calls
	bool b_add = true;
	double d_deltaChange = DELTA_X_CHANGE_START_VALUE; // the value by which we will change delta x each iteration
													   // we are starting at 50 times our precision with this value

	cout << endl << " -------------- " << endl << endl;

	for (size_t i = 1; i < ui_iterations; i++)
	{
		// tune delta_x for local search
		if (b_add)
		{
			d_deltaX += d_deltaChange;
			cout << d_deltaX << ",";
		} // end if
		else
		{
			d_deltaX -= d_deltaChange;
			cout << d_deltaX << ",";
		} // end else

		results_t * p_temp = localSearch(f, ui_SIZE, d_min, d_max, d_deltaX, b_storeData);

		ui_functionCalls += p_temp->ui_functionCalls;

		// our value is definitely better than what we had before, so delta x is not changed here
		if (p_temp->d_bestValue < p_res->d_bestValue)
		{
			delete p_res;
			p_res = p_temp;
			p_temp = nullptr;
		} // end if

		// our new value did not improve, we will try to tweak delta x in an attempt to improve
		else
		{
			b_add = !b_add; // switch direction of change for delta x (instead of adding we will subtract and vice versa)
			
			// the idea with this change is that we will try to zero in on one optimal value of delta x. When we get a worse solution by adding, we instead subtract a smaller value
			// and when we get a worse solution then, we will start adding an even smaller value. Eventually, in theory, we should converge on some middle value between two points
			// which will give us the most optimal value.
			d_deltaChange -= DELTA_X_CHANGE_VALUE; // decrease the value of change applied to delta x, otherwise we will just occilate between two values 
												   // we will change it by 5 times our precision


			delete p_temp;
		} // end else
	} // end for

	p_res->ui_functionCalls = ui_functionCalls;
	p_res->d_finalDeltaX = d_deltaX;

	return p_res;
} // end method iterativeLocalSearch
