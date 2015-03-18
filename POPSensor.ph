/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensor class for the POPWIN project. This object handles the data gathering of a group of sensors.
 *
 *
 */

#ifndef POPSENSOR_H
#define POPSENSOR_H

#include "popwin_messages.h"

class SensorProxy;

parclass POPSensor {
	classuid(1902);

public:
	// POPSensor(int newID, int wanted, int minp) @{ od.search(0, 3, 0); };
	POPSensor(const std::string& x_url) @{ od.url(x_url); };
	~POPSensor();

	/// Search and create sensor proxys for communication with sensors
	void Connect();

	void StartListening();
	void StopListening();

	/// Publish data to this POPSensor
	// void Publish(const std::string& x_message); // TODO: send only relevant data
	// void Publish(); // TODO: send only relevant data
	// void Publish(int x_publicationType, double x_data);
	/// TODO Comment
	// TODO: Maybe handle enums in popc
	void Publish(int x_publicationType, int x_data);
	// void Publish(int x_publicationType, const std::string& x_data);

	/// Retrieve data gathered 
	void RetrieveDataDouble();
	void RetrieveDataInt();
	void RetrieveDataString();

	/// Clear data gathered 
	void ClearData();
private:
	/// Subscribe to a sensor
	// void Subscribe(SensorProxy& x_sensorProxy);

	std::vector<SensorProxy*> m_sensorsProxy;
};

#endif
