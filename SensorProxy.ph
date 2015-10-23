/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief Proxy class that handles communication with one sensor but resides on the same machine as the gateway. Each proxy is connected to one sensor on the network.
 *
 *
 */

#ifndef SENSOR_PROXY_H
#define SENSOR_PROXY_H

#include <atomic>
#include <fstream>


#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "MySQLConn.h"

#include "POPSensor.ph"
#include "lineComm/popwin_messages.h"


parclass SensorProxy {
	classuid(1903);

public:
	SensorProxy(int x_id, const std::string& x_url, const std::string& x_device) @{ od.url(x_url); };
	~SensorProxy();

	sync seq POPSensorData executeQuery(string sqlRequest);
	sync seq void TestSQL();
	//sync seq void TestInsertSQL();

	/// Send notification to the connected sensor
	async seq void Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message);
	async seq void Notify(int x_measurementType, int x_measurementUnit, double x_data);
	async seq void Notify(int x_measurementType, int x_measurementUnit, int x_data);

	/// Send a publication to the connected sensor
	async seq void Publish(int x_measurementType);
	/// Gateway cannot send notifications to actuators anymore
	async seq void UnPublish(int x_measurementType);
	//async seq void Publish(int x_publicationType, int x_data);
	//async seq void Publish(int x_publicationType, double x_data);
	//async seq void Publish(int x_publicationType, const std::string& x_data);

	/// Apply a reduce function to the data {size, min, max, aver, sum, stdev}
	//sync seq double Reduce(int x_mtype, int x_dataType, int x_fct);

	/// Gateway can send notifications to actuators
	//async seq void CanPublish(int x_publicationType);

	/// Send a subscription to the connected sensor
	async seq void Subscribe(int x_measurementType, int x_dataType);
	/// Send an unsubscription to the connected sensor
	async seq void UnSubscribe(int x_measurementType, int x_dataType);

	/// Send a notification to set the gwID GW as a GW
	async seq void SetAsGateway(int gwID);

	/// Send data to the remote sensors
	async conc void StartListening();
	async conc void StopListening();

	/// Retrieve data gathered 
	//sync conc POPSensorData Gather();

	/// Clear data gathered 
	//async conc void Clear();
	async conc void clearDB();

	//sync conc int GetDataSize();

private:
	void SendRawData(const std::string& x_data);
	void ReadData(std::ostream& xr_ostream);
	void HandleIncomingMessage(const std::string& x_msg);

	void InsertSQL(struct NotifyMessage* msg);

	void copyFromResultSetToPOPSensorData(sql::ResultSet* set, POPSensorData* data);

	int m_fd;
	int m_id;
	std::atomic<bool>  m_listening;
	std::map<int,bool> m_subscriptions;
	std::map<int,bool> m_publications;

	// Different containers of data
	POPSensorData m_sensorData;

	sql::Driver *driver;
	sql::Connection *con;
};

#endif
