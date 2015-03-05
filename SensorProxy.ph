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
	SensorProxy(const std::string& x_url) @{ od.url(x_url); };
	~SensorProxy();

	/// Subscribe to a sensor
	// async conc void SubscribeMe(POPSensor& xr_gateway);

	// sync seq void Publish(int x_publicationType, double x_data);
	sync seq void Publish(int x_publicationType, int x_data);
	// sync seq void Publish(int x_publicationType, const std::string& x_data);

	/// Send data to the remote sensors
	async conc void StartListening();
	async conc void StopListening();
	sync seq std::vector<double>      RetrieveDataDouble();
	sync seq std::vector<int>         RetrieveDataInt();
	sync seq std::vector<std::string> RetrieveDataString();
	sync seq void ClearData();

private:
	void SendRawData(const std::string& JSONData);
	void ReadData(std::ostream& xr_ostream);
	void HandleIncomingMessage(const std::string& x_msg);
	// void HandleIncomingMessage(const SubsribeMessage& x_msg);
	// void HandleIncomingMessage(const NotifyMessage& x_msg);

	int m_fd;
	bool m_listening;

	// Different containers of data
	std::vector<std::string> m_stringData;
	std::vector<int>         m_intData;
	std::vector<double>      m_doubleData;
	std::vector<bool>        m_boolData;

	// std::vector<POPSensor*> m_subscribed;
};

#endif
