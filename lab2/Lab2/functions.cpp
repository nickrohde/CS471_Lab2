#include "functions.hpp"
#include <cmath>
#include <cstdarg>


inline double schwefelsFunction(const std::vector<double>* vect)
{
	double total = 0.0;

	// SUM[1->n]
	for (auto& d : *vect)
	{
		total += (-1 * d) * sin(sqrt(abs(d)));
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
		double	temp = 100 * pow((vect->at(i) * vect->at(i) - vect->at(i + 1)),2); // 100(x_i^2 - x_i+1)^2

		total += temp + pow((1 - vect->at(i)), 2); // 100(x_i^2 - x_i+1)^2 + (1-x_i)^2
	} // end for

	return total;
} // end method rosenbrockFunction


inline double rastriginFunction(const std::vector<double>* vect)
{
	double total = 0.0;

#pragma loop(hint_parallel(0))
	// SUM[1->n]
	for (std::size_t i = 0; i < vect->size(); i++)
	{
		total += ((pow(vect->at(i), 2)) - (10 * (cos(2 * _PI * vect->at(i)))));	// x_i^2 - 10cos(2pi * x_i)
	} // end for

	total += (10 * vect->size()) - (200 * vect->size()); // 10*n + SUM -- shift down by -200n to move optimal point from 0 to -200n

	return total;
} // end method rastriginFunction                                     


