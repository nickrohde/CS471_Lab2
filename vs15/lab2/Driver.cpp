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


void Driver::initialize(const std::string & s_fileName)
{
	IniParser* parser = new IniParser(s_fileName);

	// extract data from .ini file
	ui_startDim		= convertStringToType<size_t>((*parser)("DIM", "min"));   // smallest dimension to test
	ui_endDim		= convertStringToType<size_t>((*parser)("DIM", "max"));   // largest dimension to test
	ui_dimDelta		= convertStringToType<size_t>((*parser)("DIM", "delta")); // dimension increase

	if (ui_startDim > ui_endDim || ui_startDim < 2 || ui_startDim > 50 || ui_endDim < 2 || ui_endDim > 75)
	{
		delete parser;
		throw new invalid_argument("Start dimension must be bigger than end dimension; valid [DIM] range start: [2,50] - end: [2,75]");
	} // end if

	ui_iterations	= convertStringToType<size_t>((*parser)("TEST", "num_test_itrs"));
	b_storeData		= convertStringToType<bool>((*parser)("TEST", "store_data"));
	ui_numFunctions = convertStringToType<size_t>((*parser)("FUNCTIONS", "total"));
	ui_numILSItr	= convertStringToType<size_t>((*parser)("FUNCTIONS", "num_ILS_itrs"));

	if (ui_numFunctions <= 0 || ui_iterations <= 0)
	{
		delete parser;
		throw new invalid_argument("Number of functions and number of iterations must be positive integer!");
	} // end if

	for (size_t i = 1; i <= ui_numFunctions; i++)
	{
		stringstream ss;
		ss << "f" << i;

		double temp = convertStringToType<double>((*parser)("LS_DELTA", ss.str()));

		if (temp == 0.0)
		{
			delete parser;
			throw new invalid_argument("Delta values for local search must be non-zero floats!");
		} // end if

		LS_deltaX.push_back(temp);
	} // end for

	b_invalid = false;

	delete parser;
} // end method initialize


Driver::Driver(void)
{
	ui_iterations   = 30;
	ui_numILSItr    =  5;
	ui_numFunctions = 15;
	ui_startDim     = 10;
	ui_endDim       = 30;
	ui_dimDelta     = 10;
		
	b_storeData     = true;
	b_invalid       = false;
	b_stop			= false;

	compute_start = compute_end = highRes_Clock::now();

	LS_deltaX = { 4.0,4.0,4.0,4.0,4.0,4.0,4.0,4.0,4.0,4.0,4.0,4.0,0.03,5.01,0.73 };
} // end Default Constructor


Driver::Driver(string& s_fileName)
{
	compute_start   = highRes_Clock::now(),
	compute_end     = highRes_Clock::now();
	time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

	b_invalid = true;

	test = nullptr;

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
				cout << "An error occurred while passing the ini file!" << endl;
				cout << "Error: " << e.what();
				s_fileName = askUserForFileName();

				if (b_stop)
				{
					break;
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

			s_fileName = askUserForFileName();
			if (b_stop)
			{
				break;
			} // end if

			try
			{
				initialize(s_fileName);
				break;
			} // end try
			catch (invalid_argument e)
			{
				cout << "An error occurred while passing the ini file!" << endl;
				cout << "Error: " << e.what();
			} // end catch
		} // end forever
	} // end else

	b_invalid = false;
} // end Constructor 1


Driver::~Driver(void)
{


} // end Destructor


int Driver::run(void)
{
	char choice = 'q';

	if (b_invalid)
	{
		return EXIT_FAILURE; // ini file parsing was unsuccessful
	} // end if

	test = new Test(&LS_deltaX, ui_startDim, ui_endDim, ui_dimDelta, b_storeData);

	do
	{
		choice = getChoice();

		switch (choice)
		{
		case '1':
			cout << "Starting tests for Random Walk ..." << endl;			

			compute_start = highRes_Clock::now();

			test->runTest<randWlk>(randomWalk, ui_iterations, ui_iterations);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;		

			break;

		case '2':
			cout << "Starting tests for Local Search ..." << endl;			

			compute_start = highRes_Clock::now();

			test->runTest<lclSrch>(localSearch, ui_iterations);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Local Search." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

			break;

		case '3':
			cout << "Starting tests for Iterative Local Search ..." << endl;

			compute_start = highRes_Clock::now();

			test->runTest<itrLclSrch>(iterativeLocalSearch, ui_iterations, ui_numILSItr);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Iterative Local Search." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

			break;

		default:
			break;
		} // end switch
	} while (tolower(choice) != 'q');

	delete test;

	return EXIT_SUCCESS;
} // end method run


string& Driver::askUserForFileName(void)
{
	// Variables:
	string s_name = "";
	string s_check = "";

	b_stop = false;

	for(;;)
	{
		cout << "Please enter a file name: ";
		getline(cin, s_name);

		if (s_name.length() > 4)
		{
			s_check = s_name.substr(s_name.length() - 4, s_name.length() - 1);

			if (s_check == ".ini")
			{
				ifstream file(s_name, ios::in);

				if (!file.bad() && file.is_open())
				{
					// check if file contains anything
					file.seekg(0, ios::end);
					size_t size = file.tellg();

					if (size > 0)
					{
						break;
					} // end if (size > 0)
					else
					{
						cout << "The file \"" << s_name << "\" does not contain anything." << endl;
						cout << "Try again? ";
						char c = askUserYesNo();

						if (c == 'n')
						{
							cout << "Exiting ..." << endl;
							b_stop = true;
							return s_check;
						} // end if
					} // end if

					file.close();
				} // end if (!file.bad() && file.is_open())
				else
				{
					cout << "The file \"" << s_name << "\" could not be opened." << endl;
					cout << "Try again? ";
					char c = askUserYesNo();

					if (c == 'n')
					{
						cout << "Exiting ..." << endl;
						b_stop = true;
						return s_check;
					} // end if
				} // end else
			} // end if (s_check == ".ini")
		} // end if (s_name.length() > 4)

		cout << "Invalid input!" << endl;
		cout << "Try again? ";
		char c = askUserYesNo();

		if (c == 'n')
		{
			cout << "Exiting ..." << endl;
			b_stop = true;
			return s_check;
		} // end if
	} // end forever

	return s_name;
} // end method askUserForFileName