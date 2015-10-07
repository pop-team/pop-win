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
#include <iostream>

#include "POPSensor.ph"

using namespace std;

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		cout << "usage: popcrun <obj.map> ./main_test_led 160.98.X.X led.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		cout << "Creating POPSensor for leds" << popcendl;
		POPSensor popLed(argv[1], argv[2],0);
		POPSensorData temps;

		//popLed.Broadcast(PUB_LED, 3); // PUB_LED=publish for led, 3:all off
		popLed.Broadcast(MSR_LED, UNT_NONE, LED_ALL_OFF); // MSR_LED=notify for led, 1:green toggle

		cout << "Broadcast green LED ON" << popcendl;
		//popLed.Broadcast(PUB_LED, 1); // PUB_LED=publish for led, 1:green toggle
		popLed.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // MSR_LED=notify for led, 1:green toggle

		char c = '\n';
		while(true)
		{
			cout << "\nPress key to continue\n" << popcendl;
			//while(c == '\n')
			c = getchar();
			break;
		}

		cout << "Blink leds..." << popcendl;
		int count = 0;
		const int MAX_BLINKS = 400;
		for(int i = 0 ; i < MAX_BLINKS ; i++)
		{
			//popLed.Broadcast(PUB_LED, 1); // PUB_LED=publish for led, 1:green toggle
			/*popLed.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // MSR_LED=notify for led, 1:green toggle
			if(count % 2 == 0)
			{
				cout << "--------------- Led OFF --------------- " << popcendl;
			}
			else
			{
				cout << "--------------- Led ON --------------- " << popcendl;
			}
			count++;*/
			//double sum = popLed.Reduce(MSR_TEMPERATURE, TYPE_DOUBLE, POPSensorData::size);
			temps = popLed.Gather();
			popLed.Clear();
			//printf("\n------------------------------------------------------\n");
			//printf("sum: %lf\n",sum);
			temps.Print();
			//printf("\n------------------------------------------------------\n");
			sleep(1);
		}
		cout << "Finished blinking leds" << popcendl;

		printf("end of main\n");
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in popwin main loop: " << e.what() << popcendl;
		exit(0);
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << popcendl;
		exit(0);
		return 1;
	}


	return 0;
}

