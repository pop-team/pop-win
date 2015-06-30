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
#include <limits>
#include <numeric>
#include <algorithm>
#include "popwin_messages.h"

/// Data structure to store a record that comes from a notification message
class RecordHeader : POPBase
{
public:
	/// Constructor
	RecordHeader();

	/// Constructor
	RecordHeader(unsigned int x_timeStamp, const NotifyMessage& x_msg);

	/// Serialize the object
	void Serialize(POPBuffer &buf, bool pack);

	/// We need to define this operator to use this structure as key for maps
	inline bool operator<(RecordHeader const& n2) const
	{
		return timeStamp != n2.timeStamp ? timeStamp < n2.timeStamp : this < &n2;
	}

	/// A time stamp that indicates when the record was created
	unsigned int         timeStamp;

	/// The type of measurement
	enum MeasurementType measurementType;
	// enum DataType        dataType;

	/// Id of the emitter
	int                  id;

	/// Unit of measurement
	enum MeasurementUnit unit;
};

/// Define stream operator for easy printing
std::ostream& operator<< (std::ostream& x_stream, const RecordHeader& x_rec);

// ================================================================================

template<typename T> T map_acc(T lhs, const std::pair<RecordHeader, T> & rhs)
{
	  return lhs + rhs.second;
}

// ================================================================================

/// A serializable object that can store the results of data acquisition
class POPSensorData : public POPBase
{
	public:

	/// The different reduce functions
	typedef enum reduceFunctions {size, min, max, aver, sum, stdev} POPReduceF;

	/// Serialize and deserialize the class
	void Serialize(POPBuffer &buf, bool pack);

	/// Print the data to stdout
	void Print();

	/// Print the data to a .csv file
	void PrintToFile(std::ostream& xr_ostream);

	/// Clear the data
	void Clear();

	/// Return the size of the data
	int  GetSize() const;

	/// Insert a new record into the data
	void Insert(const POPSensorData&);

	/// Insert a new record into the data
	template<typename T>void Insert(std::pair<RecordHeader, T>& x_pair){RefData<T>().insert(x_pair);}

	/// Apply reduce on all the content
	/// note that there are two template types. The second is meant for the accumulator:
	/// 	for int use <int,long long>
	/// 	for double use <double,double>
	template<typename T> double Reduce(enum MeasurementType x_mtype, POPReduceF x_fct) const
	{
		std::vector<T> vect;

		for(const auto& elem : GetData<T>())
		{
			if(elem.first.measurementType == x_mtype)
				vect.push_back(elem.second);
		}
		if(vect.empty())
			return 0;

		switch(x_fct)
		{
			case size:  return vect.size();
			case min:   return *std::min_element(vect.begin(), vect.end());
			case max:   return *std::max_element(vect.begin(), vect.end());
			case aver:  return std::accumulate(vect.begin(), vect.end(), 0.0) / vect.size();
			case sum:   return std::accumulate(vect.begin(), vect.end(), 0.0);
			case stdev:
			{
				double sum = std::accumulate(vect.begin(), vect.end(), 0.0);
				double mean = sum / vect.size();
				std::vector<double> diff(vect.size());
				std::transform(vect.begin(), vect.end(), diff.begin(),
				               std::bind2nd(std::minus<double>(), mean));
				double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
				return std::sqrt(sq_sum / vect.size());
			}

		}
	}

private:
	template<typename T>std::map<RecordHeader, T>&       RefData();
	template<typename T>const std::map<RecordHeader, T>& GetData() const;

	std::map<RecordHeader, std::string> dataString;
	std::map<RecordHeader, int>         dataInt;
	std::map<RecordHeader, double>      dataDouble;
};


#endif
