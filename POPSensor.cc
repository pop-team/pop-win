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
#include "SensorProxy.ph"
#include <unistd.h>


using namespace std;

POPSensor::POPSensor(POPString x_url)
{
	
}

POPSensor::~POPSensor()
{
	cout<<"Destroying sensor proxyAAAAA "<<m_sensorsProxy.size()<<popcendl;
	for(auto it : m_sensorsProxy)
	{
		cout<<"Destroying sensor proxy"<<popcendl;
		delete(it);
	}
}

/// Search and create sensor proxys for communication with sensors
void POPSensor::Connect()
{
	// TODO: Handle resource description files
	for(int i = 0 ; i < 1 ; i++)
	{
		cout<<"Creating sensor proxy"<<popcendl;
		m_sensorsProxy.push_back(new SensorProxy("localhost"));

		// Subscribe to sensor
		// m_sensorsProxy.back()->SubscribeMe(*this);
		// POPSensor tmp(GetAccessPointForThis()); // TODO
		// m_sensorsProxy.back()->SubscribeMe(tmp);
		m_sensorsProxy.back()->SubscribeMe(*__POPThis_POPSensor);
	}
}

/// Subscribe to a sensor
void POPSensor::Subscribe(SensorProxy& x_sensorProxy)
{
	// x_sensorProxy.SubscribeMe(this);
}


/// Retrieve data that was published here
std::string POPSensor::RetrieveData()
{
	return m_data;	
}

void POPSensor::SendData(POPString JSONData)
{
	for(auto it : m_sensorsProxy)
	{
		it->SendData(JSONData);
	}
}

void POPSensor::Publish(POPString x_message)
{
	LOG_DEBUG("gagne !!!!!!");
	cout<<"m0:"<<x_message<<popcendl;
	m_data += x_message.c_str();
	m_data += ",";

}

void POPSensor::StartListening()
{
	printf("POPSensor::StartListening\n");
	for(auto it : m_sensorsProxy)
	{
		it->StartListening();
	}
}

void POPSensor::StopListening()
{
	printf("POPSensor::StopListening\n");
	for(auto it : m_sensorsProxy)
	{
		it->StopListening();
	}
}


@pack(POPSensor);
