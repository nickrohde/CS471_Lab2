#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

typedef unsigned int uint;


template <typename T>
inline T getRandomNumberInRange(const T* p_MIN, const T* p_MAX)
{
	static std::random_device rd{};
	static std::mt19937 engine{ rd() };
	static std::uniform_real_distribution<T> dist{ *p_MIN, *p_MAX };

	return dist(engine);
} // end template getRandomNumberInRange


template <typename T>
inline vector<T>* getRandomVector(const std::size_t ui_SIZE, const T* p_MIN, const T* p_MAX)
{
	vector<T>* vec = new vector<T>(ui_SIZE);

	for (std::size_t i = 0; i < ui_SIZE; i++)
	{
		vec->at(i) = (getRandomNumberInRange(p_MIN, p_MAX));
	} // end for

	return vec;
} // end template getRandomVector


void randomWalk(double(*f)(vector<double>*), const size_t ui_SIZE, const uint ui_ITERATIONS, const double* d_MIN, const double* d_MAX)
{
	ofstream results("randomWalk.csv", ios::app | ios::out);
	
	double	d_best = exp(280),
			d_result = 0;

	vector<double>* bestArgs = NULL;

	for (size_t i = 0; i < ui_ITERATIONS; i++)
	{
		vector<double>* args = getRandomVector(ui_SIZE, d_MIN, d_MAX);
		
		d_result = f(args);

		results << d_result << ",";

		if (d_result < d_best)
		{
			if (bestArgs != NULL)
			{
				delete bestArgs;
			} // end if
			
			bestArgs = args;

			d_best = d_best;
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for
}


int main(int argc, char ** argv)
{




	return EXIT_SUCCESS;
}