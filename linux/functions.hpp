#pragma once

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <cmath>
#include "utility.hpp"
#include <functional>
#include <cstdarg>



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
/*{
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
} // end method getAllCostFunctions*/

#endif