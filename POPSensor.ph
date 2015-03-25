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

#include <map>
#include <string>
#include "popwin_messages.h"

class SensorProxy;

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

	// Defind stream operator for easy printing
	friend std::ostream& operator<< (std::ostream& x_stream, const RecordHeader& x_rec)
	{
		x_stream << "time: " << x_rec.timeStamp << " id:" << x_rec.id << " type:" << explainMeasurementType(x_rec.measurementType) << " unit:" << explainMeasurementUnit(x_rec.unit);
		return x_stream;
	}

	unsigned int         timeStamp;
	enum MeasurementType measurementType;
	// enum DataType        dataType;
	int                  id;
	enum MeasurementUnit unit;
};


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
	std::map<RecordHeader, double>      RetrieveDataDouble();
	std::map<RecordHeader, int>         RetrieveDataInt();
	std::map<RecordHeader, std::string> RetrieveDataString();

	/// Clear data gathered 
	void ClearData();
private:
	/// Subscribe to a sensor
	// void Subscribe(SensorProxy& x_sensorProxy);

	std::vector<SensorProxy*> m_sensorsProxy;
};

#endif
