/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief Gateway class for the POPWIN project
 *
 *
 */


#include "Gateway.ph"
#include <unistd.h>



Gateway::Gateway(POPString x_url)
{
	
}

Gateway::~Gateway()
{
	
}

/// Search and create sensor proxys for communication with sensors
void Gateway::CreateSensors()
{
	m_sensorsProxy.push_back(new SensorProxy("localhost"));
}

/// Subscribe to a sensor
void Gateway::Subscribe(SensorProxy& x_sensorProxy)
{
	x_sensorProxy.SubscribeMe();
}

/// Publish data (called by SensorProxy)
void Gateway::Publish(std::string x_data)
{
	m_data += x_data.c_str();
	m_data += ",";
}

/// Retrieve data that was published here
std::string Gateway::RetrieveData()
{
	return m_data;	
}


@pack(Gateway);
