/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief Gateway class for the POPWIN project
 *
 *
 */

#ifndef GATEWAY_H
#define GATEWAY_H

#include "SensorProxy.ph"

parclass Gateway {
	classuid(1902);

public:
	// Gateway(int newID, int wanted, int minp) @{ od.search(0, 3, 0); };
	Gateway(POPString x_url) @{ od.url(x_url); };
	~Gateway();

	/// Search and create sensor proxys for communication with sensors
	void CreateSensors();

	/// Subscribe to a sensor
	void Subscribe(SensorProxy& x_sensorProxy);

	/// Publish data (called by SensorProxy)
	void Publish(std::string x_data);

	/// Retrieve data gathered 
	std::string RetrieveData();

private:
	std::string m_data;
	std::vector<SensorProxy*> m_sensorsProxy;
};

#endif
