#pragma once

#ifndef _LOCAL_SEARCH_H
#define _LOCAL_SEARCH_H
#include "results.hpp"
#include "utility.hpp"
#include <functional>

#define PRECISION 0.001

/// <summary>Typedef to make cost function type a bit more readable.</summary>
typedef std::function<double(const std::vector<double>*)> costFunction;

/// <summary>Typedef to make LS function type a bit more readable.</summary>
typedef std::function <results_t* (costFunction, const std::size_t, double, double, double, bool)> lclSrch;

/// <summary>Local search optimization strategy.</summary>
/// <param name="f">The cost function to run LS on.</param>
/// <param name="ui_SIZE">Number of dimensions.</param>
/// <param name="d_min">Minimum value of the range.</param>
/// <param name="d_max">Maximum value of the range.</param>
/// <param name="d_deltaX">Delta X to use during search.</param>
/// <param name="b_storeData">Whether or not to store all data.</param>
/// <returns>A pointer to a results_t object containing test results.</returns>
results_t* localSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_DELTA_X, bool b_storeData);


#endif