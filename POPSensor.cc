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

POPSensor::POPSensor(const std::string& x_url)
{
	
}

POPSensor::~POPSensor()
{
	cout<<"Destroying POPSensor and its "<<m_sensorsProxy.size() << " SensorProxy." <<popcendl;
	for(auto it : m_sensorsProxy)
	{
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
		// m_sensorsProxy.back()->SubscribeMe(*__POPThis_POPSensor);
	}
}

/// Subscribe to a sensor
/*
void POPSensor::Subscribe(SensorProxy& x_sensorProxy)
{
	// x_sensorProxy.SubscribeMe(this);
}
*/

void POPSensor::SendData(const std::string& JSONData)
{
	for(auto it : m_sensorsProxy)
	{
		it->SendData(JSONData);
	}
}

void POPSensor::StartListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StartListening();
	}
}

void POPSensor::StopListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StopListening();
	}
}

void POPSensor::RetrieveData()
{
	for(auto it : m_sensorsProxy)
	{
		vector<string> gatheredData(it->RetrieveData());
		cout << "Records found on proxy: "<< gatheredData.size() << popcendl;
		for(auto elem : gatheredData)
		{
			cout<< ">> " << elem << popcendl;
		}
	}
}

void POPSensor::ClearData()
{
	for(auto it : m_sensorsProxy)
	{
		it->ClearData();
	}
}

@pack(POPSensor);
