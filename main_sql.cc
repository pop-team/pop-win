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
		cout << "Creating POPSensor 1" << popcendl;
		POPSensor popLocal(argv[1], argv[2],0);

		for(int i = 0 ; i < 1 ; i++)
		{
			waitPressKey();
			POPSensorData d = popLocal.executeQuery("SELECT genre,value FROM popwin_schema.POPSensorData WHERE genre=\"temperature\"");
			//POPSensorData d = popLocal.executeQuery("SELECT * FROM popwin_schema.POPSensorData");
			d.printAll();
			d.first();
			while(d.next())
			{
				cout << "Getting string value from database : " << d.getString("genre") << popcendl;
			}
			popLocal.Clear();
		}
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in popwin main loop: " << e.what() << popcendl;
		//exit(0);
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << popcendl;
		//exit(0);
		return 1;
	}


	return 0;
}

