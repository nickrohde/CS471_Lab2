#include "driver.hpp"
#include <exception>


using namespace std;

char askUserYesNo(void)
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

/**
 * Program entry point. 
 * Input: Takes 1 command line parameter, the name of the .ini file to retrieve config info from.
 * Output: 0 on successful execution of the program, 1 otherwise.
 */
int main(int argc, char ** argv)
{
	Driver* driver = nullptr;
	int i_exitCode = 1;

	if (argc > 1) // ini file parameter is present
	{
		string s(argv[1]);
		driver = new Driver(s);
	} // end if
	else // no ini file parameter
	{
		char c;

		cout << "No ini file provided!\nUse default settings?";
		c = askUserYesNo();

		if (c == 'n')
		{
			string s("none");
			driver = new Driver(s);
		} // end if
		else
		{
			cout << "Using default settings." << endl;
			driver = new Driver();
		} // end else (c == 'y')
	} // end else (argc <= 1)
	
	try
	{
		i_exitCode = driver->run();
	} // end try
	catch (exception e)
	{
		delete driver;

		return EXIT_FAILURE;
	} // end catch

	delete driver;

	return i_exitCode;
} // end Main

