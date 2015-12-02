/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @author Marco Lourenço
 * @date   November 2015
 * @brief  Example executable for the POPWIN project: The main routine shows the usage of the different C++ class of the POPWIN project for 2 sensor networks.
 *
 *
 */

#include <fstream>

#include "POPSensor.ph"

using namespace std;

void waitPressKey()
{
	char c = '\n';
	while(true)
	{
		cout << "\nPress key to continue\n" << popcendl;
		//while(c == '\n')
		c = getchar();
		break;
	}
}

int main(int argc, char** argv)
{
	if(argc != 5)
	{
		cout << "usage: popcrun <obj.map> ./main_demo localhost INOUT.json 160.98.X.X IN.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		cout << "Creating POPSensor for WSN no1" << popcendl;
		POPSensor popLocal(argv[1], argv[2],0);

		cout << "Creating POPSensor for WSN no2" << popcendl;
		POPSensor popRemote(argv[3], argv[4],1);

		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_ALL_OFF); // MSR_LED=notify for led, all off

		waitPressKey();

		cout << "Broadcast green LED ON" << popcendl;
		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // MSR_LED=notify for led, green toggle

		waitPressKey();

		cout << "Gather temperatures..." << popcendl;
		for(int i = 0 ; i < 30 ; i++)
		{
            // PUB == OUT
			popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE); // PUB_LED=publish for led, green toggle

            // SUB == IN
			POPSensorData d1 = popLocal.executeQuery("SELECT genre,value FROM popwin_schema.POPSensorData WHERE genre=\"temperature\"");
			POPSensorData d2 = popRemote.executeQuery("SELECT genre,value FROM popwin_schema.POPSensorData WHERE genre=\"leds\"");
			d1.printAll();
			d2.printAll();
			popLocal.Clear();
			popRemote.Clear();

			//waitPressKey();
			sleep(1);
		}

		cout << "Broadcast green LED OFF" << popcendl;
		popLocal.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_OFF); // PUB_LED=publish for led, green toggle

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

