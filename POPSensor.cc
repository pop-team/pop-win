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
	Initialize(x_resourceFileName);
}

POPSensor::POPSensor(int x_pow, const std::string& x_resourceFileName)
{
	Initialize(x_resourceFileName);
}

POPSensor::~POPSensor()
{
	StopListening();
	m_jsonResources = "";
	cout<<"Destroying POPSensor and its "<<m_sensorsProxy.size() << " SensorProxy." <<popcendl;
	for(auto it : m_sensorsProxy)
	{
		delete(it);
	}
	m_sensorsProxy.clear();
}

void POPSensor::Initialize(const std::string& x_resourceFileName)
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

	int counter = 0;

	for(int i = 0 ; i < root["gateways"].size() ; i++)
	{

		string connectionType = root["gateways"][i].get("connection", "<not found>").asString();
		string url            = root["gateways"][i].get("url", "localhost").asString();

		if(connectionType == "usb")
		{
			for(int j = 0 ; j < 4 ; j++)
			{
				// Scan usb ports for connected gateways
				stringstream port;
				port << "/dev/ttyUSB" << j;
				try
				{
					m_sensorsProxy.push_back(new SensorProxy(1000 + counter, url, port.str()));
					cout<<"Creating sensor proxy with id="<<(1000 + counter)<<" on port "<<port.str()<<popcendl;
					counter++;
				}
				catch(...)
				{
					// cout<<"Cannot create a sensor proxy on port "<<port.str()<<popcendl;
				}
			}
		}
		else throw POPException("Only \"usb\" connection is supported for gateway", "connection=" + connectionType);
		cout<<"Created "<<m_sensorsProxy.size()<<" sensor proxy objects"<<popcendl;
	}

	if(m_sensorsProxy.empty())
		throw POPException("No sensor proxy could be create. Did you connect the gateway mote via USB ?");

	StartListening();
	SubscribeToResources();

	// Send a command to the gateway mote to set it as the gateway
	// note: since some mote have trouble with the serial line you may need to set it manually later
	for(auto it : m_sensorsProxy)
	{
		it->Publish(PUB_COMMAND, 8); // 8 is the command id for set_as
	}
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
		fullData.Insert(it->Gather());
	}
	return fullData;
}


void POPSensor::Clear()
{
	for(auto it : m_sensorsProxy)
	{
		it->Clear();
	}
}

void POPSensor::Broadcast(int x_publicationType, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
	}
}

void POPSensor::Broadcast(int x_publicationType, double x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
	}
}

void POPSensor::Broadcast(int x_publicationType, const std::string& x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Publish(x_publicationType, x_data);
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

int POPSensor::GetSize()
{
	int cpt = 0;
	for(auto it : m_sensorsProxy)
	{
		cpt += it->GetSize();
	}
	return cpt;
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

	for(int i = 0 ; i < root["wsns"]["nodes"].size() ; i++)
	{
		enum MeasurementType mtype = translateMeasurementType(root["wsns"]["nodes"][i].get("measurementType", "log").asString().c_str()); // TODO: Maybe remove wsns
		enum DataType dtype        = translateDataType       (root["wsns"]["nodes"][i].get("dataType", "unknown").asString().c_str());
		bool incoming              = false;
		string str = root["wsns"]["nodes"][i].get("direction", "<not found>").asString();
		if(str == "IN")
		{
			incoming = true;
		}
		else if(str != "OUT")
		{
			throw POPException("Error in JSON: direction must be \"IN\" or \"OUT\"");
		}

		if(mtype == static_cast<int>(MSR_LOG))
			throw POPException("measurementType not found in JSON resources description");

		// note: dataType is not mandatory
		// if(dtype == static_cast<int>(TYPE_UNKNOWN))
			// throw POPException("dataType not found in JSON resources description");

		int cpt = 0;
		for(auto it : m_sensorsProxy)
		{
			if(incoming)
			{
				cout << "Gateway " << 1000 + cpt << " subscribes to " << explainMeasurementType(mtype) << " type:" << explainDataType(dtype) << " direction:" << (incoming ? "IN" : "OUT") << popcendl;
				it->Subscribe(mtype, dtype);
			}
			cpt++;
		}
	}
}


@pack(POPSensor);
