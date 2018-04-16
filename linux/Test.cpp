#include "Test.hpp"
#include <fstream>


using namespace std;


Test::Test(void)
{
	da_ranges = new double*[NUMBER_FUNCTIONS]; // array containing the ranges for the RNG; [i][0] contains min, [i][1] contains max for function f_i

	makeRanges(da_ranges); // make matrix with ranges for functions


	da_A = new double*[SHEKEL_OUTER_SIZE]; // shekels foxhole parameter that is bound to the function

	makeMatrix(da_A); // make matrix A for shekels foxhole


	costFunctions = getAllCostFunctions(const_cast<const double**>(da_A), ui_SHEKEL_ITERATIONS); // vector containing the cost functions

	fileNames.push_back("10_dimensions.csv");
	fileNames.push_back("20_dimensions.csv");
	fileNames.push_back("30_dimensions.csv");

	compute_start = compute_end = highRes_Clock::now();
} // end Default Constructor


Test::~Test(void)
{
	if (da_ranges != nullptr)
	{
		for (size_t i = 0; i < NUMBER_FUNCTIONS; i++)
		{
			delete[] da_ranges[i];
		} // end for
	} // end if

	delete[] da_ranges;

	if (da_A != nullptr)
	{
		for (size_t i = 0; i < SHEKEL_OUTER_SIZE; i++)
		{
			delete[] da_A[i];
		} // end for
	} // end if

	delete[] da_A;
} // end Destructor



/*
 * Type F: Strategy function (e.g. RW, LS, ILS, etc.)
 * Type Args: Specific arguments for strategy function
 *
 * Executes the provided strategy with all 15 cost functions n times, where n is arg2.
 * After testing is done, the statistics are calculated, and all data written to a csv file.
 *
 */
template <typename F>
void Test::runTest(F f, const size_t ui_ITERATIONS)
{
	// run all tests on 10, 20, and 30 dimensions
	for (size_t ui_length = 10; ui_length <= 30; ui_length += 10)
	{
		vector<results> res; // for statistics

		// run each cost function with current dimension
		for (int i = 0; i < costFunctions.size(); i++)
		{			
			if (i == cost_functions::i_SHEKEL_INDEX && ui_length > 10) // only execute shekel with 10 dimensions
			{
				continue;
			} // end if

			// initialize results for current function
			res.push_back(results());

			// pointer to current results
			results * temp = &res.at(i);

			// run each function n times
			for (size_t j = 0; j < ui_ITERATIONS; j++)
			{
				compute_start = highRes_Clock::now();

				results* temp2 = f(costFunctions[i], ui_length, ui_ITERATIONS, da_ranges[i][0], da_ranges[i][1]);
				compute_end = highRes_Clock::now();

				// update best value only when new best is found
				if (temp->d_bestValue < temp2->d_bestValue)
				{
					temp->d_bestValue = temp2->d_bestValue;
					temp->bestValues  = std::move(temp2->bestValues);
				} // end if

				time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

				// statistics
				temp->d_range  += getRange(temp2->data);
				temp->d_stdDev += getStandardDeviation(temp2->data, temp2->d_avgValue);
				temp->d_median += getMedian(temp2->data);
				temp->d_avgValue += temp2->d_bestValue;
				temp->d_avgTime  += time_to_compute.count();
			} // end for j

			// calculate averages
			temp->d_avgValue /= ui_ITERATIONS;
			temp->d_range    /= ui_ITERATIONS;
			temp->d_stdDev   /= ui_ITERATIONS;
			temp->d_avgTime  /= ui_ITERATIONS;
		} // end for i

		storeResults(fileNames.at((ui_length / 10) - 1), res);
	} // end for length
} // end template runTest


void Test::storeResults(string& s_fileName, vector<results>& res)
{
	ofstream results(s_fileName, ios::app | ios::out);

	for (size_t i = 0; i < res.size(); i++)
	{
		results << "F" << (i + 1) << ",\n";
		results << res.at(i);
	} // end for

	results.close();
} // end method storeResults


