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
		cout<<"Creating sensor proxy with id="<<(1000+i)<<popcendl;
		m_sensorsProxy.push_back(new SensorProxy(1000 + i, "localhost", "/dev/ttyUSB0"));

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

map<RecordHeader, double> POPSensor::RetrieveDataDouble()
{
	map<RecordHeader, double> records;
	for(auto it : m_sensorsProxy)
	{
		auto gatheredData(it->RetrieveDataDouble());
		records.insert(gatheredData.begin(), gatheredData.end());
	}
	return records;
}

map<RecordHeader, int> POPSensor::RetrieveDataInt()
{
	map<RecordHeader, int> records;
	for(auto it : m_sensorsProxy)
	{
		auto gatheredData(it->RetrieveDataInt());
		records.insert(gatheredData.begin(), gatheredData.end());
	}
	return records;
}

map<RecordHeader, string> POPSensor::RetrieveDataString()
{
	map<RecordHeader, string> records;
	for(auto it : m_sensorsProxy)
	{
		auto gatheredData(it->RetrieveDataString());
		records.insert(gatheredData.begin(), gatheredData.end());
	}
	return records;
}

void POPSensor::ClearData()
{
	for(auto it : m_sensorsProxy)
	{
		it->ClearData();
	}
}

void POPSensor::Publish(int x_publicationType, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
	}
}

@pack(POPSensor);
