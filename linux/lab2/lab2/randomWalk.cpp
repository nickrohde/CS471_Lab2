#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

#ifndef NULL
#define NULL 0
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

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


void randomWalk(double(*f)(const vector<double>*), const size_t ui_SIZE, const uint ui_ITERATIONS, const double* d_MIN, const double* d_MAX)
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

			d_best = d_result;
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for
	
	results << ",\n,Best Solution:, " << d_best;
	results << ",\n,Best args:,";
	for(size_t i = 0; i < ui_SIZE; i++)
	{
		results << bestArgs->at(i) << ",";
	} // end for
	
	results << ",\n";
	
	results.close();
} // end method randomWalk


inline double firstDeJongsFunction(const vector<double>* vect)
{
	double total = 0.0;
	
//#pragma loop(hint_parallel(0))
	// SUM[1->n]
	for(auto& d: *vect)
	{
		total += d * d; // (x_i)^2
	} // end for
	
	return total;
} // end method firstDeJongsFunction


inline double rosenbrockFunction(const vector<double>* vect)
{
	double total = 0.0;
	
#pragma loop(hint_parallel(0))
	// SUM[1->n-1]
	for(std::size_t i = 0; i < vect->size() - 1; i++)
	{
		double temp = 1 - vect->at(i),                            // (1 - x_i)
			   temp2 = vect->at(i) * vect->at(i) - vect->at(i+1); // (x_i^2 - x_i+1)

		temp *= temp;   // (1-x_i)^2

		temp2 *= temp2; // (x_i^2 - x_i+1)^2
		temp2 *= 100;   // 100(x_i^2 - x_i+1)^2
		
		total += temp2 + temp; // 100(x_i^2 - x_i+1)^2 + (1-x_i)^2
	} // end for
	
	return total;
} // end method rosenbrockFunction


int main(int argc, char ** argv)
{
	double 	min1 = -100,
			max1 = 100;
	
	randomWalk(firstDeJongsFunction, 30, 1000000, &min1, &max1);
	randomWalk(rosenbrockFunction, 30, 1000000, &min1, &max1);



	return EXIT_SUCCESS;
}