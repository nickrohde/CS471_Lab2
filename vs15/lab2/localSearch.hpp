#pragma once

#ifndef _LOCAL_SEARCH_H
#define _LOCAL_SEARCH_H
#include "results.hpp"
#include "utility.hpp"
#include <functional>

typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function <results_t* (costFunction, const std::size_t, double, double, double)> lclSrch;


results_t* localSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_DELTA_X);


#endif