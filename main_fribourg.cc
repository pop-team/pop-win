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
	if(argc != 3)
	{
		cout << "usage: popcrun <obj.map> ./main_fribourg localhost fribourg.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		cout << "Creating POPSensor from fribourg.json" << popcendl;
		POPSensor popFribourg(argv[1], argv[2],0);
		POPSensorData fribourgData;

		popFribourg.Broadcast(MSR_LED, UNT_NONE, LED_ALL_OFF); // MSR_LED=notify for led, all off

		cout << "Broadcast green LED ON" << popcendl;
		popFribourg.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // MSR_LED=notify for led, green toggle

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
			fribourgData = popFribourg.Gather();
			popFribourg.Clear();
			printf("\n-------------------------------------------------------------------------\n");
			fribourgData.Print();
			printf("\n-------------------------------------------------------------------------\n");
			popFribourg.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // PUB_LED=publish for led, green toggle
			sleep(6);
		}
		cout << "Finished gathering temperatures" << popcendl;

		cout << "Broadcast green LED OFF" << popcendl;
		popFribourg.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // PUB_LED=publish for led, green toggle

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

