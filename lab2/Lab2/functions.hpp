#pragma once

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <cmath>		
#include "utility.hpp"	// statistics/random functions
#include <functional>
#include <cstdarg>


// cost function type, for readability
typedef std::function<double(const std::vector<double>*)> costFunction;

// index of shekel's foxholes function in vector returned by getAllCostFunctions
const int i_SHEKEL_INDEX = 14;

// Cost functions:
	// Input: point in n-dimensional space
	// Output: the function evaluated at the given point
inline double schwefelsFunction				(const std::vector<double>* vect);
inline double firstDeJongsFunction			(const std::vector<double>* vect);
inline double rosenbrockFunction			(const std::vector<double>* vect);
inline double rastriginFunction				(const std::vector<double>* vect);
inline double griewangkFunction				(const std::vector<double>* vect);
inline double sineEnvelopeSineWaveFunction	(const std::vector<double>* vect);
inline double stretchedVSineWaveFunction	(const std::vector<double>* vect);
inline double ackleysOneFunction			(const std::vector<double>* vect);
inline double ackleysTwoFunction			(const std::vector<double>* vect);
inline double eggHolderFunction				(const std::vector<double>* vect);
inline double ranaFunction					(const std::vector<double>* vect);
inline double pathologicalFunction			(const std::vector<double>* vect);
inline double michalewiczFunction			(const std::vector<double>* vect);
inline double masterCosineWaveFunction		(const std::vector<double>* vect);

// Shekel's Foxholes:
	// Input: point in n-dimensional space | 'A' matrix | 'M' value
	// Output: shekel's foxholes evaluated at the given point, with the given A and M values
inline double shekelsFoxholesFunction(const std::vector<double>* vect, const double** da_A, const std::size_t ui_M = 10);


// getAllCostFunctions:
	// Input: 'A' matrix for shekel's foxholes function | 'M' value for shekel's foxholes function
	//        Inputs are bound to shekel's foxholes function to normalize function signatures
	// Output: A const reference to a static vector containing all cost functions
const std::vector<costFunction>& getAllCostFunctions(const double ** arg1, const std::size_t arg2);


#endif