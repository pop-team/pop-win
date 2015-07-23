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
		cout << "usage: popcrun <obj.map> ./main_example 160.98.X.X temp.json 160.98.X.X led.json" << popcendl;
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
		POPSensor popTemp(argv[1], argv[2]);
		POPSensorData temps;

		cout << "Creating POPSensor for leds" << popcendl;
		POPSensor popLed(argv[3], argv[4]);

		//POPSensorData in_leds;
		//in_leds = popLed.Gather();
		//POPSensorData out_leds;

		/* if using a scatter: not implemented yet
		for(int i = 0 ; i < in_leds.GetSize() ; i++)
		{
			auto header = in_leds.getHeader(i);
			int value = in_leds.getInt(i);
			if(value == 0)
				out_leds.InsertInt(header.id, 1); // not implemented yet

		}
		popLed.Scatter(out_leds);
		 */

		cout << "Broadcast green LED ON" << popcendl;
		popLed.Broadcast(PUB_LED, 1); // PUB_LED=publish for led, 1:green toggle

		ofstream of;
		of.open("outputfile.csv");

		cout << "Gather temperatures..." << popcendl;
		for(int i = 0 ; i < 30 ; i++)
		{
			temps = popTemp.Gather();
			popTemp.Clear();
			printf("\n-----------------------------------------------------------------------------------------------\n");
			temps.Print();
			printf("\n-----------------------------------------------------------------------------------------------\n");
			temps.PrintToFile(of);
			//cout << "sleeping..." << popcendl;
			sleep(15);
			popLed.Broadcast(PUB_LED, 1); // PUB_LED=publish for led, 1:green toggle
		}
		cout << "Finished gathering temperatures" << popcendl;
		//POPSensorData temps = POPSensorData(popTemp.Gather());
		//popTemp.Clear();
		//temps = popTemp.Gather();
		//temps.Print();
		//temps.PrintToFile(of);

		of.close();

		cout << "Broadcast green LED OFF" << popcendl;
		popLed.Broadcast(PUB_LED, 1); // PUB_LED=publish for led, 1:green toggle

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

