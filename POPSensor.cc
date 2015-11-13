/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensor class for the POPWIN project. This object handles the data gathering of a group of sensors.
 *
 *
 */

#include <unistd.h>
#include <fstream>
#include <json/json.h>
#include <json/reader.h>
#include <string>

#include "POPSensor.ph"
#include "SensorProxy.ph"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/datatype.h>
#include <cppconn/statement.h>

using namespace std;

/// Constructor (URL of target platteform is specified)
POPSensor::POPSensor(const std::string& x_url, const std::string& x_resourceFileName, const int id)
{
	PopSID = 10000*id;
	cout<<"Creating POPSensor with id="<< PopSID << popcendl;
	Initialize(x_resourceFileName);
	// init DB connection
	/* Create a connection */
	cout<<"Instantiating driver" << popcendl;
	driver = get_driver_instance();
	cout<<"Connecting to db" << popcendl;
	con = driver->connect("tcp://127.0.0.1:3306", "root", "toor");
	/* Connect to the MySQL test database */
	//cout<<"Setting schema" << popcendl;
	//con->setSchema("popwin_schema");
}

/// Constructor (power requirement of target platteform is specified)
POPSensor::POPSensor(int x_pow, const std::string& x_resourceFileName, const int id)
{
	PopSID = 10000*id;
	cout<<"Creating POPSensor with id="<< PopSID << popcendl;
	Initialize(x_resourceFileName);
}

/// Destructor
POPSensor::~POPSensor()
{
	StopListening();
	cout<<"Destroying POPSensor and its "<<m_sensorsProxy.size() << " SensorProxy." <<popcendl;
	SubscribeToResources(false); // unsubscribe
	m_jsonResources = "";
	for(auto it : m_sensorsProxy)
	{
		//it->UnPublish(MSR_SET_GW); // this one is not from JSON file, cf. Initialize()
		delete(it);
	}
	m_sensorsProxy.clear();
	cout<<"Finished destroying POPSensor" << popcendl;
}

/// Initialization using parameters in resource.json
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
		// TODO: redesigne because in theory multiple GW per POPSensor possible, but not now
		string gwID			  = root["gateways"][i].get("id", "0").asString();

		if(connectionType == "usb")
		{
			for(int j = 0 ; j < 4 ; j++)
			{
				// Scan usb ports for connected gateways
				stringstream port;
				port << "/dev/ttyUSB" << j;
				try
				{
					m_sensorsProxy.push_back(new SensorProxy(PopSID + 1000 + counter, url, port.str()));
					cout<<"Creating sensor proxy with id="<<(PopSID + 1000 + counter)<<" on port "<<port.str()<<popcendl;
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
		for(auto it : m_sensorsProxy) // should be only one for now, else error
		{
			//it->TestSQL();
			it->Publish(MSR_SET_GW);
			it->SetAsGateway(stoi(gwID));
		}
	}

	if(m_sensorsProxy.empty())
		throw POPException("No sensor proxy could be create. Did you connect the gateway mote via USB ?");

	StartListening();
	SubscribeToResources(true); // subscribe

	// Send a command to the gateway mote to set it as the gateway
	// note: since some mote have trouble with the serial line you may need to set it manually later
	//for(auto it : m_sensorsProxy)
	//{
	//it->Publish(MSR_SET_GW);
	//it->Publish(PUB_COMMAND, 8); // 8 is the command id for set_as
	//it->Notify(MSR_SET_GW, UNT_NONE, 0);
	//}
}

/// Return true if the POPSensor is connected to at least one sensor
bool POPSensor::IsConnected()
{
	return !m_sensorsProxy.empty();
}

/// Start listening to messages coming from sensors
void POPSensor::StartListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StartListening();
	}
}

/// Stop listening to messages coming from sensors
void POPSensor::StopListening()
{
	for(auto it : m_sensorsProxy)
	{
		it->StopListening();
	}
}

/// Return a POPSensorData structure containing the messages received from sensors according to SQL request
POPSensorData POPSensor::executeQuery(string sqlRequest)
{
	POPSensorData d;

	try {
		sql::Statement *stmt;
		sql::ResultSet *res;

		stmt = con->createStatement();
		res = stmt->executeQuery(sqlRequest);

		// Get names of columns requested by user
		sql::ResultSetMetaData* rsmd = res->getMetaData();
		int columnCount = rsmd->getColumnCount();
		for(int i = 0;i<columnCount;i++)
		{
			d.insertColName(rsmd->getColumnLabel(i+1));
			d.insertColType(rsmd->getColumnType(i+1));
		}
		copyFromResultSetToPOPSensorData(res,rsmd,&d);
		cout<<"Cleaning objects" << popcendl;
		delete res;
		delete stmt;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "	<< __LINE__ << popcendl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << popcendl;
	}
	return d;
}

