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
int main(int argc, char** argv)
{
	POPSensor gateway("localhost");
	gateway.Connect();


	return 1;
}
