/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   Dec 2014
 * @brief  Main executable of the POPWIN project
 *
 *
 */
#include "POPSensor.ph"

using namespace std;

/// Blink all leds 3 times
void blinkLeds(POPSensor& xr_gateway)
{
	static const int usecs = 100 * 1000;
	for(int led = 0 ; led < 3 ; led++)
	{
		for(int i = 0 ; i < 2*3 ; i++)
		{
			char message[64];
			sprintf(message, "{\"function\":4,\"led\":%d}\n", led);
			xr_gateway.SendData(message);
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
		char message[64];
		sprintf(message, "{\"function\":3}\n");
		xr_gateway.SendData(message);
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

		//blinkLeds(gateway);

		sleep(2);

		cout<<"Ask for the available functions"<<popcendl;
		gateway.SendData("{\"function\":0}\n");

		// for(int i = 0 ; i < 5 ; i++)
		// {
			// gateway.SendData("{\"function\":3,\"led\":1}\n");
		// }
		sleep(2);

		cout<<"Ask sensor readings"<<popcendl;

		askSensorReadings(gateway);


		cout<<"Stop listening"<<popcendl;
		gateway.StopListening();

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
