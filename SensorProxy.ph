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

class Gateway;

parclass SensorProxy {
	classuid(1903);

public:
	SensorProxy(POPString x_url) @{ od.url(x_url); };
	~SensorProxy();

	/// Subscribe to a sensor
	void SubscribeMe(/*Gateway* xp_gateway*/);


private:
	std::string m_data;
};

#endif
