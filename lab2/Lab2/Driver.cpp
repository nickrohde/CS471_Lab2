#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>
#include <functional>
#include "randomWalk.hpp"
#include "localSearch.hpp"
#include "iterativeLocalSearch.hpp"
#include "utility.hpp"
#include "Driver.hpp"
#include "IniParser.hpp"


using namespace std;


void Driver::presentMenu(void)
{
	cout << "1 - Random Walk" << endl;
	cout << "2 - Local Search" << endl;
	cout << "3 - Iterative Local Search" << endl;
	cout << "Q - Quit" << endl;
} // end method presentMenu


char Driver::getChoice(void)
{
	char c = 0x0;
	bool first = true;

	do
	{
		if (!first)
		{
			cout << "Invalid input! Try again." << endl;
		} // end if
		first = false;

		presentMenu();

		cout << "Enter choice: ";
		cin >> c;

		if (cin.fail()) // error check
		{
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			continue;
		} // end if

		c = static_cast<char>(tolower(static_cast<char>(c)));

	} while (c != '1' && c != '2' && c != '3' && c != 'q');

	return c;
} // end method getChoice


char Driver::askUserYesNo(void)
{
	char c = 0x0;
	bool first = true;

	do
	{
		if (!first)
		{
			cout << "Invalid input! Try again." << endl;
		} // end if
		first = false;

		cout << "Enter choice (Y/N): ";
		cin >> c;

		if (cin.fail()) // error check
		{
			cin.clear();
			cin.ignore(std::cin.rdbuf()->in_avail());
			continue;
		} // end if

		c = static_cast<char>(tolower(static_cast<char>(c)));

	} while (c != 'y' && c != 'n');

	return c;
} // end method askUserYesNo


void Driver::initialize(const std::string  s_fileName)
{
	IniParser* parser = new IniParser(s_fileName);

	try
	{
		// extract data from .ini file
		ui_startDim = convertStringToType<size_t>((*parser)("DIM", "min"));   // smallest dimension to test
		ui_endDim = convertStringToType<size_t>((*parser)("DIM", "max"));   // largest dimension to test
		ui_dimDelta = convertStringToType<size_t>((*parser)("DIM", "delta")); // dimension increase

		ui_iterations = convertStringToType<size_t>((*parser)("TEST", "num_test_itrs"));
		b_storeData = convertStringToType<bool>((*parser)("TEST", "store_data"));
		ui_numFunctions = convertStringToType<size_t>((*parser)("FUNCTIONS", "total"));
		ui_numILSItr = convertStringToType<size_t>((*parser)("FUNCTIONS", "num_ILS_itrs"));
	
		if (ui_numFunctions <= 0 || ui_iterations <= 0)
		{
			delete parser;
			throw invalid_argument("Number of functions and number of iterations must be positive integer!");
		} // end if

		for (size_t i = 1; i <= ui_numFunctions; i++)
		{
			stringstream ss;
			ss << "f" << i;

			double temp = convertStringToType<double>((*parser)("LS_DELTA", ss.str()));

			if (temp == 0.0)
			{
				delete parser;
				throw invalid_argument("Delta values for local search must be non-zero floats!");
			} // end if

			LS_deltaX.push_back(temp);
		} // end for
	} // end try
	catch (invalid_argument e)
	{
		delete parser;
		throw e; // handled above
	} // end catch

	b_invalid = false;

	delete parser;
} // end method initialize


bool Driver::isValid(void)
{
	return !b_invalid;
} // end method isValid


Driver::Driver(void)
{
	// initialize variables
	compute_start = highRes_Clock::now(),
	compute_end = highRes_Clock::now();
	time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

	b_invalid = true;

	test = nullptr; // this object is created by run
} // end Default Constructor


