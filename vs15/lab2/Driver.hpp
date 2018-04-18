#pragma once

#ifndef _DRIVER_H
#define _DRIVER_H


#include <string>
#include "Test.hpp"

typedef std::chrono::high_resolution_clock::time_point timePoint;
typedef std::chrono::duration<double> duration;
typedef std::chrono::high_resolution_clock highRes_Clock;


class Driver
{
public:

	Driver(void);
	Driver(const std::string& s_fileName);

	~Driver(void);

	int run(void);

private:
	size_t ui_iterations;

	double d_LS_deltaX;

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;

	Test* test;

	void presentMenu(void);
	char getChoice(void);

	std::string& askUserForFileName(void);
	char askUserYesNo(void);

	void initialize(std::string& s_fileName);

}; // end class Driver


#endif // !_DRIVER_H