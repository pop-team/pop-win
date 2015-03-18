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

#include <map>
#include <string>
#include "POPSensor.ph"
#include "SensorProxy.ph"
#include "lineComm/popwin_messages.h"



// Data structure to store a record that comes from a notification message
class RecordHeader : POPBase
{
public:
	RecordHeader()
	{
		measurementType = MSR_LOG;
		id              = 0;
		unit            = UNT_NONE;
		timeStamp       = 0;
	}
	RecordHeader(unsigned int x_timeStamp, const NotifyMessage& x_msg)
	{
		// cout << "Create record header id:" << x_msg.id << " units " << x_msg.unit << popcendl;
		measurementType = x_msg.measurementType;
		id              = x_msg.id;
		unit            = x_msg.unit;
		timeStamp       = x_timeStamp;
	}
	void Serialize(POPBuffer &buf, bool pack)
	{
		if(pack)
		{
			int mt = static_cast<int>(measurementType);
			buf.Pack(&mt,1);

			buf.Pack(&id,1);

			int mu = static_cast<int>(unit);
			buf.Pack(&mu,1);

			int ts = (int)timeStamp;
			buf.Pack(&timeStamp,1);
		}
		else
		{
			int mt = -1;
			buf.UnPack(&mt,1);
			measurementType = static_cast<enum MeasurementType>(mt);

			buf.UnPack(&id,1);

			int mu = -1;
			buf.UnPack(&mu,1);
			unit = static_cast<enum MeasurementUnit>(mu);

			buf.UnPack(&timeStamp,1);
		}
	}
	// We need to define this operator to use this structure as key for maps
	bool operator<(RecordHeader const& n2) const
	{
		return timeStamp != n2.timeStamp ? timeStamp < n2.timeStamp : this < &n2;
	}

	unsigned int         timeStamp;
	enum MeasurementType measurementType;
	// enum DataType        dataType;
	int                  id;
	enum MeasurementUnit unit;
};

parclass SensorProxy {
	classuid(1903);

public:
	SensorProxy(int x_id, const std::string& x_url, const std::string& x_device) @{ od.url(x_url); };
	~SensorProxy();

	/// Subscribe to a sensor
	// async conc void SubscribeMe(POPSensor& xr_gateway);

	// sync seq void Publish(int x_publicationType, double x_data);
	sync seq void Publish(int x_publicationType, int x_data);
	// sync seq void Publish(int x_publicationType, const std::string& x_data);

	/// Send data to the remote sensors
	async conc void StartListening();
	async conc void StopListening();
	sync seq std::map<RecordHeader, double>      RetrieveDataDouble();
	sync seq std::map<RecordHeader, int>         RetrieveDataInt();
	sync seq std::map<RecordHeader, std::string> RetrieveDataString();
	sync seq void ClearData();

private:
	void SendRawData(const std::string& JSONData);
	void ReadData(std::ostream& xr_ostream);
	void HandleIncomingMessage(const std::string& x_msg);
	// void HandleIncomingMessage(const SubsribeMessage& x_msg);
	// void HandleIncomingMessage(const NotifyMessage& x_msg);

	int m_fd;
	int m_id;
	bool m_listening;

	// Different containers of data
	std::map<RecordHeader, std::string> m_stringData;
	std::map<RecordHeader, int>         m_intData;
	std::map<RecordHeader, double>      m_doubleData;
	// std::vector<bool>        m_boolData;

	// std::vector<POPSensor*> m_subscribed;
};

#endif
