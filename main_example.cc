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
	if(argc != 2)
	{
		cout << "usage: popcrun <obj.map> ./main_example resource.json" << popcendl;
		exit(0);
	}
	// Input a list of commands
	cout << popcendl;
	cout << "=======================================================================================" <<popcendl;
	cout << " POPWin demo: Launching the demo                                                       " <<popcendl;
	cout << "=======================================================================================" <<popcendl;

	try
	{
		POPSensor popSensor("localhost", argv[1]);

		sleep(3);

		cout<<"\nAsk the list of available commands (command 0)"<<popcendl;
		popSensor.Broadcast(PUB_COMMAND, 0);

		sleep(3);

		cout<<"\nBroadcast a command that will generate data (command 2, 3 and 4)"<<popcendl;
		popSensor.Broadcast(PUB_COMMAND, 2);
		popSensor.Broadcast(PUB_COMMAND, 3);
		popSensor.Broadcast(PUB_COMMAND, 4);

		sleep(3);

		cout<<"\nBlink all leds once"<<popcendl;
		for(int i = 0 ; i < 3 ; i++)
		{
			popSensor.Broadcast(PUB_LED, i);
			sleep(1);
			popSensor.Broadcast(PUB_LED, i);
		}

		sleep(3);

		cout<<"\nAsk for temperature readings (command 1) three times"<<popcendl;
		popSensor.Broadcast(PUB_COMMAND, 1);
		sleep(1);
		popSensor.Broadcast(PUB_COMMAND, 1);
		sleep(1);
		popSensor.Broadcast(PUB_COMMAND, 1);

		sleep(3);

		cout<<"\nPrint the gathered data to screen"<<popcendl;
		POPSensorData data = popSensor.Gather();
		data.Print();

		cout<<"\nTemperature average is "<<data.Reduce<double>(MSR_TEMPERATURE, POPSensorData::POPReduceF::aver)<< popcendl;

		sleep(3);

		cout<<"\nPrint the gathered data to out/data_demo.csv"<<popcendl;
		ofstream of;
		of.open("out/data_demo.csv");
		of << "Time, Id, Measurement, Unit, data\n";
		data.PrintToFile(of);
		of.close();

		sleep(3);

		cout<<"\nClear data"<<popcendl;
		popSensor.Clear();

		cout<<"End of popwin main"<<popcendl;
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in popwin main loop: " << e.what() << popcendl;
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << popcendl;
		return 1;
	}


	return 0;
}
