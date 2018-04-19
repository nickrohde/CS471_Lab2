#pragma once

#ifndef _UTILITY_H
#define _UTILITY_H
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>


#ifndef _PI
#define _PI 3.141592653589793238462643383279
#endif // !_PI

#ifndef _E
#define _E  2.718281828459045235360287471352
#endif // !_E

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif // !EXIT_SUCCESS

#ifndef NULL
#define NULL 0
#endif

typedef unsigned int uint;

typedef std::chrono::high_resolution_clock::time_point	timePoint;
typedef std::chrono::duration<double>					duration;
typedef std::chrono::high_resolution_clock				highRes_Clock;


template <typename T>
inline T getRandomNumberInRange(const T* p_MIN, const T* p_MAX)
{
	static std::random_device rd{};
	static std::mt19937 engine{ rd() };
	std::uniform_real_distribution<T> dist{ *p_MIN, *p_MAX };

	return dist(engine);
} // end template getRandomNumberInRange


template <typename T>
std::vector<T>* getRandomVector(const std::size_t ui_SIZE, const T* p_MIN, const T* p_MAX)
{
	std::vector<T>* vec = new std::vector<T>(ui_SIZE);

	for (std::size_t i = 0; i < ui_SIZE; i++)
	{
		vec->at(i) = (getRandomNumberInRange(p_MIN, p_MAX));
	} // end for

	return vec;
} // end template getRandomVector


template <typename T>
double getStandardDeviation(const std::vector<T>* p_data, const double d_mean)
{
	double d_standardDeviation = 0.0;

#pragma loop(hint_parallel(0))
	for (size_t i = 0; i < p_data->size(); i++)
	{
		d_standardDeviation += pow(p_data->at(i) - d_mean, 2);
	} // end for

	return sqrt(d_standardDeviation / static_cast<double>(p_data->size()));
} // end template getStandardDeviation


template <typename T>
inline double getRange(const std::vector<T>* p_data)
{
	T	result,
		min = p_data->at(0),
		max = p_data->at(0);


	for (size_t i = 0; i < p_data->size(); i++)
	{
		if (p_data->at(i) < min)
		{
			min = p_data->at(i);
		} // end if
		if (p_data->at(i) > max)
		{
			max = p_data->at(i);
		} // end if
	} // end if

	result = max - min;

	return result;
} // end template getRange


template <typename T>
inline T getMedian(std::vector<T>* p_data)
{
	std::sort(p_data->begin(), p_data->end());

	if (p_data->size() % 2)
	{
		return p_data->at(p_data->size() / 2);
	} // end if
	else
	{
		T temp = p_data->at(p_data->size() / 2);
		temp += p_data->at((p_data->size() / 2) + 1);
		return (temp / 2);
	} // end else
} // end template getMedian


template <typename T>
inline T maxValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(p_vect->size() - 1);
} // end template maxValueInVector


template <typename T>
inline T minValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(0);
} // end template minValueInVector


template <typename T>
inline T convertStringToType(const std::string s)
{
	T t;
	std::stringstream ss;

	ss << s;
	ss >> t;

	return t;
} // end template convertStringToType


template <typename T>
inline T getDistance(T a, T b)
{
	return sqrt(pow((b - a), 2));
} // end template getDistance

#endif