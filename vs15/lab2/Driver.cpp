#include <iostream>
#include <cctype>
#include <fstream>
#include <functional>
#include "randomWalk.hpp"
#include "localSearch.hpp"
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


void Driver::initialize(std::string & s_fileName)
{


} // end method initialize


Driver::Driver(void)
{
	d_LS_deltaX = 0.21;
	ui_iterations = 100;
}


Driver::Driver(const string& s_fileName)
{
	compute_start   = highRes_Clock::now(),
	compute_end     = highRes_Clock::now();
	time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

	test = nullptr;

	ifstream file(s_fileName.c_str(), ios::in);


	if (file.is_open() && !file.bad())
	{

	} // end if
	else
	{

	} // end else

	file.close();
} // end Constructor


Driver::~Driver(void)
{


} // end Destructor


int Driver::run(void)
{
	char choice = 'q';


	do
	{
		choice = getChoice();

		switch (choice)
		{
		case '1':
			cout << "Starting tests for Random Walk ..." << endl;

			test = new Test();

			compute_start = highRes_Clock::now();

			test->runTest<randWlk>(randomWalk, ui_iterations, ui_iterations);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

			delete test;

			break;

		case '2':
			cout << "Starting tests for Local Search ..." << endl;

			test = new Test();

			compute_start = highRes_Clock::now();

			test->runTest<lclSrch>(localSearch, ui_iterations, d_LS_deltaX);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;

			delete test;
			break;

		case '3':
			break;

		default:
			break;
		} // end switch
	} while (tolower(choice) != 'q');


	return true;
} // end method run


string& Driver::askUserForFileName(void)
{
	// Variables:
	string s_name = "";
	string s_check = "";

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
							exit(EXIT_FAILURE);
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
						exit(EXIT_FAILURE);
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
			exit(EXIT_FAILURE);
		} // end if
	} // end forever

	return s_name;
} // end method askUserForFileName