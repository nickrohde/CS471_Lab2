#pragma once

#ifndef _RAND_WALK_H
#define _RAND_WALK_H
#include "results.hpp"
#include "utility.hpp"
#include <functional>
#include <limits>

typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function <results_t* (costFunction, const std::size_t, double, double, double, std::size_t)> randWlk;

results_t* randomWalk(costFunction f, const std::size_t ui_SIZE, double d_min, double d_max, double unused, std::size_t ui_ITERATIONS);

#endif // !_RAND_WALK_H