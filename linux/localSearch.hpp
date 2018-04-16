#pragma once

#ifndef _LOCAL_SEARCH_H
#define _LOCAL_SEARCH_H
#include "Test.hpp"

typedef std::function<double(const std::vector<double>*)> costFunction;

results* localSearch(costFunction f, const std::size_t ui_SIZE, const std::size_t ui_ITERATIONS, double d_min, double d_max, std::vector<double>* x_0);

void getNeighbour(std::vector<double>* vect);


#endif