
#include <cmath>
#include <functional>
#include <cstdarg>
#include <limits>
#include <fstream>
#include <initializer_list>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cctype>

#ifndef _PI
#define _PI 3.141592653589793238462643383279
#endif // !_PI

#ifndef _E
#define _E  2.718281828459045235360287471352
#endif // !_E

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif // !EXIT_SUCCESS

#ifndef NULL
#define NULL 0
#endif

typedef unsigned int uint;

typedef std::chrono::high_resolution_clock::time_point timePoint;

typedef std::chrono::duration<double> duration;

typedef std::chrono::high_resolution_clock highRes_Clock;


template <typename T>
inline T getRandomNumberInRange(const T* p_MIN, const T* p_MAX)
{
	static std::random_device rd{};
	static std::mt19937 engine{ rd() };
	static std::uniform_real_distribution<T> dist{ *p_MIN, *p_MAX };

	return dist(engine);
} // end template getRandomNumberInRange


template <typename T>
std::vector<T>* getRandomVector(const std::size_t ui_SIZE, const T* p_MIN, const T* p_MAX)
{
	std::vector<T>* vec = new std::vector<T>(ui_SIZE);

	for (std::size_t i = 0; i < ui_SIZE; i++)
	{
		vec->at(i) = (getRandomNumberInRange(p_MIN, p_MAX));
	} // end for

	return vec;
} // end template getRandomVector


template <typename T>
double getStandardDeviation(const std::vector<T>* p_data, const double d_mean)
{
	double d_standardDeviation = 0.0;

#pragma loop(hint_parallel(0))
	for (size_t i = 0; i < p_data->size(); i++)
	{
		d_standardDeviation += pow(p_data->at(i) - d_mean, 2);
	} // end for

	return sqrt(d_standardDeviation / static_cast<double>(p_data->size()));
} // end template getStandardDeviation


template <typename T>
inline double getRange(const std::vector<T>* p_data)
{
	T	result,
		min = p_data->at(0),
		max = p_data->at(0);


	for (size_t i = 0; i < p_data->size(); i++)
	{
		if (p_data->at(i) < min)
		{
			min = p_data->at(i);
		} // end if
		if (p_data->at(i) > max)
		{
			max = p_data->at(i);
		} // end if
	} // end if

	result = max - min;

	return result;
} // end template getRange


template <typename T>
inline T getMedian(std::vector<T>* p_data)
{
	std::sort(p_data->begin(), p_data->end());

	if (p_data->size() % 2)
	{
		return p_data->at(p_data->size() / 2);
	} // end if
	else
	{
		T temp = p_data->at(p_data->size() / 2);
		temp += p_data->at((p_data->size() / 2) + 1);
		return (temp / 2);
	} // end else
} // end template getMedian


template <typename T>
inline T maxValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(p_vect->size() - 1);
} // end template maxValueInVector


template <typename T>
inline T minValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(0);
} // end template minValueInVector


#define SHEKEL_OUTER_SIZE 30
#define SHEKEL_INNER_SIZE 10
#define NUMBER_FUNCTIONS 15

class results
{
public:

	results()
	{
		d_range = d_stdDev = d_avgTime = d_avgValue = d_median = 0.0;
		d_bestValue = std::numeric_limits<double>::max();
		bestValues;
		data;
	} // end Constructor

	friend std::ostream& operator<<(std::ostream& stream, results& res);


	double d_range, d_stdDev, d_avgTime, d_avgValue, d_bestValue, d_median;
	std::vector<double> bestValues;
	std::vector<double> data;
};


namespace cost_functions
{
	typedef std::function<double(const std::vector<double>*)> costFunction; // readability

	const int i_SHEKEL_INDEX = 14;


	inline double schwefelsFunction(const std::vector<double>* vect)
	{
		double total = 0.0;

		// SUM[1->n]
		for (auto& d : *vect)
		{
			double	temp = -1 * d,       // -x_i
				root = sqrt(abs(d)); // sqrt( | x_i | )
			temp *= sin(root);           // sin( sqrt( | x_i | ) )
			total += temp;               // sum
		} // end for

		return total;
	} // end method schwefelsFunction


	inline double firstDeJongsFunction(const std::vector<double>* vect)
	{
		double total = 0.0;

		// SUM[1->n]
		for (auto& d : *vect)
		{
			total += d * d; // (x_i)^2
		} // end for

		return total;
	} // end method firstDeJongsFunction


