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
	firstNextCall = true;
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

/// Empty the data object
void POPSensorData::clear()
{
	databaseValues.clear();
}

/// Return the number of rows
int POPSensorData::getRow()
{
	return databaseValues.size();
}

/*! Center-aligns string within a field of width w. Pads with blank spaces
    to enforce alignment. */
std::string POPSensorData::center(const string s, const int w) {
	stringstream ss, spaces;
	int padding = w - s.size();                 // count excess room to pad
	for(int i=0; i<padding/2; ++i)
		spaces << " ";
	ss << spaces.str() << s << spaces.str();    // format with padding
	if(padding>0 && padding%2!=0)               // if odd #, add 1 space
		ss << " ";
	return ss.str();
}

void POPSensorData::printAll()
{
	cout 	<< center("type",10) 	<< " | "
			<< center("genre",15) 	<< " | "
			<< center("location",15)<< " | "
			<< center("sensorID",10)<< " | "
			<< center("value",10) 	<< " | "
			<< center("unit",10) 	<< popcendl;
	cout << string(10*4+15*2 + 5*3, '-') << popcendl;
	first();
	while (next()) {
		/* Access column data by alias or column name */
		cout 	<< center(getString("type"),10) 	<< " | "
				<< center(getString("genre"),15) 	<< " | "
				<< center(getString("location"),15) << " | "
				<< center(getString("sensorID"),10) << " | "
				<< center(getString("value"),10) 	<< " | "
				<< center(getString("unit"),10) 	<< popcendl;

	}
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
	firstNextCall = true;
}

/// Iterate through all rows of the table of data
bool POPSensorData::next()
{
	if(!firstNextCall)
		list_iter++;
	else
		firstNextCall = false;
	if(list_iter == databaseValues.end())
		return false;
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
		for(list_iter = databaseValues.begin(); list_iter != databaseValues.end(); ++list_iter)
		{
			map_iter = list_iter->find("type");
			//string typeKey = map_iter->first;
			//buf.Pack(&typeKey,1);
			if(string* typeValue = boost::get<string>(&(map_iter->second)))
			{
				//cout << "Packing typeValue " << *typeValue << popcendl;
				buf.Pack(typeValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}

			map_iter = list_iter->find("genre");
			//string genreKey = map_iter->first;
			//buf.Pack(&genreKey,1);
			if(string* genreValue = boost::get<string>(&(map_iter->second)))
			{
				//cout << "Packing genreValue " << *genreValue << popcendl;
				buf.Pack(genreValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}

			map_iter = list_iter->find("location");
			//string locationKey = map_iter->first;
			//buf.Pack(&locationKey,1);
			if(string* locationValue = boost::get<string>(&(map_iter->second)))
			{
				//cout << "Packing locationValue " << *locationValue << popcendl;
				buf.Pack(locationValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}

			map_iter = list_iter->find("unit");
			//string unitKey = map_iter->first;
			//buf.Pack(&unitKey,1);
			if(string* unitValue = boost::get<string>(&(map_iter->second)))
			{
				//cout << "Packing unitValue " << *unitValue << popcendl;
				buf.Pack(unitValue,1);
			}
			else
			{
				string empty = "";
				buf.Pack(&empty,1);
			}

			map_iter = list_iter->find("sensorID");
			//string sensorIDKey = map_iter->first;
			//buf.Pack(&sensorIDKey,1);
			if(int* sensorIDValue = boost::get<int>(&(map_iter->second)))
			{
				//cout << "Packing sensorIDValue " << *sensorIDValue << popcendl;
				buf.Pack(sensorIDValue,1);
			}
			else
			{
				int empty = 0;
				//cout << "Packing empty sensorIDValue " << empty << popcendl;
				buf.Pack(&empty,1);
			}

			map_iter = list_iter->find("value");
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
				//cout << "Packing val " << *val << popcendl;
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
			++map_iter;
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
			//cout << "UnPacking typeValue " << typeValue << popcendl;
			unPackMap["type"] = boost::variant< int, float, double, std::string >(typeValue);

			string genreValue;
			buf.UnPack(&genreValue,1);
			//cout << "UnPacking genreValue " << genreValue << popcendl;
			unPackMap["genre"] = boost::variant< int, float, double, std::string >(genreValue);

			string locationValue;
			buf.UnPack(&locationValue,1);
			//cout << "UnPacking locationValue " << locationValue << popcendl;
			unPackMap["location"] = boost::variant< int, float, double, std::string >(locationValue);

			string unitValue;
			buf.UnPack(&unitValue,1);
			//cout << "UnPacking unitValue " << unitValue << popcendl;
			unPackMap["unit"] = boost::variant< int, float, double, std::string >(unitValue);

			int sensorIDValue;
			buf.UnPack(&sensorIDValue,1);
			//cout << "UnPacking sensorIDValue " << sensorIDValue << popcendl;
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
				//cout << "UnPacking valueValue " << valueValue << popcendl;
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
