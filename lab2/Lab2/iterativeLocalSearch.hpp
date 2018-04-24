#pragma once

#ifndef _ILS_H
#define _ILS_H
#include "results.hpp"
#include "utility.hpp"
#include <functional>

/// <summary>Typedef to make cost function type a bit more readable.</summary>
typedef std::function<double(const std::vector<double>*)> costFunction;

/// <summary>Typedef to make ILS function type a bit more readable.</summary>
typedef std::function <results_t* (costFunction, const std::size_t, double, double, double, bool, std::size_t)> itrLclSrch;

/// <summary>Iterative local search optimization strategy.</summary>
/// <param name="f">The cost function to run ILS on.</param>
/// <param name="ui_SIZE">Number of dimensions.</param>
/// <param name="d_min">Minimum value of the range.</param>
/// <param name="d_max">Maximum value of the range.</param>
/// <param name="d_deltaX">Delta X to use during search.</param>
/// <param name="b_storeData">Whether or not to store all data.</param>
/// <param name="ui_iterations">Number of iterations to run ILS for.</param>
/// <returns>A pointer to a results_t object containing test results.</returns>
results_t* iterativeLocalSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_deltaX, bool b_storeData, std::size_t ui_iterations);

#endif // !_ILS_H

