/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
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
#include "popwin_messages.h"
#include "POPSensorData.h"

class SensorProxy;

/// A POPSensor object organizes communication with the remote sensors by creating a set of SensorProxies. This is transparent to the user.
parclass POPSensor {
	classuid(1902);

public:
	// POPSensor(int newID, int wanted, int minp) @{ od.search(0, 3, 0); };
	POPSensor(const std::string& x_url, const std::string& x_resourceFileName, const int id) @{ od.url(x_url); };
	POPSensor(int x_pow, const std::string& x_resourceFileName, const int id) @{ od.power(x_pow); };
	~POPSensor();

	POPSensorData executeQuery(string sqlRequest);
	//void TestSQL();
	//void TestInsertSQL();

	/// Retrieve data gathered 
	//POPSensorData Gather();

	/// Broacast data through the network
	//void Broadcast(int x_publicationType, int x_data);
	//void Broadcast(int x_publicationType, double x_data);
	//void Broadcast(int x_publicationType, const std::string& x_data);
	void Broadcast(int x_measurementType, int x_measurementUnit, int x_data);
	void Broadcast(int x_measurementType, int x_measurementUnit, double x_data);
	void Broadcast(int x_measurementType, int x_measurementUnit, const std::string& x_data);

	// Apply a reduce function to the data {size, min, max, aver, sum, stdev}
	//double Reduce(int x_mtype, int x_dataType, int x_fct);

	/// Clear data gathered 
	void Clear();


	/// Methods specific to the POPWin project

	/// Send a publication to sensors
	// void Publish(int x_publicationType, int x_data); --> use broadcast

	/// Send notification to the connected sensor
	void Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message);

	/// Check if connected to any sensor
	bool IsConnected();

	// Size of collected data
	//sync conc int GetDataSize();


private:
	/// Initialize from config
	void Initialize(const std::string& x_resourceFileName);

	/// Send a subscription to sensors
	void Subscribe(int x_measurementType, int x_dataType);

	/// Start listening on serial line
	void StartListening();

	/// Stop listening
	void StopListening();

	/// Subscribe to resources specified at connection
	void SubscribeToResources(bool unsub);

	/// List of sensor proxy. Each proxy can be connected to one sensor via serial line
	std::vector<SensorProxy*> m_sensorsProxy;

	/// Descriptino of resources
	std::string m_jsonResources;

	/// ID of the POPSensor
	int PopSID;
};

#endif
