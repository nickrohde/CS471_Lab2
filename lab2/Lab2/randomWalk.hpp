#pragma once

#ifndef _RAND_WALK_H
#define _RAND_WALK_H
#include "Test.hpp"
#include <limits>

typedef std::function<double(const std::vector<double>*)> costFunction;
typedef std::function <results* (costFunction, const std::size_t, const std::size_t, double, double)> randWlk;

results* randomWalk(costFunction f, const std::size_t ui_SIZE, const std::size_t ui_ITERATIONS, double d_min, double d_max);

#endif // !_RAND_WALK_H