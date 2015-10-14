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

int main(int argc, char** argv)
{
	if(argc != 5)
	{
		cout << "usage: popcrun <obj.map> ./main_example localhost INOUT.json 160.98.X.X IN.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		cout << "Creating POPSensor for temperature" << popcendl;
		POPSensor popLocal(argv[1], argv[2],0);
		POPSensorData localData;

		cout << "Creating POPSensor for leds" << popcendl;
		POPSensor popRemote(argv[3], argv[4],1);
		POPSensorData remoteData;

		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_ALL_OFF); // MSR_LED=notify for led, all off

		cout << "Broadcast green LED ON" << popcendl;
		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // MSR_LED=notify for led, green toggle

		char c = '\n';
		while(true)
		{
			cout << "\nPress key to continue\n" << popcendl;
			//while(c == '\n')
			c = getchar();
			break;
		}

		cout << "Gather temperatures..." << popcendl;
		for(int i = 0 ; i < 3000 ; i++)
		{
			popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // PUB_LED=publish for led, green toggle
			localData = popLocal.Gather();
			remoteData = popRemote.Gather();
			popLocal.Clear();
			popRemote.Clear();
			//printf("\n-----------------------------------------------------------------------------------------------\n");
			//remoteData.Print();
			//printf("\n-----------------------------------------------------------------------------------------------\n");
			printf("\n");
			localData.Print();
			printf("\n");
			printf("\n");
			remoteData.Print();
			printf("\n");
			//printf("\n-----------------------------------------------------------------------------------------------\n");
			sleep(3);
		}
		cout << "Finished gathering temperatures" << popcendl;

		cout << "Broadcast green LED OFF" << popcendl;
		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // PUB_LED=publish for led, green toggle

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

