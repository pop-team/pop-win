/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensor class for the POPWIN project. This object handles the data gathering of a group of sensors.
 *
 *
 */


#include "POPSensorData.h"

using namespace std;


void POPSensorData::Serialize(POPBuffer &buf, bool pack)
{
	/* TODO
	   buf.Pack(&stringData,1);
	   buf.Pack(&intData,1);
	   buf.Pack(&doubleData,1);
	 */
}

void POPSensorData::Print()
{
	for(auto elem : dataDouble)
	{
		cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
	}

	// Print int data
	cout << "\nRecords found on proxy <int>: "<< dataInt.size() << popcendl;
	for(auto elem : dataInt)
	{
		cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
	}

	// Print string data
	cout << "\nRecords found on proxy <string>: "<< dataString.size() << popcendl;
	for(auto elem : dataString)
	{
		cout<< ">>" << elem.first << " -- " << elem.second << popcendl;
	}
}

void POPSensorData::Clear()
{
	dataString.clear();
	dataInt.clear();
	dataDouble.clear();
}