inline double griewangkFunction(const std::vector<double>* vect)
{
	double	total = 0.0,
			sum = 0.0,		// SUM[1->n]
			product = 1.0;	// PROD[1->n]

#pragma loop(hint_parallel(0))
	for (std::size_t i = 0; i < vect->size(); i++)
	{
		// SUM[1->n]
		{
			sum += ((vect->at(i) * vect->at(i))/ 4000);
		} // end SUM[1->n]

		// PROD[1->n]
		{
			product *= cos((vect->at(i) / sqrt(static_cast<double>(i + 1))));
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
		double	sumOfSquares = 0;

		sumOfSquares = (vect->at(i) * vect->at(i)) + (vect->at(i + 1) * vect->at(i + 1)); // x_i^2 + (x_i+1)^2

		total += ((pow(sin(sumOfSquares - 0.5), 2)) / (pow(((sumOfSquares * 0.001) + 1), 2))); // sin( x_i^2 + (x_i+1)^2 - 0.5 )^2 / (0.001(x_i^2 + (x_i+1)^2) + 1)^2
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
		double	sumOfSquares = 0.0,
				temp = 0.0;

		sumOfSquares = (vect->at(i) * vect->at(i)) + (vect->at(i + 1) * vect->at(i + 1)); // x_i^2 + (x_i+1)^2

		temp = pow(sin(50.0 * pow(sumOfSquares, 0.1)),2) + 1; // sin^2( 50( root_10( x_i^2 + (x_i+1)^2 ))) + 1

		total += temp * pow(sumOfSquares, 0.25); // (sin^2( 50( root_10( x_i^2 + (x_i+1)^2 ))) + 1) * root_4(x_i^2 + (x_i+1)^2)
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
		double	sumOfSquares = 0,
				temp = 0;

		sumOfSquares = oneOverE * sqrt((vect->at(i) * vect->at(i)) + (vect->at(i + 1) * vect->at(i + 1))); // 1/(e^0.2) * sqrt(x_i ^ 2 + (x_i + 1) ^ 2)

		temp = 3 * (cos(2 * vect->at(i)) + sin(2 * vect->at(i + 1))); // 3(cos(2 * x_i) + sin(2 * x_i+1))

		total += temp + sumOfSquares;      // 1/(e^0.2) * sqrt(x_i ^ 2 + (x_i + 1) ^ 2) + 3(cos(2 * x_i) + sin(2 * x_i+1))
	} // end for

	return total;
} // end method ackleysOneFunction


inline double ackleysTwoFunction(const std::vector<double>* vect)
{
	double total = 20.0 * (vect->size() - 1.0); // 20(n-1) + SUM

#pragma loop(hint_parallel(0))
	// SUM[1->n-1]
	for (std::size_t i = 0; i < vect->size() - 1; i++)
	{
		double	sumOfSquares = 0.0,
				temp		 = 0.0;

		temp = 0.5 * (cos(2 * _PI * vect->at(i)) + cos(2 * _PI * vect->at(i + 1))); // (cos(2pi * x_i) + cos(2pi * x_i+1))/2

		sumOfSquares = 0.2 * sqrt(((vect->at(i) * vect->at(i)) + (vect->at(i + 1) * vect->at(i + 1))) * 0.5); // (sqrt((x_i^2 + (x_i+1)^2)/2))/5

		total += _E - (20 / exp(sumOfSquares)) - exp(temp);                // e - e ^ (sqrt((x_i^2 + (x_i+1)^2)/2))/5 - e ^ ((cos(2pi * x_i) + cos(2pi * x_i+1))/2)
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
		double	temp = 0,
				temp2 = 0;

		temp = vect->at(i) * sin(sqrt(abs(vect->at(i) - vect->at(i + 1) - 47)));					// sin( sqrt(|(x_i - x_(i+1) - 47)|)) * x_i

		temp2 = (vect->at(i + 1) + 47) * sin(sqrt(abs((vect->at(i) / 2) + vect->at(i + 1) + 47)));	// sin( sqrt( |(x_i/2) + x_(i+1) + 47| )) * (x_(i+1) + 47)

		total += temp + temp2;							  // (sin( sqrt(|(x_i - x_(i+1) - 47)|)) * x_i) + (sin( sqrt( |(x_i/2) + x_(i+1) + 47| )) * (x_(i+1) + 47))
	} // end for

	total *= -1; // - SUM

	return total;
} // end method eggHolderFunction


inline double ranaFunction(const std::vector<double>* vect)
{
	double	total = static_cast<double>(20 * (vect->size() - 1));

#pragma loop(hint_parallel(0))
	// SUM[1->n-1]
	for (std::size_t i = 0; i < vect->size() - 1; i++)
	{
		double	sum = 0,
				sumPlusXi = 0,
				sumMinXi = 0,
				temp = 0,
				temp2 = 0;

		sum = vect->at(i + 1) + 1;			// x_(i+1) + 1

		sumPlusXi = abs(sum + vect->at(i)); // |(x_(i+1) + 1) + x_i|

		sumMinXi = abs(sum - vect->at(i));  // |(x_(i+1) + 1) - x_i|

		temp = vect->at(i) * sin(sumMinXi) * cos(sumPlusXi);			// x_i * sin(|(x_(i+1) + 1) - x_i|) * cos(|(x_(i+1) + 1) + x_i|)
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
		double	temp = pow(sin(sqrt(100 * vect->at(i) * vect->at(i) + vect->at(i + 1) * vect->at(i + 1))), 2) - 0.5, // sin^2(sqrt( 100(x_i)^2 + x_(i+1)^2)) - 0.5
				temp2 = (0.001 * pow((vect->at(i) * vect->at(i) - 2 * vect->at(i) * vect->at(i + 1) + vect->at(i + 1) * vect->at(i + 1)), 2)) + 1; // 0.001((x_i)^2 - 2(x_i) * x_(i+1) + (x_(i+1))^2)^2 + 1

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
		double	temp = 0;

		temp = pow(sin(((i + 1) * vect->at(i) * vect->at(i)) / _PI), 20);	// sin^20((i * (x_i)^2) / pi)

		total += sin(vect->at(i)) * temp;									// sin(x_i) * sin((i * (x_i)^2) / pi)^20
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
		temp2 = temp = (vect->at(i) * vect->at(i)) + (0.5 * vect->at(i) * vect->at(i + 1)) + (vect->at(i + 1) * vect->at(i + 1));

		temp *= -0.125;                  // (-1/8)(x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2)

		temp2 = 4 * sqrt(temp2);         // 4( x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2 )

		total += exp(temp) * cos(temp2); // e^((-1/8)(x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2)) * cos( 4( x_i^2 + 0.5(x_i)(x_i+1) + (x_i+1)^2 ))
	} // end for

	total *= -1; // - SUM

	return total;
} // end method masterCosineWaveFunction


inline double shekelsFoxholesFunction(const std::vector<double>* vect, const double** da_A, const std::size_t ui_M)
{
	const double da_C[] = { 0.806,0.517,0.1,0.908,0.965,0.669,0.524,0.902,0.351,0.876,0.462,
							0.491,0.463,0.741,0.352,0.869,0.813,0.811,0.0828,0.964,0.789,
							0.360,0.369,0.992,0.332,0.817,0.632,0.883,0.608,0.326 };

	double total = 0;

#pragma loop(hint_parallel(0))
	// SUM[1->m]
	for (std::size_t i = 0; i < ui_M; i++)
	{
		double temp = 0;

		// SUM[1->n]
		for (std::size_t j = 0; j < vect->size(); j++)
		{
			temp += pow((vect->at(j) - da_A[i][j]), 2); // (x_j - a_i,j)^2
		} // end for

		total += 1 / (temp + da_C[i]); // 1 / ( SUM[1->n](x_j - a_i,j)^2 + C_i )
	} // end for

	total *= -1;

	return total;
} // end method shekelsFoxholesFunction


const std::vector<costFunction>& getAllCostFunctions(const double ** arg1, const std::size_t arg2)
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
		std::bind(shekelsFoxholesFunction, std::placeholders::_1, arg1, arg2), 
	};

	return functions;
} // end method getAllCostFunctions