	inline double rosenbrockFunction(const std::vector<double>* vect)
	{
		double total = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double temp = 1 - vect->at(i),                            // (1 - x_i)
				temp2 = vect->at(i) * vect->at(i) - vect->at(i + 1); // (x_i^2 - x_i+1)

			temp *= temp;   // (1-x_i)^2

			temp2 *= temp2; // (x_i^2 - x_i+1)^2
			temp2 *= 100;   // 100(x_i^2 - x_i+1)^2

			total += temp2 + temp; // 100(x_i^2 - x_i+1)^2 + (1-x_i)^2
		} // end for

		return total;
	} // end method rosenbrockFunction


	inline double rastriginFunction(const std::vector<double>* vect)
	{
		double total = 0.0;
		//* Using the function 10 * n + [SUM(x^2 - 10cos(2*pi*x)] - 200*N
#pragma loop(hint_parallel(0))
		// SUM[1->n]
		for (std::size_t i = 0; i < vect->size(); i++)
		{
			double  temp = pow(vect->at(i), 2),					// x_i^2
				temp2 = 10 * (cos(2 * _PI * vect->at(i)));	// 10cos(2pi * x_i)

			total += (temp - temp2);							// x_i^2 - 10cos(2pi * x_i)
		} // end for

		total += (10 * vect->size());							// 10*n + SUM

		total -= 200 * vect->size();							// shift down by -200n to move optimal point from 0 to -200n
																/*/
																// Using the function 2 * n * SUM(x^2 - 10cos(2*pi*x)
																#pragma loop(hint_parallel(0))
																// SUM[1->n]
																for(std::size_t i = 0; i < vect->size(); i++)
																{
																double temp  = pow(vect->at(i), 2),           // x_i^2
																temp2 = cos((_PI * 2) * vect->at(i)); // cos(2pi * x_i)

																temp2 *= 10;             // 10cos(2pi * x_i)

																total += temp - temp2;   // x_i^2 - 10cos(2pi * x_i)
																} // end for

																total *= (2 * vect->size()); // 2n * SUM
																//*/
		return total;
	} // end method rastriginFunction                                     


	inline double griewangkFunction(const std::vector<double>* vect)
	{
		double total = 0.0,
			sum = 0.0, // SUM[1->n]
			product = 1.0; // PROD[1->n]

#pragma loop(hint_parallel(0))
		for (std::size_t i = 0; i < vect->size(); i++)
		{
			// SUM[1->n]
			{
				double tempSum = vect->at(i) * vect->at(i); // x_i^2

				tempSum /= 4000; // (x_i)^2 / 4000

				sum += tempSum;
			} // end SUM[1->n]

			  // PROD[1->n]
			{
				double tempProd = vect->at(i) / sqrt(static_cast<double>(i + 1)); // x_i / sqrt(i)

				tempProd = cos(tempProd); // cos( x_i / sqrt(i) )

				product *= tempProd;
			} // end PROD[1->n]
		} // end for

		total += 1 + sum - product; // 1 + SUM - PROD

		return total;
	} // end method griewangkFunction


	inline double sineEnvelopeSineWaveFunction(const std::vector<double>* vect)
	{
		double total = 0.5 * (vect->size() - 1), // 0.5(n-1) + SUM
			sum = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double temp2 = 0,
				quotient = 0,
				sumOfSquares = 0;

			sumOfSquares = vect->at(i) * vect->at(i);      // x_i^2
			sumOfSquares += vect->at(i + 1) * vect->at(i + 1); // x_i^2 + (x_i+1)^2

			sum = sumOfSquares - 0.5;     // x_i^2 + (x_i+1)^2 - 0.5
			sum = sin(sum);               // sin( x_i^2 + (x_i+1)^2 - 0.5 )
			sum = sum * sum;              // sin^2( x_i^2 + (x_i+1)^2 - 0.5 )

			temp2 = sumOfSquares * 0.001; //  0.001(x_i^2 + (x_i+1)^2)
			temp2 += 1;                   //  0.001(x_i^2 + (x_i+1)^2) + 1
			temp2 = temp2 * temp2;		  // (0.001(x_i^2 + (x_i+1)^2) + 1)^2

			quotient = sum / temp2;         // sin( x_i^2 + (x_i+1)^2 - 0.5 )^2 / (0.001(x_i^2 + (x_i+1)^2) + 1)^2

			total += quotient;
		} // end for

		total *= -1; // - SUM

		return total;
	} // end method sineEnvelopeSineWaveFunction


	inline double stretchedVSineWaveFunction(const std::vector<double>* vect)
	{
		double total = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double sumOfSquares = 0.0,
				product = 0.0,
				temp = 0.0,
				temp2 = 0.0;

			sumOfSquares = vect->at(i) * vect->at(i);          // x_i^2 
			sumOfSquares += vect->at(i + 1) * vect->at(i + 1); // x_i^2 + (x_i+1)^2

			temp = pow(sumOfSquares, 0.25); // root_4(x_i^2 + (x_i+1)^2)
			temp2 = pow(sumOfSquares, 0.1); // root_10(x_i^2 + (x_i+1)^2)

			temp2 *= 50.0;					// 50( root_10( x_i^2 + (x_i+1)^2 ))

			temp2 = sin(temp2);             // sin( 50( root_10( x_i^2 + (x_i+1)^2 )))
			temp2 = temp2 * temp2;          // sin^2( 50( root_10( x_i^2 + (x_i+1)^2 )))
			temp2 += 1;						// sin^2( 50( root_10( x_i^2 + (x_i+1)^2 ))) + 1

			product = temp2 * temp;         // (sin^2( 50( root_10( x_i^2 + (x_i+1)^2 ))) + 1) * root_4(x_i^2 + (x_i+1)^2)

			total += product;				// (sin^2( 50( root_10( x_i^2 + (x_i+1)^2 ))) + 1) * root_4(x_i^2 + (x_i+1)^2) 
		} // end for

		return total;
	} // end method stretchedVSineWaveFunction


	inline double ackleysOneFunction(const std::vector<double>* vect)
	{
		const double oneOverE = 1 / pow(_E, 0.2); // 1 / e ^ 0.2

		double total = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double sumOfSquares = 0,
				temp = 0,
				temp2 = 0;

			sumOfSquares = vect->at(i) * vect->at(i);          // x_i^2 
			sumOfSquares += vect->at(i + 1) * vect->at(i + 1); // x_i^2 + (x_i+1)^2

			sumOfSquares = sqrt(sumOfSquares); // sqrt(x_i ^ 2 + (x_i + 1) ^ 2)
			sumOfSquares *= oneOverE;          // 1/(e^0.2) * sqrt(x_i ^ 2 + (x_i + 1) ^ 2)

			temp = cos(2 * vect->at(i));      // cos(2 * x_i)
			temp2 = sin(2 * vect->at(i + 1));  // sin(2 * x_i+1)

			temp += temp2;                     // cos(2 * x_i) + sin(2 * x_i+1)
			temp *= 3;						   // 3(cos(2 * x_i) + sin(2 * x_i+1))

			total += temp + sumOfSquares;      // 1/(e^0.2) * sqrt(x_i ^ 2 + (x_i + 1) ^ 2) + 3(cos(2 * x_i) + sin(2 * x_i+1))
		} // end for

		return total;
	} // end method ackleysOneFunction


	inline double ackleysTwoFunction(const std::vector<double>* vect)
	{
		double total = 20.0 * (vect->size() - 1.0), // 20(n-1) + SUM
			product = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double sumOfSquares = 0.0,
				temp = 0.0,
				temp2 = 0.0;

			temp = cos(2 * _PI * vect->at(i));     // cos(2pi * x_i)
			temp2 = cos(2 * _PI * vect->at(i + 1)); // cos(2pi * x_i+1)

			temp += temp2;                           // cos(2pi * x_i) + cos(2pi * x_i+1)
			temp *= 0.5;                             // (cos(2pi * x_i) + cos(2pi * x_i+1))/2

			temp2 = exp(temp);                       // e ^ ((cos(2pi * x_i) + cos(2pi * x_i+1))/2)

			sumOfSquares = vect->at(i) * vect->at(i);          // x_i^2 
			sumOfSquares += vect->at(i + 1) * vect->at(i + 1); // x_i^2 + (x_i+1)^2
			sumOfSquares /= 2;								   // (x_i^2 + (x_i+1)^2)/2

			sumOfSquares = sqrt(sumOfSquares);                 // sqrt((x_i^2 + (x_i+1)^2)/2)
			sumOfSquares *= 0.2;                               // (sqrt((x_i^2 + (x_i+1)^2)/2))/5

			temp = exp(sumOfSquares);                          // e ^ (sqrt((x_i^2 + (x_i+1)^2)/2))/5

			total += _E - (20 / temp) - temp2;                // e - e ^ (sqrt((x_i^2 + (x_i+1)^2)/2))/5 - e ^ ((cos(2pi * x_i) + cos(2pi * x_i+1))/2)
		} // end for

		return total;
	} // end method ackleysTwoFunction


	inline double eggHolderFunction(const std::vector<double>* vect)
	{
		double total = 0,
			product = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double temp = 0,
				temp2 = 0;

			temp = vect->at(i) - vect->at(i + 1) - 47; // (x_i - x_(i+1) - 47)
			temp = abs(temp);                          // |(x_i - x_(i+1) - 47)|

			temp = sqrt(temp);                         // sqrt(|(x_i - x_(i+1) - 47)|)
			temp = sin(temp);                          // sin( sqrt(|(x_i - x_(i+1) - 47)|))
			temp *= vect->at(i);                       // sin( sqrt(|(x_i - x_(i+1) - 47)|)) * x_i

			temp2 = (vect->at(i) / 2) + vect->at(i + 1) + 47; // (x_i/2) + x_(i+1) + 47
			temp2 = abs(temp2);                               // |(x_i/2) + x_(i+1) + 47|

			temp2 = sqrt(temp2);                              // sqrt( |(x_i/2) + x_(i+1) + 47| )
			temp2 = sin(temp2);                               // sin( sqrt( |(x_i/2) + x_(i+1) + 47| ))
			temp2 *= (vect->at(i + 1) + 47);                    // sin( sqrt( |(x_i/2) + x_(i+1) + 47| )) * (x_(i+1) + 47)

			total += temp + temp2;							  // (sin( sqrt(|(x_i - x_(i+1) - 47)|)) * x_i) + (sin( sqrt( |(x_i/2) + x_(i+1) + 47| )) * (x_(i+1) + 47))
		} // end for

		total *= -1; // - SUM

		return total;
	} // end method eggHolderFunction


	inline double ranaFunction(const std::vector<double>* vect)
	{
		double	total = static_cast<double>(20 * (vect->size() - 1)),
			product = 0.0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double	sum = 0,
				sumPlusXi = 0,
				sumMinXi = 0,
				temp = 0,
				temp2 = 0;

			sum = vect->at(i + 1) + 1;     // x_(i+1) + 1

			sumPlusXi = sum + vect->at(i); //  (x_(i+1) + 1) + x_i
			sumPlusXi = abs(sumPlusXi);    // |(x_(i+1) + 1) + x_i|

			sumMinXi = sum - vect->at(i);  //  (x_(i+1) + 1) - x_i
			sumMinXi = abs(sumMinXi);      // |(x_(i+1) + 1) - x_i|

			temp = vect->at(i) * sin(sumMinXi) * cos(sumPlusXi);          // x_i * sin(|(x_(i+1) + 1) - x_i|) * cos(|(x_(i+1) + 1) + x_i|)
			temp2 = (vect->at(i + 1) + 1) * cos(sumMinXi) * sin(sumPlusXi); // (x_(i+1) + 1) * cos(|(x_(i+1) + 1) - x_i|) * sin(|(x_(i+1) + 1) + x_i|)

			total += temp + temp2; // [ x_i * sin(|(x_(i+1) + 1) - x_i|) * cos(|(x_(i+1) + 1) + x_i|)] + [ (x_(i+1) + 1) * cos(|(x_(i+1) + 1) - x_i|) * sin(|(x_(i+1) + 1) + x_i|)]
		} // end for

		return total;
	} // end method ranaFunction


	inline double pathologicalFunction(const std::vector<double>* vect)
	{
		double total = 0.5 * (vect->size() - 1);

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double	temp = 0,
				temp2 = 0;

			// 100(x_i)^2 + x_(i+1)^2
			temp = 100 * vect->at(i) * vect->at(i) + vect->at(i + 1) * vect->at(i + 1);
			temp = sqrt(temp); // sqrt( 100(x_i)^2 + x_(i+1)^2)

			temp = sin(temp);  // sin(sqrt( 100(x_i)^2 + x_(i+1)^2))
			temp *= temp;      // sin^2(sqrt( 100(x_i)^2 + x_(i+1)^2))

			temp -= 0.5;       // sin(sqrt( 100(x_i)^2 + x_(i+1)^2))^2 - 0.5

							   // (x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2
			temp2 = vect->at(i) * vect->at(i) - 2 * vect->at(i) * vect->at(i + 1) + vect->at(i + 1) * vect->at(i + 1);
			temp2 = temp2 * temp2; // ((x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2)^2
			temp2 *= 0.001;        // 0.001((x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2)^2
			temp2 += 1;            // 0.001((x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2)^2 + 1

			total += temp / temp2; // [ sin^2(sqrt( 100(x_i)^2 + x_(i+1)^2))^2 - 0.5 ] / [ 0.001((x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2)^2 + 1 ]
		} // end for

		return total;
	} // end method pathologicalFunction


	inline double michalewiczFunction(const std::vector<double>* vect)
	{
		double total = 0;

#pragma loop(hint_parallel(0))
		// SUM[1->n]
		for (std::size_t i = 0; i < vect->size(); i++)
		{
			double	temp = 0,
				temp2 = 0;

			temp = sin(vect->at(i));                   // sin(x_i)
			temp2 = (i + 1) * vect->at(i) * vect->at(i); // i * (x_i)^2
			temp2 /= _PI;                             // (i * (x_i)^2) / pi

			temp2 = sin(temp2);                        // sin((i * (x_i)^2) / pi)
			temp2 = pow(temp2, 20);                    // sin^20((i * (x_i)^2) / pi)

			total += temp * temp2;                     // sin(x_i) * sin((i * (x_i)^2) / pi)^20
		} // end for

		total *= -1; // - SUM

		return total;
	} // end method michalewiczFunction


	inline double masterCosineWaveFunction(const std::vector<double>* vect)
	{
		double total = 0;

#pragma loop(hint_parallel(0))
		// SUM[1->n-1]
		for (std::size_t i = 0; i < vect->size() - 1; i++)
		{
			double	temp = 0,
				temp2 = 0;

			// x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2
			temp = (vect->at(i) * vect->at(i)) + (0.5 * vect->at(i) * vect->at(i + 1)) + (vect->at(i + 1) * vect->at(i + 1));

			temp2 = temp;

			temp *= -0.125;                  // (-1/8)(x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2)

			temp2 = 4 * sqrt(temp2);         // 4( x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2 )

			total += exp(temp) * cos(temp2); // e^((-1/8)(x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2)) * cos( 4( x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2 ))
		} // end for

		total *= -1; // - SUM

		return total;
	} // end method masterCosineWaveFunction


	inline double shekelsFoxholesFunction(const std::vector<double>* vect, const double** da_A, const std::size_t ui_M = 10)
	{
		const double da_C[] = { 0.806,0.517,0.1,0.908,0.965,0.669,0.524,0.902,0.351,0.876,0.462,
			0.491,0.463,0.741,0.352,0.869,0.813,0.811,0.0828,0.964,0.789,0.360,0.369,
			0.992,0.332,0.817,0.632,0.883,0.608,0.326 };

		double total = 0;

#pragma loop(hint_parallel(0))
		// SUM[1->m]
		for (std::size_t i = 0; i < ui_M; i++)
		{
			double temp = 0;

			// SUM[1->n]
			for (std::size_t j = 0; j < vect->size(); j++)
			{
				double	temp2 = 0;

				temp2 = vect->at(j) - da_A[i][j]; // x_j - a_i,j
				temp += temp2 * temp2;			  // (x_j - a_i,j)^2
			} // end for

			temp += da_C[i];   // SUM[1->n](x_j - a_i,j)^2 + C_i

			total += 1 / temp; // 1 / ( SUM[1->n](x_j - a_i,j)^2 + C_i )
		} // end for

		total *= -1;

		return total;
	} // end method shekelsFoxholesFunction


	const std::vector<costFunction>& getAllCostFunctions(const double ** arg1, const std::size_t arg2);

} // end namespace cost_functions


