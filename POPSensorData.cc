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

using namespace std;

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

void POPSensorData::Print()
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

void POPSensorData::PrintToFile(ostream& xr_ostream)
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

void POPSensorData::Clear()
{
	dataString.clear();
	dataInt.clear();
	dataDouble.clear();
}