Driver::Driver(string s_fileName) : Driver()
{
	// attempt to parse .ini file
	ifstream file(s_fileName.c_str(), ios::in);

	if (file.is_open() && !file.bad())
	{
		file.close();

		for (;;)
		{
			try
			{
				initialize(s_fileName);
				break;
			} // end try
			catch (invalid_argument e)
			{
				cout << "A required field was not found the file!" << endl;
				cout << e.what() << endl << endl;
				cout << "Try different file?" << endl;
				clearInput();
				char c = askUserYesNo();

				if (c == 'n')
				{
					return; // the object is in an invalid state
				} // end if

				s_fileName = askUserForFileName();

				if (b_stop)
				{
					return; // the object is in an invalid state
				} // end if
			} // end catch
		} // end forever
	} // end if
	else
	{
		file.close();

		for (;;)
		{
			cout << "The file \"" << s_fileName << "\" could not be opened." << endl;
			cout << "Try different file?" << endl;
			clearInput();
			char c = askUserYesNo();

			if (c == 'n')
			{
				return; // the object is in an invalid state
			} // end if

			s_fileName = askUserForFileName();

			if (b_stop)
			{
				return; // the object is in an invalid state
			} // end if

			try
			{
				initialize(s_fileName);
				break;
			} // end try
			catch (invalid_argument e)
			{
				cout << "An error occurred while parsing the ini file!" << endl;
				cout << "Error: " << e.what();
				cout << "Try different file?" << endl;
				char c = askUserYesNo();

				if (c == 'n')
				{
					return; // the object is in an invalid state
				} // end if
			} // end catch
		} // end forever
	} // end else

	b_invalid = false;
} // end Constructor 1


int Driver::run(void)
{
	char choice = 'q';

	if (b_invalid)
	{
		cout << "Parsing of the .ini file failed. Exiting ..." << endl;
		return EXIT_FAILURE; // ini file parsing was unsuccessful
	} // end if

	test = new Test(&LS_deltaX, ui_startDim, ui_endDim, ui_dimDelta, b_storeData);

	do
	{
		clearInput();
		choice = getChoice(); // get user input for which algorithm to run

		switch (choice)
		{
			case '1':
			{
				cout << "Starting tests for Random Walk ..." << endl;

				compute_start = highRes_Clock::now(); // start timer for whole run

				test->runTest<randWlk>(randomWalk, ui_iterations, b_storeData, ui_iterations);

				compute_end = highRes_Clock::now();
				time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

				cout << "Finished running tests for Random Walk." << endl;
				cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

				break;
			} // end case 1

			case '2':
			{	
				cout << "Starting tests for Local Search ..." << endl;

				compute_start = highRes_Clock::now(); // start timer for whole run

				test->runTest<lclSrch>(localSearch, b_storeData, ui_iterations);

				compute_end = highRes_Clock::now();
				time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

				cout << "Finished running tests for Local Search." << endl;
				cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

				break;
			} // end case 2

			case '3':
			{
				cout << "Starting tests for Iterative Local Search ..." << endl;
				cout << "This may take upwards of 10 minutes." << endl;

				compute_start = highRes_Clock::now(); // start timer for whole run

				test->runTest<itrLclSrch>(iterativeLocalSearch, ui_iterations, b_storeData, ui_numILSItr);

				compute_end = highRes_Clock::now();
				time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

				cout << "Finished running tests for Iterative Local Search." << endl;
				cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

				break;
			} // end case 3

			default:
				break;
		} // end switch
	} while (tolower(choice) != 'q');

	delete test;

	return EXIT_SUCCESS;
} // end method run


string Driver::askUserForFileName(void)
{
	// Variables:
	string s_name = "";

	b_stop = false;

	for(;;)
	{
		cout << "Please enter the name of the .ini file: ";
		clearInput();
		getline(cin, s_name);

		if (s_name.length() > 0)
		{
				ifstream file(s_name, ios::in);

				if (!file.bad() && file.is_open())
				{
					file.close();
					return s_name;
				} // end if 
				else
				{
					file.close();
					cout << "The file \"" << s_name << "\" could not be opened." << endl;
					cout << "Try different file?" << endl;
					clearInput();
					char c = askUserYesNo();

					if (c == 'n')
					{
						b_stop = true;
						return "";
					} // end if
				} // end else
		} // end if (s_name.length() > 0)
		else
		{
			cout << "Invalid input!" << endl;
			cout << "Try again?" << endl;
			clearInput();
			char c = askUserYesNo();

			if (c == 'n')
			{
				cout << "Exiting ..." << endl;
				b_stop = true;
				return "";
			} // end if
		} // end else
	} // end forever

	return s_name;
} // end method askUserForFileName


void Driver::clearInput()
{
	cin.clear();
	cin.ignore(std::cin.rdbuf()->in_avail());
} // end method clearInput