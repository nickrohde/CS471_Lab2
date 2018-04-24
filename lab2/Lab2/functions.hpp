#pragma once

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "utility.hpp"
#include <functional>


/// <summary>Typedef to make cost function type a bit more readable.</summary>
typedef std::function<double(const std::vector<double>*)> costFunction;

/// <summary>Index of shekel's foxholes function in the vector returned by getAllCostFunctions</summary>
const int i_SHEKEL_INDEX = 14;

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double schwefelsFunction				(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double firstDeJongsFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double rosenbrockFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double rastriginFunction				(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double griewangkFunction				(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double sineEnvelopeSineWaveFunction	(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double stretchedVSineWaveFunction	(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double ackleysOneFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double ackleysTwoFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double eggHolderFunction				(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double ranaFunction					(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double pathologicalFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double michalewiczFunction			(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double masterCosineWaveFunction		(const std::vector<double>* vect);

/// <summary>Cost function for testing an optimization strategy</summary>
/// <param name="vect">The point at which the function is to be evaluated.</param>
/// <param name="da_A">A matrix for the outer loop of this function.</param>
/// <param name="ui_M">The number of times to execute the outer loop of the function.</param>
/// <returns>The value of the function evaluated at <paramref name="vect"/></returns>
inline double shekelsFoxholesFunction(const std::vector<double>* vect, const double** da_A, const std::size_t ui_M = 10);

/// <summary>Gets a vector containg all 15 cost functions.</summary>
/// <param name="da_A">A matrix for the outer loop of shekel's foxholes function.</param>
/// <param name="ui_M">The number of times to execute the outer loop of shekel's foxholes function is to be run.</param>
/// <returns>A constant vector containing pointers to all 15 functions above.</returns>
const std::vector<costFunction>& getAllCostFunctions(const double ** arg1, const std::size_t arg2);


#endif