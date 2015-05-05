/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   May 2015
 * @brief  Main executable of the POPWIN project
 *
 *
 */

// #include <unistd.h>
// #include <termios.h>
// #include <map>
#include <fstream>

#include "POPSensor.ph"

using namespace std;

// -------------------------------------------------------------------------------- //
//                                COMMANDS
// -------------------------------------------------------------------------------- //


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
	if(argc != 1)
	{
		cout << "usage: popcrun <obj.map> ./main_example" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		POPSensor popSensor("localhost", "resources.json");
		popSensor.StartListening();

		// For testing: generate test data
		generateTestData(popSensor);


		cout<<"Stop listening"<<popcendl;
		popSensor.StopListening();


		// Print double data
		/*
		auto gatheredDataDouble(xr_gateway.RetrieveDataDouble());
		cout << "\nRecords found on proxy <double>: "<< gatheredDataDouble.size() << popcendl;
		for(auto elem : gatheredDataDouble)
		{
			cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
		}

		// Print int data
		auto gatheredDataInt(xr_gateway.RetrieveDataInt());
		cout << "\nRecords found on proxy <int>: "<< gatheredDataInt.size() << popcendl;
		for(auto elem : gatheredDataInt)
		{
			cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
		}

		// Print string data
		auto gatheredDataString(xr_gateway.RetrieveDataString());
		cout << "\nRecords found on proxy <string>: "<< gatheredDataString.size() << popcendl;
		for(auto elem : gatheredDataString)
		{
			cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
		}
		*/

		cout<<"Clear data"<<popcendl;
		popSensor.ClearData();


		cout<<"End of popwin main"<<popcendl;
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in popwin main loop: " << e.what() << popcendl;
		return 1;
	}
	catch(std::exception *e)
	{
		cerr<<"Exception caught in popwin main loop (*excep): " << e->what() << popcendl;
		return 1;
	}
	catch(int e)
	{
		cerr<<"Exception caught in popwin main loop (int): " << e << popcendl;
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << popcendl;
		return 1;
	}


	return 0;
}
