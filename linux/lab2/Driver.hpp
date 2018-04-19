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
	Driver(std::string& s_fileName);

	~Driver(void);

	int run(void);

private:
	size_t	ui_iterations,
			ui_numILSItr,
			ui_numFunctions,
			ui_startDim,
			ui_endDim,
			ui_dimDelta;

	bool	b_storeData,
			b_invalid,
			b_stop;	

	timePoint	compute_start,
				compute_end;
	duration	time_to_compute;

	Test* test;

	std::vector<double> LS_deltaX;

	void presentMenu(void);
	char getChoice(void);

	std::string& askUserForFileName(void);
	char askUserYesNo(void);

	void initialize(const std::string& s_fileName);

}; // end class Driver


#endif // !_DRIVER_H