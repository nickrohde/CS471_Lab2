#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "utility.hpp"
#include "functions.hpp"
#include "randomWalk.hpp"



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

		if (cin.fail())
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
	/*double 	min1 = -100,
			max1 = 100;
			https://stackoverflow.com/questions/14719911/how-do-we-pass-an-arbitrary-function-to-another-function
	randomWalk(firstDeJongsFunction, 30, 1000000, &min1, &max1);
	randomWalk(rosenbrockFunction, 30, 1000000, &min1, &max1);
	*/
	char choice = 'q';

	do
	{
		choice = getChoice();

		switch (choice)
		{
		case '1':
			break;

		case '2':
			break;

		case '3':
			break;

		case '4':
			break;

		default:
			break;
		} // end switch
	} while (tolower(choice) != 'q');




	return EXIT_SUCCESS;
} // end Main