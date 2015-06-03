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
	RecordHeader();
	RecordHeader(unsigned int x_timeStamp, const NotifyMessage& x_msg);
	void Serialize(POPBuffer &buf, bool pack);

	// We need to define this operator to use this structure as key for maps
	inline bool operator<(RecordHeader const& n2) const
	{
		return timeStamp != n2.timeStamp ? timeStamp < n2.timeStamp : this < &n2;
	}

	unsigned int         timeStamp;
	enum MeasurementType measurementType;
	// enum DataType        dataType;
	int                  id;
	enum MeasurementUnit unit;
};

// Define stream operator for easy printing
std::ostream& operator<< (std::ostream& x_stream, const RecordHeader& x_rec);



class POPSensorData : public POPBase
{
	public:
	void Serialize(POPBuffer &buf, bool pack);
	void Print();
	void PrintToFile(std::ostream& xr_ostream);
	void Clear();
	int  GetSize() const;
	void Insert(std::pair<RecordHeader, int>&);
	void Insert(std::pair<RecordHeader, double>&);
	void Insert(std::pair<RecordHeader, std::string>&);
	void Insert(const POPSensorData&);

private:
	std::map<RecordHeader, std::string> dataString;
	std::map<RecordHeader, int>         dataInt;
	std::map<RecordHeader, double>      dataDouble;
};


#endif
