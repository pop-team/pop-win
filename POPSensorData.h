/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensorData class for the POPWIN project. This object represents the data gathered from sensor.
 *
 *
 */

#ifndef POPSENSOR_DATA_H
#define POPSENSOR_DATA_H

#include <map>
#include <string>
#include "popwin_messages.h"

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
		// x_stream << "time: " << x_rec.timeStamp << " id:" << x_rec.id << " type:" << explainMeasurementType(x_rec.measurementType) << " unit:" << explainMeasurementUnit(x_rec.unit);
		x_stream << x_rec.timeStamp << ", " << x_rec.id << ", " << explainMeasurementType(x_rec.measurementType) << ", " << explainMeasurementUnit(x_rec.unit);
		return x_stream;
	}

	unsigned int         timeStamp;
	enum MeasurementType measurementType;
	// enum DataType        dataType;
	int                  id;
	enum MeasurementUnit unit;
};


class POPSensorData : public POPBase
{
	public:
	void Serialize(POPBuffer &buf, bool pack);
	void Print();
	void PrintToFile(std::ostream& xr_ostream);
	void Clear();


	std::map<RecordHeader, std::string> dataString;
	std::map<RecordHeader, int>         dataInt;
	std::map<RecordHeader, double>      dataDouble;
};


#endif
