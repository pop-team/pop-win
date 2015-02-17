/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   Dec 2014
 * @brief  Main executable of the POPWIN project
 *
 *
 */
#include "POPSensor.ph"

using namespace std;

int main(int argc, char** argv)
{
	POPSensor sensorGroup("localhost");

	cout<<"Connect"<<popcendl;
	sensorGroup.Connect();
	cout<<"Sleep"<<popcendl;

	sleep(2);

	cout<<"Read the available functions:"<<popcendl;
	sensorGroup.StartListening();

	sleep(2);

	for(int i = 0 ; i < 5 ; i++)
	{
		// cout<<"Send {\"function\":0}"<<popcendl;
		sensorGroup.SendData("{\"function\":0}\n");
	}


/*
	cout<<"Send {\"function\":3,\"led\":1}"<<popcendl;
	sensorGroup.SendData("{\"function\":3,\"led\":0}\n");
	sensorGroup.SendData("{\"function\":3,\"led\":1}\n");
	sensorGroup.SendData("{\"function\":3,\"led\":2}\n");
	sensorGroup.SendData("{\"function\":3,\"led\":3}\n");
	sensorGroup.SendData("{\"function\":3,\"led\":4}\n");
	*/

	cout<<"main sleep"<<popcendl;

	sleep(2);
	cout<<"End of main sleep"<<popcendl;
	sensorGroup.StopListening();


	return 0;
}
