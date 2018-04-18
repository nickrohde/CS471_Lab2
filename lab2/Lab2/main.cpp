#include "driver.hpp"
#include <exception>


using namespace std;

/*
 * Program entry point. 
 * Input: Takes 1 command line parameter, the name of the .ini file to retrieve config info from.
 * Output: 0 on successful execution of the program, 1 otherwise.
 */
int main(int argc, char ** argv)
{
	Driver* driver = nullptr;

	if (argc > 1) // ini file parameter is present
	{
		driver = new Driver(argv[1]);
	} // end if
	else // no ini file parameter
	{
		driver = new Driver();
	} // end else
	
	try
	{
		driver->run();
	} // end try
	catch (exception e)
	{
		delete driver;

		return EXIT_FAILURE;
	} // end catch

	delete driver;

	return EXIT_SUCCESS;
} // end Main