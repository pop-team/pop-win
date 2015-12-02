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

/// Constructor
POPSensorData::POPSensorData()
{
	list_iter = databaseValues.begin();
	colNameSize = 0;
	colTypeSize = 0;
	firstNextCall = true;
}

/// Destructor
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

/// Center-aligns string within a field of width w.
/// Pads with blank spaces to enforce alignment.
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

/// Print all data received in a table format.
void POPSensorData::printAll()
{
	string output = "|";
	cout << popcendl;
	for(int i = 0;i<colTypeSize;i++)
	{
		output+= center(colNames[i],15) + " | ";
	}
	cout << output << popcendl;
	cout << string(15*colTypeSize + colTypeSize*3, '-') << popcendl;
	first();
	while (next()) {
		output = "|";
		for(int i = 0;i<colTypeSize;i++)
		{
			output+= center(getString(colNames[i]),15) + " | ";
		}
		cout << output << popcendl;
	}
	cout << popcendl;
}

/// Insert column name (value, id...) into the list
void POPSensorData::insertColName(string colName)
{
	colNames[colNameSize++] = colName;
}

/// Insert column type (float, double, string...) into the list
void POPSensorData::insertColType(int colType)
{
	colTypes[colTypeSize++] = colType;
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
	if(!firstNextCall) {
		list_iter++;
	}
	else {
		firstNextCall = false;
	}
	if(list_iter == databaseValues.end()) {
		return false;
	}
	else {
		return true;
	}
}

