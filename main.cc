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
#include "lineComm/popwin_messages.h"

using namespace std;

// Get an input key from user
char getch()
{
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror ("tcsetattr ~ICANON");
	return (buf);
}

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
	cout << "input command number (0 to get the list of commands)>>" << popcendl;
	int num = getchar() - '0';
	cout << "send command " << num << popcendl;
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
	cout << "q: Quit" <<popcendl;

	cout << "a: Ask the sensor to read temperature" <<popcendl;
	commands['a'] = askSensorReadings;

	cout << "b: Make leds blink on sensor" <<popcendl;
	commands['b'] = blinkLeds;

	cout << "c: Send a custom command to sensor" <<popcendl;
	commands['c'] = customCommand;

	cout << "g: Generate test data" <<popcendl;
	commands['g'] = generateTestData;

	cout << "e: Clear stored data" <<popcendl;
	commands['e'] = clearData;

	cout << "p: Print stored data" <<popcendl;
	commands['p'] = printData;


	try
	{
		POPSensor gateway("localhost");
		gateway.Connect();
		gateway.StartListening();

		//cout<<"Ask to send the list of commands"<<popcendl;
		//gateway.Publish(PUB_COMMAND, 0);

		char c = '0';
		while(c != 'q')
		{
			cout << ">>" << popcendl;
			c = getchar();
			auto cmd = commands.find(c);
			if(cmd != commands.end())
			{
				(*cmd->second)(gateway);
			}
			else
			{
				cout << "No command for " << c << popcendl;
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
