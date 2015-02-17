/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief Proxy class that handles communication with one sensor but resides on the same machine as the gateway
 *
 *
 */

#ifndef SENSOR_PROXY_H
#define SENSOR_PROXY_H

#include <vector>
#include <string>
#include "POPSensor.ph"
#include "SensorProxy.ph"


parclass SensorProxy {
	classuid(1903);

public:
	SensorProxy(POPString x_url) @{ od.url(x_url); };
	~SensorProxy();

	/// Subscribe to a sensor
	void SubscribeMe(POPSensor& xr_gateway);

	/// Send data to the remote sensors
	async seq void SendData(POPString JSONData);
	async conc void StartListening();
	async conc void StopListening();

private:
	void ReadData(std::ostream& xr_ostream);

	int m_fd;
	bool m_listening;
	std::string m_data;
	std::vector<POPSensor*> m_subscribed;
};

#endif
