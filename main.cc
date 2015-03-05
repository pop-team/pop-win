/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   Dec 2014
 * @brief  Main executable of the POPWIN project
 *
 *
 */

// TODO LW: Check that exceptions are correctly handled. E.g. sensor disconnected
#include "POPSensor.ph"
#include "lineComm/popwin_messages.h"

using namespace std;

/// Blink all leds 3 times
void blinkLeds(POPSensor& xr_gateway)
{
	static const int usecs = 500 * 1000;
	for(int led = 0 ; led < 3 ; led++)
	{
		for(int i = 0 ; i < 2*3 ; i++)
		{
			xr_gateway.Publish(PUB_LED, led);
			usleep(usecs);
		}
	}
}

/// Ask temperature and acceleration readings 
void askSensorReadings(POPSensor& xr_gateway)
{
	// Note: for the moment, only temperature is available
	static const int usecs = 100 * 1000;
	for(int i = 0 ; i < 10 ; i++)
	{
		xr_gateway.Publish(PUB_COMMAND, 1);
		usleep(usecs);
	}
}

int main(int argc, char** argv)
{
	try
	{
		POPSensor gateway("localhost");
		gateway.Connect();
		gateway.StartListening();

		cout<<"Ask to send the list of commands"<<popcendl;
		gateway.Publish(PUB_COMMAND, 0);

		cout<<"Blink leds on device"<<popcendl;
		blinkLeds(gateway);

		sleep(2);

		cout<<"Ask sensor readings"<<popcendl;
		askSensorReadings(gateway);

		sleep(2);

		cout<<"Generate test data"<<popcendl;
		for(int i = 0 ; i < 10 ; i++)
		{
			// Each command generates 10 sanples of data (types: double, int, string)
			gateway.Publish(PUB_COMMAND, 2);
			gateway.Publish(PUB_COMMAND, 3);
			gateway.Publish(PUB_COMMAND, 4);

			// Wait a bit to avoid overloading the mote
			usleep(0.3 * 1000000);
		}

		sleep(2);

		cout<<"Stop listening"<<popcendl;
		gateway.StopListening();

		sleep(2);

		cout<<"Print the data gathered"<< popcendl;
		gateway.RetrieveDataDouble();
		gateway.RetrieveDataInt();
		gateway.RetrieveDataString();

		cout<<"Clear the data gathered"<< popcendl;
		gateway.ClearData();

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
