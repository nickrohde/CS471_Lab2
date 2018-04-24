#pragma once

#ifndef _UTILITY_H
#define _UTILITY_H
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>


/// <summary>Pi</summary>
#ifndef _PI
#define _PI 3.141592653589793238462643383279
#endif // !_PI

/// <summary>E</summary>
#ifndef _E
#define _E  2.718281828459045235360287471352
#endif // !_E

/// <summary>Return values.</summary>
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif // !EXIT_SUCCESS

/// <summary>Null</summary>
#ifndef NULL
#define NULL 0
#endif

/// <summary>Typedefs for convenience.</summary>
typedef unsigned int uint;
typedef std::chrono::high_resolution_clock::time_point	timePoint;
typedef std::chrono::duration<double>					duration;
typedef std::chrono::high_resolution_clock				highRes_Clock;

/// <summary>Gets a random number in the given range using a mersenne twister.</summary>
/// <typeparam name="T">Some type that the mersenne twister can generate.</typeparam>
/// <param name="p_MIN">Pointer to the minimum value of the range.</param>
/// <param name="p_MAX">Pointer to the maximum value of the range.</param>
/// <returns>A random number between <paramref name="p_MIN"/> and <paramref name="p_MAX"/> (both inclusive).</returns>
template <typename T>
inline T getRandomNumberInRange(const T* p_MIN, const T* p_MAX)
{
	static std::random_device rd{};
	static std::mt19937 engine{ rd() };
	std::uniform_real_distribution<T> dist{ *p_MIN, *p_MAX };

	return dist(engine);
} // end template getRandomNumberInRange

/// <summary>Gets a vector of specified size with random values.</summary>
/// <typeparam name="T">Type that the mersenne twister can generate.</typeparam>
/// <param name="ui_SIZE">Size of the vector.</param>
/// <param name="p_MIN">Pointer to the minimum value of the range.</param>
/// <param name="p_MAX">Pointer to the maximum value of the range.</param>
/// <returns>A vector containing <paramref name="ui_SIZE"/> random numbers between <paramref name="p_MIN"/> and <paramref name="p_MAX"/> (both inclusive).</returns>
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

/// <summary>Calculates the standard deviation of the given set.</summary>
/// <typeparam name="T">Type that implements operator+, operator-, operator*, operator/, and double(T) (explicit cast).</typeparam>
/// <param name="p_data">Vector containing the set.</param>
/// <param name="d_mean">The mean of the set.</param>
/// <returns>The standard deviation of the given set as a double.</returns>
/// <exception cref="std::bad_cast">Thrown if T does not implement the explicit conversion operator double(T).</exception>
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

/// <summary>Calculates the range of the given set.</summary>
/// <typeparam name="T">Type that implements operator- and double(T) (explicit cast).</typeparam>
/// <param name="p_data">Vector containing the set.</param>
/// <returns>The range of the given set as a double.</returns>
/// <exception cref="std::bad_cast">Thrown if T does not implement the explicit conversion operator double(T).</exception>
template <typename T>
inline double getRange(const std::vector<T>* p_data)
{
	double	result;
	T		min = p_data->at(0),
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

	result = static_cast<double>(max - min);

	return result;
} // end template getRange

/// <summary>Calculates the median of the given set.</summary>
/// <typeparam name="T">Type that implements operator+ and operator/.</typeparam>
/// <param name="p_data">Vector containing the set.</param>
/// <returns>The median of the given set as a double.</returns>
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

/// <summary>Finds the maximum of the given set.</summary>
/// <typeparam name="T">Type that implements std::less.</typeparam>
/// <param name="p_vect">Vector containing the set.</param>
/// <returns>The maximum of the given set as a double.</returns>
template <typename T>
inline T maxValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(p_vect->size() - 1);
} // end template maxValueInVector

/// <summary>Finds the minimum of the given set.</summary>
/// <typeparam name="T">Type that implements std::less.</typeparam>
/// <param name="p_vect">Vector containing the set.</param>
/// <returns>The minimum of the given set as a double.</returns>
template <typename T>
inline T minValueInVector(std::vector<T>* p_vect)
{
	std::sort(p_vect->begin(), p_vect->end());

	return p_vect->at(0);
} // end template minValueInVector

/// <summary>Converts a string to type T.</summary>
/// <typeparam name="T">A type that can be extracted from a string using stringstream.</typeparam>
/// <param name="s">The string to convert.</param>
/// <returns>An equivalent T object to the string, if such an object exists, otherwise a default object of type T.</returns>
template <typename T>
inline T convertStringToType(const std::string s)
{
	T t;
	std::stringstream ss;

	ss << s;
	ss >> t;

	return t;
} // end template convertStringToType

/// <summary>Finds the distance between point A and point B.</summary>
  /// <typeparam name="T">A type that implements double(T) (explicit cast).</typeparam>
  /// <param name="a">The first point.</param>
  /// <param name="b">The second point.</param>
  /// <returns>The distance between points <paramref name="a"/> and <paramref name="b">.</returns>
template <typename T>
inline double getDistance(T a, T b)
{
	return sqrt(pow((b - a), 2));
} // end template getDistance

#endif