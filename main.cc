/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   Dec 2014
 * @brief  Main executable of the POPWIN project
 *
 *
 */

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

/// Send a custom command
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
	for(int i = 0 ; i < 5 ; i++)
	{
		// Each command generates 10 sanples of data (types: double, int, string)
		xr_gateway.Publish(PUB_COMMAND, 2);
		usleep(0.3 * 1000000);
		xr_gateway.Publish(PUB_COMMAND, 3);
		usleep(0.3 * 1000000);
		xr_gateway.Publish(PUB_COMMAND, 4);

		// Wait a bit to avoid overloading the mote
		usleep(0.3 * 1000000);
	}
}

/// Test the different communication messages
void testCommunication(POPSensor& xr_gateway)
{
	cout<<"Send a test notification to sensors"<<popcendl;
	xr_gateway.Notify(MSR_LOG, UNT_NONE, "This is a test notification");
	usleep(0.3 * 1000000);
	cout<<"Send a test subsciption to sensors"<<popcendl;
	xr_gateway.Subscribe(MSR_VIBRATION, TYPE_DOUBLE);
	usleep(0.3 * 1000000);
	cout<<"Send a test publication to sensors (should send the command list)"<<popcendl;
	xr_gateway.Publish(PUB_COMMAND, 0);
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
	// Print double data
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

	cout << " t: Test communication" <<popcendl;
	commands['t'] = testCommunication;
	cout << "---------------------------------------------------------------------------------------" <<popcendl;
	cout << popcendl;


	try
	{
		POPSensor gateway("localhost");
		gateway.Connect("usb");
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

			try
			{
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
			catch(POPException &e)
			{
				cout<<"Cannot execute command. Received exception: " << e.what() << popcendl;
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
