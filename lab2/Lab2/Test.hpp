#pragma once

#include <vector>
#include <initializer_list>

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

	template <typename F, typename... Args>
	void runTest(F f, const std::size_t ui_ITERATIONS, Args... args);


private:
	double	d_avgRange,
			d_avgStdDev,
			d_avgTime,
			d_avgValue,
		    d_avgBest;

	void storeResults(std::string);
	void makeMatrix(double**&);
	
};