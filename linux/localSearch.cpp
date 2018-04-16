#include "localSearch.hpp"

#define DELTA_X 0.21

using namespace std;

results * localSearch(costFunction f, const std::size_t ui_SIZE, const std::size_t ui_ITERATIONS, double d_min, double d_max, std::vector<double>* x_0)
{
	vector<double>* x_best = new vector<double>();
	
	bool b_continue = true;
	
	results * res = new results();
	
	x_best = std::copy(x_0);
	
	while (b_continue)
	{
		std::vector<double>* x_local = new vector<double>();
		
		b_continue = false;
		
		
		
		
		
		
	} // end while



	return nullptr;
}

void getNeighbour(std::vector<double>* vect)
{
}
