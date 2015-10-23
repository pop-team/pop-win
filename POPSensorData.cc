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


POPSensorData::POPSensorData()
{
	list_iter = databaseValues.begin();
	colNameSize = 0;
}

POPSensorData::~POPSensorData()
{
	databaseValues.clear();
}

/// Insert data into the list
void POPSensorData::insert(map< string,boost::variant< int, float, double, std::string > > newMap)
{
	databaseValues.push_back(newMap);
}

/// Insert data into the list
void POPSensorData::insertColName(string colName)
{
	colNames[colNameSize++] = colName;
}

/// make itnernal iterator point to first row again
void POPSensorData::first()
{
	list_iter = databaseValues.begin();
}

/// Iterate through all rows of the table of data
bool POPSensorData::next()
{
	list_iter++;
	if(list_iter == databaseValues.end())
	{
		return false;
	}
	else
		return true;
}

int POPSensorData::getInt(string columnLabel)
{
	map_iter = list_iter->find(columnLabel);
	if(int* val = boost::get<int>(&(map_iter->second)))
	{
		return *val;
	}
	else return 0;
}

int POPSensorData::getInt(int columnIndex)
{
	return getInt(colNames[columnIndex-1]);
}

string POPSensorData::getString(string columnLabel)
{
	map_iter = list_iter->find(columnLabel);
	if(string* val = boost::get<string>(&(map_iter->second)))
	{
		return *val;
	}
	else if(int* val = boost::get<int>(&(map_iter->second)))
	{
		return boost::lexical_cast<std::string>(*val);
	}
	else if(float* val = boost::get<float>(&(map_iter->second)))
	{
		return boost::lexical_cast<std::string>(*val);
	}
	else if(double* val = boost::get<double>(&(map_iter->second)))
	{
		return boost::lexical_cast<std::string>(*val);
	}
	return "";
}

string POPSensorData::getString(int columnIndex)
{
	return getString(colNames[columnIndex-1]);
}

float POPSensorData::getFloat(string columnLabel)
{
	map_iter = list_iter->find(columnLabel);
	if(float* val = boost::get<float>(&(map_iter->second)))
	{
		return *val;
	}
	else return 0.0f;
}

float POPSensorData::getFloat(int columnIndex)
{
	return getFloat(colNames[columnIndex-1]);
}

double POPSensorData::getDouble(string columnLabel)
{
	map_iter = list_iter->find(columnLabel);
	if(double* val = boost::get<double>(&(map_iter->second)))
	{
		return *val;
	}
	else return 0.0;
}

double POPSensorData::getDouble(int columnIndex)
{
	return getDouble(colNames[columnIndex-1]);
}

void POPSensorData::Serialize(POPBuffer &buf, bool pack)
{
	if(pack)
	{
		int size = databaseValues.size();
		buf.Pack(&size, 1);
		cout << "Packing size:" << size << popcendl;
		list_iter = databaseValues.begin();
		cout << "list_iter is begin()" << popcendl;
		cout << "list_iter is already end(): " << (list_iter == databaseValues.end()) << popcendl;
		while(list_iter != databaseValues.end())
		{
			map_iter = list_iter->begin();
			//string typeKey = map_iter->first;
			//buf.Pack(&typeKey,1);
			if(string* typeValue = boost::get<string>(&(map_iter->second)))
			{
				buf.Pack(typeValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}
			map_iter++;

			//string genreKey = map_iter->first;
			//buf.Pack(&genreKey,1);
			if(string* genreValue = boost::get<string>(&(map_iter->second)))
			{
				buf.Pack(genreValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}
			map_iter++;

			//string locationKey = map_iter->first;
			//buf.Pack(&locationKey,1);
			if(string* locationValue = boost::get<string>(&(map_iter->second)))
			{
				buf.Pack(locationValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}
			map_iter++;

			//string unitKey = map_iter->first;
			//buf.Pack(&unitKey,1);
			if(string* unitValue = boost::get<string>(&(map_iter->second)))
			{
				buf.Pack(unitValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}
			map_iter++;

			//string sensorIDKey = map_iter->first;
			//buf.Pack(&sensorIDKey,1);
			if(int* sensorIDValue = boost::get<int>(&(map_iter->second)))
			{
				buf.Pack(sensorIDValue,1);
			}
			else
			{
				int empty = 0;
				buf.Pack(&empty,1);
			}
			map_iter++;

			//string valueKey = map_iter->first;
			//buf.Pack(&valueKey,1);
			// find what type of data is value
			if(string* val = boost::get<string>(&(map_iter->second)))
			{
				uint8_t PODType = 0;
				buf.Pack(&PODType,1);
				buf.Pack(val,1);
			}
			else if(int* val = boost::get<int>(&(map_iter->second)))
			{
				uint8_t PODType = 1;
				buf.Pack(&PODType,1);
				buf.Pack(val,1);
			}
			else if(float* val = boost::get<float>(&(map_iter->second)))
			{
				uint8_t PODType = 2;
				buf.Pack(&PODType,1);
				buf.Pack(val,1);
			}
			else if(double* val = boost::get<double>(&(map_iter->second)))
			{
				uint8_t PODType = 3;
				buf.Pack(&PODType,1);
				buf.Pack(val,1);
			}
			map_iter++;
			list_iter++;

		}
		cout << "Finished packing" << popcendl;
	}
	else
	{
		int size = 0;
		buf.UnPack(&size,1);
		cout << "UnPacking size:" << size << popcendl;
		databaseValues.clear();
		for(int i=0 ; i < size ; i++)
		{
			map< string,boost::variant< int, float, double, std::string > > unPackMap;

			string typeValue;
			buf.UnPack(&typeValue,1);
			unPackMap["type"] = boost::variant< int, float, double, std::string >(typeValue);

			string genreValue;
			buf.UnPack(&genreValue,1);
			unPackMap["genre"] = boost::variant< int, float, double, std::string >(genreValue);

			string locationValue;
			buf.UnPack(&locationValue,1);
			unPackMap["location"] = boost::variant< int, float, double, std::string >(locationValue);

			string unitValue;
			buf.UnPack(&unitValue,1);
			unPackMap["unit"] = boost::variant< int, float, double, std::string >(unitValue);

			int sensorIDValue;
			buf.UnPack(&sensorIDValue,1);
			unPackMap["sensorID"] = boost::variant< int, float, double, std::string >(sensorIDValue);

			uint8_t PODType;
			buf.UnPack(&PODType,1);
			if(PODType == 0) // string
			{
				string valueValue;
				buf.UnPack(&valueValue,1);
				unPackMap["value"] = boost::variant< int, float, double, std::string >(valueValue);
			}
			else if(PODType == 1) // int
			{
				int valueValue;
				buf.UnPack(&valueValue,1);
				unPackMap["value"] = boost::variant< int, float, double, std::string >(valueValue);
			}
			else if(PODType == 2) // float
			{
				float valueValue;
				buf.UnPack(&valueValue,1);
				unPackMap["value"] = boost::variant< int, float, double, std::string >(valueValue);
			}
			else if(PODType == 3) // double
			{
				double valueValue;
				buf.UnPack(&valueValue,1);
				unPackMap["value"] = boost::variant< int, float, double, std::string >(valueValue);
			}

			databaseValues.push_back(unPackMap);
			unPackMap.clear();
		}
		cout << "Finished unpacking" << popcendl;
	}
}
