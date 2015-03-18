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
#include <unistd.h>
#include <termios.h>
#include <map>

#include "POPSensor.ph"

using namespace std;

// -------------------------------------------------------------------------------- //
//                                COMMANDS
// -------------------------------------------------------------------------------- //

typedef void (*Command)(POPSensor& xr_gateway);

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

/// Ask temperature and acceleration readings 
void customCommand(POPSensor& xr_gateway)
{
	// Note: for the moment, only temperature is available
	static const int usecs = 100 * 1000;
	cout << "Input command number (0 to get the list of commands):" << popcendl;
	int num = getchar() - '0';
	while(num == '\n' - '0')
		num = getchar() - '0';
	// cout << "send command " << num << popcendl;
	xr_gateway.Publish(PUB_COMMAND, num);
}


/// Ask the remote to generate test data
void generateTestData(POPSensor& xr_gateway)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		// Each command generates 10 sanples of data (types: double, int, string)
		xr_gateway.Publish(PUB_COMMAND, 2);
		xr_gateway.Publish(PUB_COMMAND, 3);
		xr_gateway.Publish(PUB_COMMAND, 4);

		// Wait a bit to avoid overloading the mote
		usleep(0.3 * 1000000);
	}
}

/// Ask the remote to generate test data
void clearData(POPSensor& xr_gateway)
{
	// Each command generates 10 sanples of data (types: double, int, string)
	xr_gateway.ClearData();
}

/// Ask the remote to generate test data
void printData(POPSensor& xr_gateway)
{
	xr_gateway.RetrieveDataDouble();
	xr_gateway.RetrieveDataInt();
	xr_gateway.RetrieveDataString();
}

int main(int argc, char** argv)
{
	// Input a list of commands
	map<char, Command> commands;
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: type a command to interact with your sensors                             " <<popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " q: Quit" <<popcendl;

	cout << " a: Ask the sensor to read temperature" <<popcendl;
	commands['a'] = askSensorReadings;

	cout << " b: Make leds blink on sensor" <<popcendl;
	commands['b'] = blinkLeds;

	cout << " c + <nb>: Send a custom command to sensor. 'c0' should list the available commands." <<popcendl;
	commands['c'] = customCommand;

	cout << " g: Generate test data" <<popcendl;
	commands['g'] = generateTestData;

	cout << " e: Clear stored data" <<popcendl;
	commands['e'] = clearData;

	cout << " p: Print stored data" <<popcendl;
	commands['p'] = printData;
	cout << "---------------------------------------------------------------------------------------" <<popcendl;
	cout << popcendl;


	try
	{
		POPSensor gateway("localhost");
		gateway.Connect();
		gateway.StartListening();

		//cout<<"Ask to send the list of commands"<<popcendl;
		//gateway.Publish(PUB_COMMAND, 0);

		char c = '\n';
		while(true)
		{
			cout << "Enter selection:" << popcendl;
			while(c == '\n')
				c = getchar();
			if(c == 'q')
				break;
			auto cmd = commands.find(c);
			if(cmd != commands.end())
			{
				(*cmd->second)(gateway);
				c = '\n';
			}
			else
			{
				cout << "No command for " << c << popcendl;
				c = '\n';
			}
		}

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