inline void Test::makeMatrix(double**& da_A)
{
	da_A[0]  = new double[SHEKEL_INNER_SIZE]{ 9.681, 0.667, 4.783, 9.095, 3.517, 9.325, 6.544, 0.211, 5.122, 2.02 };
	da_A[1]  = new double[SHEKEL_INNER_SIZE]{ 9.4, 2.041, 3.788, 7.931, 2.882, 2.672, 3.568, 1.284, 7.033, 7.374 };
	da_A[2]  = new double[SHEKEL_INNER_SIZE]{ 8.025, 9.152, 5.114, 7.621, 4.564, 4.711, 2.996, 6.126, 0.734, 4.982 };
	da_A[3]  = new double[SHEKEL_INNER_SIZE]{ 2.196, 0.415, 5.649, 6.979, 9.510, 9.166, 6.304, 6.054, 9.377, 1.426 };
	da_A[4]  = new double[SHEKEL_INNER_SIZE]{ 8.074, 8.777, 3.467, 1.863, 6.708, 6.349, 4.534, 0.276, 7.633, 1.567 };
	da_A[5]  = new double[SHEKEL_INNER_SIZE]{ 7.650, 5.658, 0.720, 2.764, 3.278, 5.283, 7.474, 6.274, 1.409, 8.208 };
	da_A[6]  = new double[SHEKEL_INNER_SIZE]{ 1.256, 3.605, 8.623, 6.905, 4.584, 8.133, 6.071, 6.888, 4.187, 5.448 };
	da_A[7]  = new double[SHEKEL_INNER_SIZE]{ 8.314, 2.261, 4.24, 1.781, 4.124, 0.932, 8.129, 8.658, 1.208, 5.762 };
	da_A[8]  = new double[SHEKEL_INNER_SIZE]{ 0.226, 8.858, 1.42, 0.954, 1.622, 4.698, 6.228, 9.096, 0.972, 7.637 };
	da_A[9]  = new double[SHEKEL_INNER_SIZE]{ 7.305, 2.228, 1.242, 5.928, 9.133, 1.826, 4.06, 5.204, 8.713, 8.247 };
	da_A[10] = new double[SHEKEL_INNER_SIZE]{ 0.652, 7.027, 0.508, 4.876, 8.807, 4.632, 5.808, 6.937, 3.291, 7.016 };
	da_A[11] = new double[SHEKEL_INNER_SIZE]{ 2.699, 3.516, 5.847, 4.119, 4.461, 7.496, 8.817, 0.69, 6.593, 9.789 };
	da_A[12] = new double[SHEKEL_INNER_SIZE]{ 8.327, 3.897, 2.017, 9.57, 9.825, 1.15, 1.395, 3.885, 6.354, 0.109 };
	da_A[13] = new double[SHEKEL_INNER_SIZE]{ 2.132, 7.006, 7.136, 2.641, 1.882, 5.943, 7.273, 7.691, 2.88, 0.564 };
	da_A[14] = new double[SHEKEL_INNER_SIZE]{ 4.707, 5.579, 4.08, 0.581, 9.698, 8.542, 8.077, 8.515, 9.231, 4.67 };
	da_A[15] = new double[SHEKEL_INNER_SIZE]{ 8.304, 7.559, 8.567, 0.322, 7.128, 8.392, 1.472, 8.524, 2.277, 7.826 };
	da_A[16] = new double[SHEKEL_INNER_SIZE]{ 8.632, 4.409, 4.832, 5.768, 7.05, 6.715, 1.711, 4.323, 4.405, 4.591 };
	da_A[17] = new double[SHEKEL_INNER_SIZE]{ 4.887, 9.112, 0.17, 8.967, 9.693, 9.867, 7.508, 7.77, 8.382, 6.74 };
	da_A[18] = new double[SHEKEL_INNER_SIZE]{ 2.44, 6.686, 4.299, 1.007, 7.008, 1.427, 9.398, 8.48, 9.95, 1.675 };
	da_A[19] = new double[SHEKEL_INNER_SIZE]{ 6.306, 8.583, 6.084, 1.138, 4.350, 3.134, 7.853, 6.061, 7.457, 2.258 };
	da_A[20] = new double[SHEKEL_INNER_SIZE]{ 0.652, 2.343, 1.37, 0.821, 1.31, 1.063, 0.689, 8.819, 8.833, 9.07 };
	da_A[21] = new double[SHEKEL_INNER_SIZE]{ 5.558, 1.272, 5.756, 9.857, 2.279, 2.764, 1.284, 1.677, 1.244, 1.234 };
	da_A[22] = new double[SHEKEL_INNER_SIZE]{ 3.352, 7.549, 9.817, 9.437, 8.687, 4.167, 2.57, 6.54, 0.228, 0.027 };
	da_A[23] = new double[SHEKEL_INNER_SIZE]{ 8.798, 0.88, 2.37, 0.168, 1.701, 3.68, 1.231, 2.39, 2.499, 0.064 };
	da_A[24] = new double[SHEKEL_INNER_SIZE]{ 1.46, 8.057, 1.337, 7.217, 7.914, 3.615, 9.981, 9.198, 5.292, 1.224 };
	da_A[25] = new double[SHEKEL_INNER_SIZE]{ 0.432, 8.645, 8.774, 0.249, 8.081, 7.461, 4.416, 0.652, 4.002, 4.644 };
	da_A[26] = new double[SHEKEL_INNER_SIZE]{ 0.679, 2.8, 5.523, 3.049, 2.968, 7.225, 6.73, 4.199, 9.614, 9.229 };
	da_A[27] = new double[SHEKEL_INNER_SIZE]{ 4.263, 1.074, 7.286, 5.599, 8.291, 5.2, 9.214, 8.272, 4.398, 4.506 };
	da_A[28] = new double[SHEKEL_INNER_SIZE]{ 9.496, 4.83, 3.15, 8.27, 5.079, 1.231, 5.731, 9.494, 1.883, 9.732 };
	da_A[29] = new double[SHEKEL_INNER_SIZE]{ 4.138, 2.562, 2.532, 9.661, 5.611, 5.5, 6.886, 2.341, 9.699, 6.5 };
} // end method makeMatrix


inline void Test::makeRanges(double**& ranges)
{
	ranges[0]  = new double[2]{ -512, 512 };
	ranges[1]  = new double[2]{ -100, 100 };
	ranges[2]  = new double[2]{ -100, 100 };
	ranges[3]  = new double[2]{  -30,  30 };
	ranges[4]  = new double[2]{ -500, 500 };
	ranges[5]  = new double[2]{  -30,  30 };
	ranges[6]  = new double[2]{  -30,  30 };
	ranges[7]  = new double[2]{  -32,  32 };
	ranges[8]  = new double[2]{  -32,  32 };
	ranges[9]  = new double[2]{ -500, 500 };
	ranges[10] = new double[2]{ -500, 500 };
	ranges[11] = new double[2]{ -100, 100 };
	ranges[12] = new double[2]{    0, _PI };
	ranges[13] = new double[2]{  -30,  30 };
	ranges[14] = new double[2]{    0,  10 };
} // end method makeRanges


std::ostream& operator<<(std::ostream& stream, results& res)
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

