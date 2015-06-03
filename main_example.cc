/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   May 2015
 * @brief  Example executable for the POPWIN project: The main routine shows the usage of the different C++ class of the POPWIN project.
 *
 *
 */

// #include <unistd.h>
// #include <termios.h>
// #include <map>
#include <fstream>

#include "POPSensor.ph"

using namespace std;

/// Ask the remote to generate test data
void generateTestData(POPSensor& xr_popSensor)
{
	for(int i = 0 ; i < 5 ; i++)
	{
		// Each command generates 10 sanples of data (types: double, int, string)
		xr_popSensor.Publish(PUB_COMMAND, 2);
		usleep(0.3 * 1000000);
		xr_popSensor.Publish(PUB_COMMAND, 3);
		usleep(0.3 * 1000000);
		xr_popSensor.Publish(PUB_COMMAND, 4);

		// Wait a bit to avoid overloading the mote
		usleep(0.3 * 1000000);
	}
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "usage: popcrun <obj.map> ./main_example resource.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		POPSensor popSensor("localhost", argv[1]);

		// For testing: generate test data
		generateTestData(popSensor);
		// TODO sleep

		cout<<"Print the gathered data"<<popcendl;
		POPSensorData data(popSensor.Gather());
		data.Print();

		cout<<"Clear data"<<popcendl;
		popSensor.Clear();


		cout<<"End of popwin main"<<popcendl;
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in popwin main loop: " << e.what() << popcendl;
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << popcendl;
		return 1;
	}


	return 0;
}
