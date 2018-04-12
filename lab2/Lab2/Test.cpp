#include "Test.hpp"
#include "utility.hpp"
#include <fstream>


using namespace std;

void Test::runTest(results (*f) (double (*c) (const std::vector<double>*)), double (*c) (const std::vector<double>*), const size_t ui_ITERATIONS)
{
	for (size_t i = 0; i < ui_ITERATIONS; i++)
	{
		results temp = f(c);

		d_avgRange += temp.d_range;
		d_avgStdDev += temp.d_stdDev;
		d_avgTime += temp.d_avgTime;
		d_avgValue += temp.d_avgValue;

	} // end for
}

void Test::runTest(results (*f) (double (*c) (const std::vector<double>*, double **, std::size_t)), double (*c) (const std::vector<double>*, double **, std::size_t), const size_t ui_ITERATIONS)
{

}

void Test::storeResults(string s_fileName)
{
	ofstream results(s_fileName);


	results.close();
}
