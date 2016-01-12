/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @author Marco Lourenço
 * @date   November 2015
 * @brief POPSensor class for the POPWIN project. This object handles the data gathering of a group of sensor proxies.
 *
 *                                                                  ^^^^^^^^^^^^
 *   main-------POPSensor--- SensorProxy1 --- RemoteSensorX ---   ^Sensor network^
 *                      \                                           ^^^^^^^^^^^^  
 *                      |                                           
 *                      |
 *                      |                                                         
 *                      \                                           ^^^^^^^^^^^^  
 *                       --- SensorProxy2 --- RemoteSensorY ---   ^Sensor network^
 *                                                                  ^^^^^^^^^^^^
 *                            ...
 *
 */

#ifndef POPSENSOR_H
#define POPSENSOR_H

#include <map>
#include <string>
#include "gatewayMote/popwin_messages.h"
#include "POPSensorData.h"

class SensorProxy;

/// A POPSensor object organizes communication with the remote sensors by creating a set of SensorProxies. This is transparent to the user.
parclass POPSensor {
	classuid(1902);

public:

	POPSensor(const std::string& x_url, const std::string& x_resourceFileName, const int id) @{ od.url(x_url); };
	POPSensor(int x_pow, const std::string& x_resourceFileName, const int id) @{ od.power(x_pow); };
	~POPSensor();

    /// Return a POPSensorData structure containing the messages received from sensors according to SQL request
	POPSensorData executeQuery(string sqlRequest);

	/// Broacast data through the network for int data
	void Broadcast(int x_measurementType, int x_measurementUnit, int x_data);
	/// Broacast data through the network for double data
	void Broadcast(int x_measurementType, int x_measurementUnit, double x_data);
	/// Broacast data through the network for string data
	void Broadcast(int x_measurementType, int x_measurementUnit, const std::string& x_data);

	/// Clear data gathered 
	void Clear();


	/// Methods specific to the POPWin project

	/// Send a publication to sensors
	// void Publish(int x_publicationType, int x_data); --> use broadcast

	/// Check if connected to any sensor
	bool IsConnected();


private:

	void Initialize(const std::string& x_resourceFileName);

	/// Send a subscription to sensors
	void Subscribe(int x_measurementType, int x_dataType);

	/// Start listening on serial line
	void StartListening();

	/// Stop listening
	void StopListening();

	/// Subscribe to resources specified at connection
	void SubscribeToResources(bool unsub);

    /// Send notification to the connected sensor
	void Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message);

	/// List of sensor proxy. Each proxy can be connected to one sensor via serial line
	std::vector<SensorProxy*> m_sensorsProxy;

	/// Descriptino of resources
	std::string m_jsonResources;

	/// ID of the POPSensor
	int PopSID;

	/// Transform a ResultSet returned by MySQL into a serializable POPSensorData (for POP framework)
	void copyFromResultSetToPOPSensorData(sql::ResultSet* set, sql::ResultSetMetaData* rsmd, POPSensorData* data);

	sql::Driver *driver;
	sql::Connection *con;
};

#endif
