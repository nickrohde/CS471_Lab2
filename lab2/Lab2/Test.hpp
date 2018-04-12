#pragma once

#include <vector>

struct results
{
	double d_range, d_stdDev, d_avgTime, d_avgValue, d_bestValue;
	std::vector<double>* bestValues;
};



class Test
{
public:
	Test(void) 
	{
		d_avgRange = d_avgStdDev = d_avgTime = d_avgValue = 0;
	} // end Default Constructor

	void runTest(results (*f) (double (*c) (const std::vector<double>*)), double (*c) (const std::vector<double>*), const size_t);
	void runTest(results (*f) (double (*c) (const std::vector<double>*, double**, std::size_t)), double (*c) (const std::vector<double>*, double**, std::size_t), const size_t);



private:
	double	d_avgRange,
			d_avgStdDev,
			d_avgTime,
			d_avgValue;

	void storeResults(std::string);

};