void POPSensor::copyFromResultSetToPOPSensorData(sql::ResultSet* set, sql::ResultSetMetaData* rsmd, POPSensorData* data)
{
	while (set->next()) {
		map< string,boost::variant< int, float, double, std::string > > rSetToPOPData;
		int columnCount = rsmd->getColumnCount();
		for(int i = 0;i<columnCount;i++)
		{
			const string crtLabel = rsmd->getColumnLabel(i+1);
			if(rsmd->getColumnType(i+1) == sql::DataType::INTEGER)
			{
				rSetToPOPData[crtLabel] = boost::variant< int, float, double, std::string >(set->getInt(crtLabel));
			}
			// DataType float doesn't exist, even if FLOAT is available in database. REAL has the same value (7)
			else if(rsmd->getColumnType(i+1) == sql::DataType::REAL)
			{
				// getFloat doesn't exist, use double instead
				rSetToPOPData[crtLabel] = boost::variant< int, float, double, std::string >((double)set->getDouble(crtLabel));
			}
			else if(rsmd->getColumnType(i+1) == sql::DataType::DOUBLE)
			{
				rSetToPOPData[crtLabel] = boost::variant< int, float, double, std::string >((double)set->getDouble(crtLabel));
			}
			else if(rsmd->getColumnType(i+1) == sql::DataType::VARCHAR)
			{
				rSetToPOPData[crtLabel] = boost::variant< int, float, double, std::string >(set->getString(crtLabel));
			}
		}

		data->insert(rSetToPOPData);
		rSetToPOPData.clear();
	}
}


/// Return a POPSensorData structure containing the messages received from sensors
/*POPSensorData POPSensor::Gather()
{
	POPSensorData fullData;
	for(auto it : m_sensorsProxy)
	{
		fullData.Insert(it->Gather());
	}
	return fullData;
}*/

/// Clear the stored messages
void POPSensor::Clear()
{
	try {
		sql::Statement *stmt;

		stmt = con->createStatement();
		stmt->execute("Truncate table popwin_schema.POPSensorData;");
		delete stmt;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "	<< __LINE__ << popcendl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << popcendl;
	}
}

/// Broadcast a message to all sensors
/*void POPSensor::Broadcast(int x_publicationType, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		//it->Publish(x_publicationType, x_data);
	}
}*/

/// Broadcast a message to all sensors
void POPSensor::Broadcast(int x_genre, int x_unit, int x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Notify(x_genre, x_unit, x_data);
	}
}

/// Broadcast a message to all sensors
/*void POPSensor::Broadcast(int x_publicationType, double x_data)
{
	for(auto it : m_sensorsProxy)
	{
		//it->Publish(x_publicationType, x_data);
	}
}*/

/// Broadcast a message to all sensors
void POPSensor::Broadcast(int x_genre, int x_unit, double x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Notify(x_genre, x_unit, x_data);
	}
}

/// Broadcast a message to all sensors
/*void POPSensor::Broadcast(int x_publicationType, const std::string& x_data)
{
	for(auto it : m_sensorsProxy)
	{
		//it->Publish(x_publicationType, x_data);
	}
}*/

/// Broadcast a message to all sensors
void POPSensor::Broadcast(int x_genre, int x_unit, const std::string& x_data)
{
	for(auto it : m_sensorsProxy)
	{
		it->Notify(x_genre, x_unit, x_data);
	}
}

/// Send a notification to all sensors
void POPSensor::Notify(int x_genre, int x_unit, const std::string& x_message)
{
	for(auto it : m_sensorsProxy)
	{
		it->Notify(x_genre, x_unit, x_message);
	}
}

/// Subscribe to messages of given type and data type
void POPSensor::Subscribe(int x_genre, int x_dataType)
{
	for(auto it : m_sensorsProxy)
	{
		it->Subscribe(x_genre, x_dataType);
	}
}

/// Return the size of the stored data
/*int POPSensor::GetDataSize()
{
	int cpt = 0;
	for(auto it : m_sensorsProxy)
	{
		cpt += it->GetDataSize();
	}
	return cpt;
}*/

