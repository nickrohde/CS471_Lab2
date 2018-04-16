#pragma once
#include <vector>

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