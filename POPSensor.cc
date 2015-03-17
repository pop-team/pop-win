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
		m_sensorsProxy.push_back(new SensorProxy(1000 + i, "localhost"));

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

void POPSensor::RetrieveDataDouble()
{
	for(auto it : m_sensorsProxy)
	{
		map<RecordHeader, double> gatheredData(it->RetrieveDataDouble());
		cout << "Records found on proxy: "<< gatheredData.size() << popcendl;
		for(auto elem : gatheredData)
		{
			cout<< ">>" << elem.first.timeStamp << " id:" << elem.first.id << " type:" << explainMeasurementType(elem.first.measurementType) << " unit:" << explainMeasurementUnit(elem.first.unit) << ": " << elem.second << popcendl;
		}
	}
}

void POPSensor::RetrieveDataInt()
{
	for(auto it : m_sensorsProxy)
	{
		map<RecordHeader, int> gatheredData(it->RetrieveDataInt());
		cout << "Records found on proxy: "<< gatheredData.size() << popcendl;
		for(auto elem : gatheredData)
		{
			cout<< ">>" << elem.first.timeStamp << " id:" << elem.first.id << " type:" << explainMeasurementType(elem.first.measurementType) << " unit:" << explainMeasurementUnit(elem.first.unit) << ": " << elem.second << popcendl;
		}
	}
}

void POPSensor::RetrieveDataString()
{
	for(auto it : m_sensorsProxy)
	{
		map<RecordHeader, string> gatheredData(it->RetrieveDataString());
		cout << "Records found on proxy: "<< gatheredData.size() << popcendl;
		for(auto elem : gatheredData)
		{
			cout<< ">>" << elem.first.timeStamp << " id:" << elem.first.id << " type:" << explainMeasurementType(elem.first.measurementType) << " unit:" << explainMeasurementUnit(elem.first.unit) << ": " << elem.second << popcendl;
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

void POPSensor::Publish(int x_publicationType, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
	}
}

@pack(POPSensor);
