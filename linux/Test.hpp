#pragma once

#ifndef _TEST_H
#define _TEST_H
#include <initializer_list>
#include "functions.hpp"


#define SHEKEL_OUTER_SIZE 30
#define SHEKEL_INNER_SIZE 10
#define NUMBER_FUNCTIONS  15


class results
{
public:

	results()
	{
		d_range = d_stdDev = d_avgTime = d_avgValue = d_bestValue = d_median = 0.0;
		bestValues = nullptr;
		data = nullptr;
	} // end Constructor

	friend std::ostream& operator<<(std::ostream& stream, results& res);


	double d_range, d_stdDev, d_avgTime, d_avgValue, d_bestValue, d_median;
	std::vector<double>* bestValues;
	std::vector<double>* data;
};



class Test
{
public:
	Test(void);
	~Test(void);

	template <typename F>
	void runTest(F f, const std::size_t ui_ITERATIONS);


private:
	const std::size_t ui_SHEKEL_ITERATIONS = 10;

	double ** da_ranges,
		   ** da_A;

	std::vector<costFunction> costFunctions;

	std::vector<std::string> fileNames;

	void storeResults(std::string&, std::vector<results>&);
	void makeMatrix(double**&);
	void makeRanges(double**&);

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;
	
};

#endif // !_TEST_H