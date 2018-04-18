#pragma once

#ifndef _RESULTS_H
#define _RESULTS_H

#include <vector>
#include <iostream>

class results_t
{
public:
	// Public Functions:
	results_t(void)
	{
		d_range = d_stdDev = d_avgTime = d_avgValue = d_bestValue = d_median = 0.0;
		bestValues = nullptr;
		data = nullptr;
	} // end Constructor

	// Operators:
	friend std::ostream& operator<<(std::ostream& stream, results_t& res);

	// Public Data Members:
	double d_range, d_stdDev, d_avgTime, d_avgValue, d_bestValue, d_median;

	std::vector<double>* bestValues;
	std::vector<double>* data;
};

std::ostream& operator<<(std::ostream& stream, results_t& res)
{
	stream << "Optimum found: " << res.d_bestValue << ",\n";

	stream << "Optimal point: ";

	for (size_t i = 0; i < res.bestValues->size(); i++)
	{
		stream << ", " << res.bestValues->at(i);
	} // end for

	stream << ",\n";

	stream << "Mean: " << res.d_avgValue << ",\n";
	stream << "Median: " << res.d_median << ",\n";
	stream << "Time: " << res.d_avgTime << ",\n";
	stream << "Median: " << res.d_range << ",\n";
	stream << "SD: " << res.d_stdDev << ",\n\n";

	return stream;
} // end operator << 

#endif