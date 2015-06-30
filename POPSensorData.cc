/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief POPSensorData class for the POPWIN project. This object represents the data gathered from sensor.
 *
 *
 */


#include "POPSensorData.h"
#include "fstream"

using namespace std;

/// Return a reference to the data
template<> map<RecordHeader, int>&         POPSensorData::RefData()   {return dataInt;}

/// Return a reference to the data
template<> map<RecordHeader, double>&      POPSensorData::RefData()   {return dataDouble;}

/// Return a reference to the data
template<> map<RecordHeader, string>&      POPSensorData::RefData()   {return dataString;}

/// Return the data
template<> const map<RecordHeader, int>&         POPSensorData::GetData() const   {return dataInt;}

/// Return the data
template<> const map<RecordHeader, double>&      POPSensorData::GetData() const   {return dataDouble;}

/// Return the data
template<> const map<RecordHeader, string>&      POPSensorData::GetData() const   {return dataString;}



RecordHeader::RecordHeader()
{
	measurementType = MSR_LOG;
	id              = 0;
	unit            = UNT_NONE;
	timeStamp       = 0;
}
RecordHeader::RecordHeader(unsigned int x_timeStamp, const NotifyMessage& x_msg)
{
	// cout << "Create record header id:" << x_msg.id << " units " << x_msg.unit << popcendl;
	measurementType = x_msg.measurementType;
	id              = x_msg.id;
	unit            = x_msg.unit;
	timeStamp       = x_timeStamp;
}

void RecordHeader::Serialize(POPBuffer &buf, bool pack)
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

// Define stream operator for easy printing
ostream& operator<< (ostream& x_stream, const RecordHeader& x_rec)
{
	// x_stream << "time: " << x_rec.timeStamp << " id:" << x_rec.id << " type:" << explainMeasurementType(x_rec.measurementType) << " unit:" << explainMeasurementUnit(x_rec.unit);
	x_stream << x_rec.timeStamp << ", " << x_rec.id << ", " << explainMeasurementType(x_rec.measurementType) << ", " << explainMeasurementUnit(x_rec.unit);
	return x_stream;
}


// ================================================================================================================================================================

template<class T> void serialize(map<RecordHeader, T>& records, POPBuffer &buf, bool pack)
{
	if(pack)
	{
		int size = records.size();
		buf.Pack(&size, 1);
		for(auto& elem : records)
		{
			RecordHeader head = elem.first;
			head.Serialize(buf, true);
			buf.Pack(&((T &)(elem.second)), 1);
		}
	}
	else
	{
		int size = 0;
		buf.UnPack(&size,1);
		records.clear();
		for(int i=0 ; i < size ; i++)
		{
			RecordHeader key;
			T value{};
			key.Serialize(buf, false);
			buf.UnPack(&value,1);
			records[key] = value;
		}
	}
}

void POPSensorData::Serialize(POPBuffer &buf, bool pack)
{
	 serialize(dataDouble, buf, pack);
	 serialize(dataInt,    buf, pack);
	 serialize(dataString, buf, pack);

	 // cout << "serialize "<<pack << " " << dataDouble.size() << popcendl;
}

void POPSensorData::Print() const
{
	for(auto elem : dataDouble)
	{
		cout << elem.first << " -- " << elem.second << popcendl;
	}

	// Print int data
	for(auto elem : dataInt)
	{
		cout << elem.first << " -- " << elem.second << popcendl;
	}

	// Print string data
	for(auto elem : dataString)
	{
		cout << elem.first << " -- " << elem.second << popcendl;
	}
}

void POPSensorData::PrintToFile(ostream& xr_ostream) const
{
	for(auto elem : dataDouble)
	{
		xr_ostream << elem.first << ", " << elem.second << endl;
	}

	// Print int data
	for(auto elem : dataInt)
	{
		xr_ostream << elem.first << ", " << elem.second << endl;
	}

	// Print string data
	for(auto elem : dataString)
	{
		xr_ostream << elem.first << ", " << elem.second << endl;
	}
}

void POPSensorData::ReadFromFile(istream& xr_istream)
{
	string line;

	dataInt.clear();    // Note: limitation: for now, only double is read since we cannot really differenciate int and double
	dataDouble.clear();
	dataString.clear();

	// Read line containing titles
	assert(getline(xr_istream,line));

	// Read each line
	while(getline(xr_istream,line))
	{
		unsigned int timeStamp; // TODO: unsigned long ?
		RecordHeader rec;
		stringstream lineStream(line);
		string       cell;

		getline(lineStream,cell,',');
		rec.timeStamp = atof(cell.c_str());

		getline(lineStream,cell,',');
		rec.id = atoi(cell.c_str());

		getline(lineStream,cell,',');
		rec.measurementType = translateMeasurementType(cell.c_str() + 1);

		getline(lineStream,cell,',');
		rec.unit = translateMeasurementUnit(cell.c_str() + 1);
		
		getline(lineStream,cell,',');
		dataDouble[rec] = atof(cell.c_str());
	}
}


void POPSensorData::PrintToPlot(const string& x_fileName) const
{
	// copy html header to target
	stringstream ss1;
	ss1 << "cat plots/index.head.html > " << x_fileName;
	system(ss1.str().c_str());

	ofstream of(x_fileName.c_str(), fstream::app);

	enum MeasurementType mt = MSR_HUMIDITY;

	of << "'" << explainMeasurementType(mt) << "': {\nlabel: '" << explainMeasurementType(mt) << "',\ndata: [\n";
	unsigned long initialTime = 0;
	
	for(const auto& elem : dataDouble)
	{
		if(elem.first.measurementType == mt && elem.first.id == 199)
		{
			if(initialTime == 0)
				initialTime = elem.first.timeStamp;
			// cout << "[" << elem.first.measurementType << "," << elem.first.id << "], " << popcendl;
			of << "[" << (elem.first.timeStamp - initialTime) / 3600000 << "," << elem.second << "], ";
		}
	}

	of << "]}\n";

	of.close();

	stringstream ss2;
	ss2 << "cat plots/index.foot.html >> " << x_fileName;
	system(ss2.str().c_str());
}


void POPSensorData::Clear()
{
	dataString.clear();
	dataInt.clear();
	dataDouble.clear();
}

int POPSensorData::GetSize() const
{
	return dataString.size() + dataInt.size() + dataDouble.size();
}

void POPSensorData::Insert(const POPSensorData& xr_popSensorData)
{
	dataDouble.insert(xr_popSensorData.dataDouble.begin(), xr_popSensorData.dataDouble.end());
	dataInt.insert(xr_popSensorData.dataInt.begin(), xr_popSensorData.dataInt.end());
	dataString.insert(xr_popSensorData.dataString.begin(), xr_popSensorData.dataString.end());
}