using namespace cost_functions;

const std::vector<costFunction>& cost_functions::getAllCostFunctions(const double ** arg1, const std::size_t arg2)
{
	static std::vector<costFunction> functions
	{
		schwefelsFunction ,
		firstDeJongsFunction ,
		rosenbrockFunction ,
		rastriginFunction ,
		griewangkFunction ,
		sineEnvelopeSineWaveFunction ,
		stretchedVSineWaveFunction ,
		ackleysOneFunction ,
		ackleysTwoFunction ,
		eggHolderFunction ,
		ranaFunction ,
		pathologicalFunction ,
		michalewiczFunction ,
		masterCosineWaveFunction ,
		std::bind(shekelsFoxholesFunction, std::placeholders::_1, arg1, arg2), // arguments are bound to make the signature the same for all functions
	};

	return functions;
} // end method getAllCostFunctions


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

	std::vector<cost_functions::costFunction> costFunctions;

	std::vector<std::string> fileNames;

	void storeResults(std::string&, std::vector<results>&);
	void makeMatrix(double**&);
	void makeRanges(double**&);

	timePoint	compute_start,
		compute_end;
	duration	time_to_compute;

};

using namespace std;


Test::Test(void)
{
	da_ranges = new double*[NUMBER_FUNCTIONS]; // array containing the ranges for the RNG; [i][0] contains min, [i][1] contains max for function f_i

	makeRanges(da_ranges); // make matrix with ranges for functions


	da_A = new double*[SHEKEL_OUTER_SIZE]; // shekels foxhole parameter that is bound to the function

	makeMatrix(da_A); // make matrix A for shekels foxhole


	costFunctions = cost_functions::getAllCostFunctions(const_cast<const double**>(da_A), ui_SHEKEL_ITERATIONS); // vector containing the cost functions

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

		for (int i = 0; i < ((ui_length > 10) ? (costFunctions.size() - 1) : costFunctions.size()); i++)
		{
			res.push_back(results());
		} // end for

							 // run each cost function with current dimension
		for (int i = 0; i < costFunctions.size(); i++)
		{
			if (i == cost_functions::i_SHEKEL_INDEX && ui_length > 10) // only execute shekel with 10 dimensions
			{
				continue;
			} // end if

			// pointer to current results
			results * temp = &(res.at(i));

			// run each function n times
			for (size_t j = 0; j < ui_ITERATIONS; j++)
			{
				compute_start = highRes_Clock::now();

				results temp2 = f(costFunctions[i], ui_length, ui_ITERATIONS, da_ranges[i][0], da_ranges[i][1]);
				compute_end = highRes_Clock::now();

				// update best value only when new best is found
				if (temp->d_bestValue > temp2.d_bestValue)
				{
					temp->d_bestValue = temp2.d_bestValue;
					temp->bestValues = std::move(temp2.bestValues);
				} // end if

				time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

				// statistics
				temp->d_range += getRange(&(temp2.data));
				temp->d_stdDev += getStandardDeviation(&(temp2.data), temp2.d_avgValue);
				temp->d_median += getMedian(&(temp2.data));
				temp->d_avgValue += temp2.d_bestValue;
				temp->d_avgTime += time_to_compute.count();
			} // end for j

			  // calculate averages
			temp->d_avgValue /= ui_ITERATIONS;
			temp->d_range /= ui_ITERATIONS;
			temp->d_stdDev /= ui_ITERATIONS;
			temp->d_avgTime /= ui_ITERATIONS;
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
	da_A[0] = new double[SHEKEL_INNER_SIZE] { 9.681, 0.667, 4.783, 9.095, 3.517, 9.325, 6.544, 0.211, 5.122, 2.02 };
	da_A[1] = new double[SHEKEL_INNER_SIZE] { 9.4, 2.041, 3.788, 7.931, 2.882, 2.672, 3.568, 1.284, 7.033, 7.374 };
	da_A[2] = new double[SHEKEL_INNER_SIZE] { 8.025, 9.152, 5.114, 7.621, 4.564, 4.711, 2.996, 6.126, 0.734, 4.982 };
	da_A[3] = new double[SHEKEL_INNER_SIZE] { 2.196, 0.415, 5.649, 6.979, 9.510, 9.166, 6.304, 6.054, 9.377, 1.426 };
	da_A[4] = new double[SHEKEL_INNER_SIZE] { 8.074, 8.777, 3.467, 1.863, 6.708, 6.349, 4.534, 0.276, 7.633, 1.567 };
	da_A[5] = new double[SHEKEL_INNER_SIZE] { 7.650, 5.658, 0.720, 2.764, 3.278, 5.283, 7.474, 6.274, 1.409, 8.208 };
	da_A[6] = new double[SHEKEL_INNER_SIZE] { 1.256, 3.605, 8.623, 6.905, 4.584, 8.133, 6.071, 6.888, 4.187, 5.448 };
	da_A[7] = new double[SHEKEL_INNER_SIZE] { 8.314, 2.261, 4.24, 1.781, 4.124, 0.932, 8.129, 8.658, 1.208, 5.762 };
	da_A[8] = new double[SHEKEL_INNER_SIZE] { 0.226, 8.858, 1.42, 0.954, 1.622, 4.698, 6.228, 9.096, 0.972, 7.637 };
	da_A[9] = new double[SHEKEL_INNER_SIZE] { 7.305, 2.228, 1.242, 5.928, 9.133, 1.826, 4.06, 5.204, 8.713, 8.247 };
	da_A[10] = new double[SHEKEL_INNER_SIZE] { 0.652, 7.027, 0.508, 4.876, 8.807, 4.632, 5.808, 6.937, 3.291, 7.016 };
	da_A[11] = new double[SHEKEL_INNER_SIZE] { 2.699, 3.516, 5.847, 4.119, 4.461, 7.496, 8.817, 0.69, 6.593, 9.789 };
	da_A[12] = new double[SHEKEL_INNER_SIZE] { 8.327, 3.897, 2.017, 9.57, 9.825, 1.15, 1.395, 3.885, 6.354, 0.109 };
	da_A[13] = new double[SHEKEL_INNER_SIZE] { 2.132, 7.006, 7.136, 2.641, 1.882, 5.943, 7.273, 7.691, 2.88, 0.564 };
	da_A[14] = new double[SHEKEL_INNER_SIZE] { 4.707, 5.579, 4.08, 0.581, 9.698, 8.542, 8.077, 8.515, 9.231, 4.67 };
	da_A[15] = new double[SHEKEL_INNER_SIZE] { 8.304, 7.559, 8.567, 0.322, 7.128, 8.392, 1.472, 8.524, 2.277, 7.826 };
	da_A[16] = new double[SHEKEL_INNER_SIZE] { 8.632, 4.409, 4.832, 5.768, 7.05, 6.715, 1.711, 4.323, 4.405, 4.591 };
	da_A[17] = new double[SHEKEL_INNER_SIZE] { 4.887, 9.112, 0.17, 8.967, 9.693, 9.867, 7.508, 7.77, 8.382, 6.74 };
	da_A[18] = new double[SHEKEL_INNER_SIZE] { 2.44, 6.686, 4.299, 1.007, 7.008, 1.427, 9.398, 8.48, 9.95, 1.675 };
	da_A[19] = new double[SHEKEL_INNER_SIZE] { 6.306, 8.583, 6.084, 1.138, 4.350, 3.134, 7.853, 6.061, 7.457, 2.258 };
	da_A[20] = new double[SHEKEL_INNER_SIZE] { 0.652, 2.343, 1.37, 0.821, 1.31, 1.063, 0.689, 8.819, 8.833, 9.07 };
	da_A[21] = new double[SHEKEL_INNER_SIZE] { 5.558, 1.272, 5.756, 9.857, 2.279, 2.764, 1.284, 1.677, 1.244, 1.234 };
	da_A[22] = new double[SHEKEL_INNER_SIZE] { 3.352, 7.549, 9.817, 9.437, 8.687, 4.167, 2.57, 6.54, 0.228, 0.027 };
	da_A[23] = new double[SHEKEL_INNER_SIZE] { 8.798, 0.88, 2.37, 0.168, 1.701, 3.68, 1.231, 2.39, 2.499, 0.064 };
	da_A[24] = new double[SHEKEL_INNER_SIZE] { 1.46, 8.057, 1.337, 7.217, 7.914, 3.615, 9.981, 9.198, 5.292, 1.224 };
	da_A[25] = new double[SHEKEL_INNER_SIZE] { 0.432, 8.645, 8.774, 0.249, 8.081, 7.461, 4.416, 0.652, 4.002, 4.644 };
	da_A[26] = new double[SHEKEL_INNER_SIZE] { 0.679, 2.8, 5.523, 3.049, 2.968, 7.225, 6.73, 4.199, 9.614, 9.229 };
	da_A[27] = new double[SHEKEL_INNER_SIZE] { 4.263, 1.074, 7.286, 5.599, 8.291, 5.2, 9.214, 8.272, 4.398, 4.506 };
	da_A[28] = new double[SHEKEL_INNER_SIZE] { 9.496, 4.83, 3.15, 8.27, 5.079, 1.231, 5.731, 9.494, 1.883, 9.732 };
	da_A[29] = new double[SHEKEL_INNER_SIZE] { 4.138, 2.562, 2.532, 9.661, 5.611, 5.5, 6.886, 2.341, 9.699, 6.5 };
} // end method makeMatrix


inline void Test::makeRanges(double**& ranges)
{
	ranges[0] = new double[2]{ -512, 512 };
	ranges[1] = new double[2]{ -100, 100 };
	ranges[2] = new double[2]{ -100, 100 };
	ranges[3] = new double[2]{ -30,  30 };
	ranges[4] = new double[2]{ -500, 500 };
	ranges[5] = new double[2]{ -30,  30 };
	ranges[6] = new double[2]{ -30,  30 };
	ranges[7] = new double[2]{ -32,  32 };
	ranges[8] = new double[2]{ -32,  32 };
	ranges[9] = new double[2]{ -500, 500 };
	ranges[10] = new double[2]{ -500, 500 };
	ranges[11] = new double[2]{ -100, 100 };
	ranges[12] = new double[2]{ 0, _PI };
	ranges[13] = new double[2]{ -30,  30 };
	ranges[14] = new double[2]{ 0,  10 };
} // end method makeRanges


std::ostream& operator<<(std::ostream& stream, results& res)
{
	stream << "Optimum found: " << res.d_bestValue << ",\n";

	stream << "Optimal point: ";

	for (size_t i = 0; i < res.bestValues.size(); i++)
	{
		stream << ", " << res.bestValues.at(i);
	} // end for

	stream << ",\n";

	stream << "Mean:, " << res.d_avgValue << ",\n";
	stream << "Median:, " << res.d_median << ",\n";
	stream << "Time:, " << res.d_avgTime << ",\n";
	stream << "Median:, " << res.d_range << ",\n";
	stream << "SD:, " << res.d_stdDev << ",\n\n";

	return stream;
} // end operator << 


typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function < results(cost_functions::costFunction, const std::size_t, const std::size_t, double, double)> randWlk;

results randomWalk(cost_functions::costFunction f, const std::size_t ui_SIZE, const uint ui_ITERATIONS, double d_min, double d_max)
{
	double	d_best = std::numeric_limits<double>::max(),
		d_result = 0,
		d_avgValue = 0;

	std::vector<double>* bestArgs = nullptr;
	std::vector<double>* data = new std::vector<double>();

	for (std::size_t i = 0; i < ui_ITERATIONS; i++)
	{
		std::vector<double>* args = getRandomVector(ui_SIZE, &d_min, &d_max);

		d_result = f(args);

		d_avgValue += d_result;

		data->push_back(d_result);

		if (d_result < d_best)
		{
			if (bestArgs != nullptr)
			{
				delete bestArgs;
			} // end if

			bestArgs = args;

			d_best = d_result;

			// shrink search space whenever we find a better solution
			d_min = minValueInVector(args);
			d_max = maxValueInVector(args);
		} // end if
		else
		{
			delete args;
		} // end else
	} // end for

	d_avgValue /= ui_ITERATIONS;

	// statistical results
	results* res = new results();
	res->bestValues = std::move(*bestArgs);
	res->d_avgValue = d_avgValue;
	res->d_bestValue = d_best;
	res->data = std::move(*data);

	return *res;
} // end method randomWalk




using namespace std;

void presentMenu(void)
{
	cout << "1 - Random Walk" << endl;
	cout << "2 - Local Search" << endl;
	cout << "3 - Iterative Local Search" << endl;
	cout << "4 - All" << endl;
	cout << "Q - Quit" << endl;
} // end method presentMenu


char getChoice(void)
{
	char c = 0x0;
	bool first = true;

	do
	{
		if (!first)
		{
			cout << "Invalid input! Try again." << endl;
		} // end if
		first = false;

		presentMenu();

		cout << "Enter choice: ";
		cin >> c;

		if (cin.fail()) // error check
		{
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			continue;
		} // end if

		c = static_cast<char>(tolower(static_cast<char>(c)));

	} while (c != '1' && c != '2' && c != '3' && c != '4' && c != 'q');

	return c;
} // end method getChoice


int main(int argc, char ** argv)
{
	const size_t ui_ITERATIONS = 100;


	char choice = 'q';

	Test *test = new Test();

	timePoint	compute_start = highRes_Clock::now(),
		compute_end = highRes_Clock::now();
	duration	time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

	do
	{
		choice = getChoice();

		switch (choice)
		{
		case '1':
			cout << "Starting tests for Random Walk ..." << endl;

			compute_start = highRes_Clock::now();

			test->runTest<randWlk>(randomWalk, ui_ITERATIONS);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;
			break;

		case '2':
			break;

		case '3':
			break;

		case '4':
			cout << "Starting tests for Random Walk ..." << endl;
			compute_start = highRes_Clock::now();

			//test->runTest(randomWalk, ui_ITERATIONS);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;
			break;

		default:
			break;
		} // end switch
	} while (tolower(choice) != 'q');


	//delete test;


	return EXIT_SUCCESS;
} // end Main