int POPSensorData::getInt(string columnLabel)
{
	map_iter = list_iter->find(columnLabel);
	if(int* val = boost::get<int>(&(map_iter->second)))
	{
		return *val;
	}
	else if(string* val = boost::get<string>(&(map_iter->second)))
	{
		int defaultInt = 0;
		try
		{
			return boost::lexical_cast<int>(*val);
		} catch(const boost::bad_lexical_cast &)
		{
			fprintf(stderr,"Warning: cannot cast \"%s\" from string to int.\n",val->c_str());
			return defaultInt;
		}
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
	else if(string* val = boost::get<string>(&(map_iter->second)))
	{
		try
		{
			return boost::lexical_cast<float>(*val);
		} catch(const boost::bad_lexical_cast &)
		{
			fprintf(stderr,"Warning: cannot cast \"%s\" from string to float.\n",val->c_str());
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
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
	else if(string* val = boost::get<string>(&(map_iter->second)))
	{
		double defaultDouble = 0.0;
		try
		{
			return boost::lexical_cast<double>(*val);
		} catch(const boost::bad_lexical_cast &)
		{
			fprintf(stderr,"Warning: cannot cast \"%s\" from string to double.\n",val->c_str());
			return defaultDouble;
		}
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
		buf.Pack(&colNameSize,1);
		for(int i = 0;i<colNameSize;i++)
		{
			int size = colNames[i].length()+1; // length() doesn't take in account the null terminating char, careful !
			buf.Pack(&size,1);
			buf.Pack(colNames[i].c_str(),size);
		}
		buf.Pack(&colTypeSize,1);
		buf.Pack(colTypes,colTypeSize);
		if(colNameSize != colTypeSize)
		{
			cout << "Error in matching colName with colType size, aborting..." << popcendl;
			exit(1);
		}
		for(list_iter = databaseValues.begin(); list_iter != databaseValues.end(); ++list_iter)
		{
			for(int i = 0;i<colTypeSize;i++)
			{
				map_iter = list_iter->find(colNames[i]);

				int crtType = colTypes[i];
				if(crtType == sql::DataType::INTEGER)
				{
					//buf.Pack(&crtType,1);
					if(int* val = boost::get<int>(&(map_iter->second)))
					{
						//cout << "Packing value|colName|colType " << *val << "|" << colNames[i] << "|" << crtType << popcendl;
						buf.Pack(val,1);
					}
					else
					{
						int empty = 0;
						//cout << "Packing empty sensorIDValue " << empty << popcendl;
						buf.Pack(&empty,1);
					}
				}
				// DataType float doesn't exist, even if FLOAT is available in database. REAL has the same value (7)
				else if(crtType == sql::DataType::REAL)
				{
					//buf.Pack(&crtType,1);
					if(float* val = boost::get<float>(&(map_iter->second)))
					{
						//cout << "Packing value|colName|colType " << *val << "|" << colNames[i] << "|" << crtType << popcendl;
						buf.Pack(val,1);
					}
					else
					{
						float empty = 0.0f;
						//cout << "Packing value|colName|colType " << empty << "|" << colNames[i] << "|" << crtType << popcendl;
						buf.Pack(&empty,1);
					}
				}
				else if(crtType == sql::DataType::DOUBLE)
				{
					//buf.Pack(&crtType,1);
					if(double* val = boost::get<double>(&(map_iter->second)))
					{
						//cout << "Packing value|colName|colType " << *val << "|" << colNames[i] << "|" << crtType << popcendl;
						buf.Pack(val,1);
					}
					else
					{
						double empty = 0.0;
						//cout << "Packing empty sensorIDValue " << empty << popcendl;
						buf.Pack(&empty,1);
					}
				}
				else if(crtType == sql::DataType::VARCHAR)
				{
					//buf.Pack(&crtType,1);
					if(string* val = boost::get<string>(&(map_iter->second)))
					{
						//cout << "Packing value|colName|colType " << *val << "|" << colNames[i] << "|" << crtType << popcendl;
						buf.Pack(val,1);
					}
					else
					{
						string empty = "";
						buf.Pack(&empty,1);
					}
				}
			}

		}
		cout << "Finished packing" << popcendl;
	}
	else
	{

		int size = 0;
		buf.UnPack(&size,1);
		buf.UnPack(&colNameSize,1);
		for(int i = 0;i<colNameSize;i++)
		{
			int charSize;
			buf.UnPack(&charSize,1);
			char* crtName = new char[charSize];
			buf.UnPack(crtName,charSize);
			colNames[i] = string(crtName);
		}
		buf.UnPack(&colTypeSize,1);
		buf.UnPack(colTypes,colTypeSize);
		databaseValues.clear();
		for(int i=0 ; i < size ; i++)
		{
			map< string,boost::variant< int, float, double, std::string > > unPackMap;

			for(int i = 0;i<colTypeSize;i++)
			{
				string crtName = colNames[i];
				int crtType = colTypes[i];
				if(crtType == sql::DataType::INTEGER)
				{
					//buf.Pack(&crtType,1);
					int val;
					buf.UnPack(&val,1);
					unPackMap[crtName] = boost::variant< int, float, double, std::string >(val);
					//cout << "UnPacking value|colName|colType " << val << "|" << crtName << "|" << crtType << popcendl;
				}
				// DataType float doesn't exist, even if FLOAT is available in database. REAL has the same value (7)
				else if(crtType == sql::DataType::REAL)
				{
					//buf.Pack(&crtType,1);
					float val;
					buf.UnPack(&val,1);
					unPackMap[crtName] = boost::variant< int, float, double, std::string >(val);
					//cout << "UnPacking value|colName|colType " << val << "|" << crtName << "|" << crtType << popcendl;
				}
				else if(crtType == sql::DataType::DOUBLE)
				{
					//buf.Pack(&crtType,1);
					double val;
					buf.UnPack(&val,1);
					unPackMap[crtName] = boost::variant< int, float, double, std::string >(val);
					//cout << "UnPacking value|colName|colType " << val << "|" << crtName << "|" << crtType << popcendl;
				}
				else if(crtType == sql::DataType::VARCHAR)
				{
					//buf.Pack(&crtType,1);
					string val;
					buf.UnPack(&val,1);
					unPackMap[crtName] = boost::variant< int, float, double, std::string >(val);
					//cout << "UnPacking value|colName|colType " << val << "|" << crtName << "|" << crtType << popcendl;
				}
			}

			databaseValues.push_back(unPackMap);
			unPackMap.clear();
		}
		cout << "Finished unpacking" << popcendl;
	}
}
