#include <iostream>
#include <cctype>
#include <chrono>
#include <functional>
#include <vector>
#include "randomWalk.hpp"
#include "Test.hpp"


using namespace std;

void presentMenu(void)
{
	cout << "1 - Random Walk" << endl;
	cout << "2 - Local Search" << endl;
	cout << "3 - Iterative Local Search" << endl;
	cout << "4 - All" << endl;
	cout << "Q - Quit" << endl;
} // end method presentMenu


char getChoice(void)
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

	} while (c != '1' && c != '2' && c != '3' && c != '4' && c != 'q');

	return c;
} // end method getChoice


int main(int argc, char ** argv)
{
	const size_t ui_ITERATIONS = 100;


	char choice = 'q';

	Test *test = new Test();

	timePoint	compute_start = highRes_Clock::now(),
		compute_end = highRes_Clock::now();
	duration	time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

	do
	{
		choice = getChoice();

		switch (choice)
		{
		case '1':
			cout << "Starting tests for Random Walk ..." << endl;

			compute_start = highRes_Clock::now();

			test->runTest<randWlk>(randomWalk, ui_ITERATIONS);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;
			break;

		case '2':
			break;

		case '3':
			break;

		case '4':
			cout << "Starting tests for Random Walk ..." << endl;
			compute_start = highRes_Clock::now();

			//test->runTest(randomWalk, ui_ITERATIONS);

			compute_end = highRes_Clock::now();
			time_to_compute = std::chrono::duration_cast<duration>(compute_end - compute_start);

			cout << "Finished running tests for Random Walk." << endl;
			cout << "Time elapsed: " << time_to_compute.count() << " seconds." << endl << endl;
			break;

		default:
			break;
		} // end switch
	} while (tolower(choice) != 'q');


	//delete test;


	return EXIT_SUCCESS;
} // end Main

