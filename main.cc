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

	cout<<"Send {\"function\":0}"<<popcendl;
	sensorGroup.SendData("{\"function\":0}");
	cout<<"Read"<<popcendl;
	sensorGroup.ReadData();
	cout<<"Send {\"function\":3,\"led\":1}"<<popcendl;
	sensorGroup.SendData("{\"function\":3,\"led\":0}");
	sensorGroup.SendData("{\"function\":3,\"led\":1}");
	sensorGroup.SendData("{\"function\":3,\"led\":2}");
	sensorGroup.SendData("{\"function\":3,\"led\":3}");
	sensorGroup.SendData("{\"function\":3,\"led\":4}");
	cout<<"Sleep"<<popcendl;
	sleep(2);


	return 1;
}
