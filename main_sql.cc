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
	if(argc != 3)
	{
		cout << "usage: popcrun <obj.map> ./main_example localhost INOUT.json" << popcendl;
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

		//popLocal.TestSQL();
		//popLocal.TestInsertSQL();

		for(int i = 0 ; i < 3000 ; i++)
		{
			waitPressKey();
			cout << "Testing sql" << popcendl;
			popLocal.executeQuery("");
			popLocal.Clear();
			//popLocal.TestSQL();
		}

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

