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
#include <fstream>
#include <json/json.h>
#include <json/reader.h>

using namespace std;


POPSensor::POPSensor(const std::string& x_url, const std::string& x_resourceFileName)
{
	// Read json resource file into string
	ifstream jif(x_resourceFileName);
	if(!jif.is_open())
		throw POPException("Cannot open json resource file", argv[1]);
	stringstream ss;
	ss << jif.rdbuf();
	jif.close();


	// Read json resource file
	m_jsonResources = ss.str();
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_jsonResources, root, false))
	{
		throw POPException("Error while reading json string"); // , m_jsonResources); // note: JSON string may be too long for buffer
	}

	string connectionType = root["gateway"].get("connection", "<not found>").asString();

	if(connectionType == "usb")
	{
		for(int i = 0 ; i < 4 ; i++)
		{
			// Scan usb ports for connected gateways
			stringstream port;
			port << "/dev/ttyUSB" << i;
			try
			{
				m_sensorsProxy.push_back(new SensorProxy(1000 + i, "localhost", port.str()));
				cout<<"Creating sensor proxy with id="<<(1000+i)<<" on port "<<port.str()<<popcendl;
			}
			catch(...)
			{
				// cout<<"Cannot create a sensor proxy on port "<<port.str()<<popcendl;
			}
		}
	}
	else throw POPException("Only \"usb\" connection is supported for gateway", "connection=" + connectionType);
	cout<<"Created "<<m_sensorsProxy.size()<<" sensor proxy objects"<<popcendl;

	if(m_sensorsProxy.empty())
		throw POPException("No sensor proxy could be create. Did you connect the gateway mote via USB ?");
}

POPSensor::~POPSensor()
{
	m_jsonResources = "";
	cout<<"Destroying POPSensor and its "<<m_sensorsProxy.size() << " SensorProxy." <<popcendl;
	for(auto it : m_sensorsProxy)
	{
		delete(it);
	}
	m_sensorsProxy.clear();
}

bool POPSensor::IsConnected()
{
	return !m_sensorsProxy.empty();
}

void POPSensor::StartListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StartListening();
	}
	SubscribeToResources();
}

void POPSensor::StopListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StopListening();
	}
}

POPSensorData POPSensor::Gather()
{
	POPSensorData fullData;
	for(auto it : m_sensorsProxy)
	{
		POPSensorData data(it->Gather());

		fullData.dataDouble.insert(data.dataDouble.begin(), data.dataDouble.end());
		fullData.dataInt.insert(data.dataInt.begin(), data.dataInt.end());
		fullData.dataString.insert(data.dataString.begin(), data.dataString.end());
	}
	return fullData;
}


void POPSensor::ClearData()
{
	for(auto it : m_sensorsProxy)
	{
		it->ClearData();
	}
}

void POPSensor::Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message)
{
	for(auto it : m_sensorsProxy)
	{
		it->Notify(x_measurementType, x_measurementUnit, x_message);
	}
}


void POPSensor::Subscribe(int x_measurementType, int x_dataType)
{
	for(auto it : m_sensorsProxy)
	{
		it->Subscribe(x_measurementType, x_dataType);
	}
}


void POPSensor::Publish(int x_publicationType, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
	}
}

void POPSensor::SubscribeToResources()
{
	if(m_jsonResources.empty())
		throw POPException("No resources specified. Did you connect ?");
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(m_jsonResources, root, false))
	{
		throw POPException("Error while reading json string", m_jsonResources);
	}


	cout << root["wsns"]["nodes"].size() << popcendl;
	for(int i = 0 ; i < root["wsns"]["nodes"].size() ; i++)
	{

		enum MeasurementType mtype = translateMeasurementType(root["wsns"]["nodes"][i].get("measureType", "<not found>").asString().c_str()); // TODO replace measurement with measure
		enum DataType dtype        = translateDataType       (root["wsns"]["nodes"][i].get("dataType", "<not found>").asString().c_str());
		bool direction             = root["wsns"]["nodes"][i].get("direction", "<not found>").asString().c_str() == string("IN");

		if(mtype == static_cast<int>(MSR_LOG))
			throw POPException("measureType not found in JSON resources description");
		if(dtype == static_cast<int>(TYPE_UNKNOWN))
			throw POPException("dataType not found in JSON resources description");

		for(auto it : m_sensorsProxy)
		{
			if(direction)
			{
				cout << "Subscribe to " << explainMeasurementType(mtype) << " " << explainDataType(dtype) << " direction:" << (direction ? "IN" : "OUT") << popcendl;
				it->Subscribe(mtype, dtype);
			}
		}
		//TODO store
	}
}

@pack(POPSensor);
