/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensor class for the POPWIN project. This object handles the data gathering of a group of sensors.
 *
 *
 */


#include "POPSensor.ph"
#include <unistd.h>


using namespace std;

POPSensor::POPSensor(POPString x_url)
{
	
}

POPSensor::~POPSensor()
{
	for(auto it : m_sensorsProxy)
	{
		delete(it);
	}
}

/// Search and create sensor proxys for communication with sensors
void POPSensor::Connect()
{
	// TODO: Handle resource description files
	for(int i = 0 ; i < 4 ; i++)
	{
		m_sensorsProxy.push_back(new SensorProxy("localhost"));
	}
}

/// Subscribe to a sensor
void POPSensor::Subscribe(SensorProxy& x_sensorProxy)
{
	// x_sensorProxy.SubscribeMe(this);
}

/// Publish data (called by SensorProxy)
void POPSensor::Publish(std::string x_data)
{
	m_data += x_data.c_str();
	m_data += ",";
}

/// Retrieve data that was published here
std::string POPSensor::RetrieveData()
{
	return m_data;	
}


@pack(POPSensor);