/// Subscribe to all resources contained in resource.json
void POPSensor::SubscribeToResources(bool sub)
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
		enum MeasurementType mtype;
		enum DataType dtype;
		string locationType;
		enum MeasurementUnit unit;
		//enum PublicationType ptype;
		bool incoming              = false;
		bool outgoing			   = false;
		string str = root["wsns"]["nodes"][i].get("direction", "<not found>").asString();
		if(str == "IN")
		{
			incoming = true;
			mtype = translateMeasurementType(root["wsns"]["nodes"][i].get("genre", "log").asString().c_str());
			dtype = translateDataType(root["wsns"]["nodes"][i].get("type", "unknown").asString().c_str());
			unit = translateMeasurementUnit(root["wsns"]["nodes"][i].get("unit", "no unit").asString().c_str());
			locationType = root["wsns"]["nodes"][i].get("location", "none").asString();
		}
		else if(str == "OUT")
		{
			outgoing = true;
			mtype = translateMeasurementType(root["wsns"]["nodes"][i].get("genre", "log").asString().c_str());
		}
		else if(str == "INOUT")
		{
			incoming = true;
			outgoing = true;
			mtype = translateMeasurementType(root["wsns"]["nodes"][i].get("genre", "log").asString().c_str());
			dtype = translateDataType(root["wsns"]["nodes"][i].get("type", "unknown").asString().c_str());
			unit = translateMeasurementUnit(root["wsns"]["nodes"][i].get("unit", "no unit").asString().c_str());
			locationType = root["wsns"]["nodes"][i].get("location", "none").asString();
		}
		else
		{
			throw POPException("Error in JSON: direction must be \"IN\", \"INOUT\" or \"OUT\"");
		}

		if((str == "IN" || str == "OUT" || str == "INOUT") && mtype == static_cast<int>(MSR_LOG))
			throw POPException("genre not found in JSON resources description");

		// note: dataType is not mandatory
		// if(dtype == static_cast<int>(TYPE_UNKNOWN))
		// throw POPException("dataType not found in JSON resources description");

		int cpt = 0;
		for(auto it : m_sensorsProxy)
		{
			if(!sub)
			{
				if(incoming)
				{
					cout << "Gateway " << PopSID + 1000 + cpt << " unsubscribes to " << explainMeasurementType(mtype) << " type:" << explainDataType(dtype) << " direction: IN" << popcendl;
					it->UnSubscribe(mtype, dtype);
				}
				if(outgoing)
				{
					cout << "Gateway " << PopSID + 1000 + cpt << " cannot publish " << explainMeasurementType(mtype) << " commands anymore," << " direction: OUT" << popcendl;
					it->UnPublish(mtype);
				}
			}
			else
			{
				if(incoming)
				{
					cout << "Gateway " << PopSID + 1000 + cpt << " subscribes to " << explainMeasurementType(mtype) << " type:" << explainDataType(dtype) << " direction: IN" << popcendl;
					it->Subscribe(mtype, dtype);
				}
				if(outgoing)
				{
					cout << "Gateway " << PopSID + 1000 + cpt << " can publish " << explainMeasurementType(mtype) << " commands," << " direction: OUT" << popcendl;
					it->Publish(mtype);
				}
			}
			cpt++;
		}
	}
}

/// Apply a reduce operation to the stored data {size, min, max, aver, sum, stdev}
/*double POPSensor::Reduce(int x_mtype, int x_dataType, int x_fct)
{
	std::vector<double> vect;

	for(auto it : m_sensorsProxy)
	{
		vect.push_back(it->Reduce(x_mtype, x_dataType, x_fct));
	}

	if(vect.empty())
		return 0;

	switch(x_fct)
	{
	case POPSensorData::size:  return vect.size();
	case POPSensorData::min:   return *std::min_element(vect.begin(), vect.end());
	case POPSensorData::max:   return *std::max_element(vect.begin(), vect.end());
	case POPSensorData::aver:  return std::accumulate(vect.begin(), vect.end(), 0.0) / vect.size();
	case POPSensorData::sum:   return std::accumulate(vect.begin(), vect.end(), 0.0);
	case POPSensorData::stdev:
	{
		double sum = std::accumulate(vect.begin(), vect.end(), 0.0);
		double mean = sum / vect.size();
		std::vector<double> diff(vect.size());
		std::transform(vect.begin(), vect.end(), diff.begin(),
				std::bind2nd(std::minus<double>(), mean));
		double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		return std::sqrt(sq_sum / vect.size());
	}

	}
}*/

@pack(POPSensor);
