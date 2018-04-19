#pragma once

#ifndef _ILS_H
#define _ILS_H
#include "results.hpp"
#include "utility.hpp"
#include <functional>


typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function <results_t* (costFunction, const std::size_t, double, double, double, std::size_t)> itrLclSrch;


results_t* iterativeLocalSearch(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double d_deltaX, std::size_t ui_iterations);





#endif // !_ILS_H

