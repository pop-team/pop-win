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

#include <iostream>
#include <string>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/datatype.h>

#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <list>
#include <vector>

using namespace std;

/// A serializable object that can store the results of data acquisition
class POPSensorData : public POPBase
{
public:
	POPSensorData();
	virtual ~POPSensorData();

	void insert(map< string,boost::variant< int, float, double, std::string > > newMap);

	void insertColName(string colName);
	void insertColType(int colType);

	void first();
	bool next();
	int getRow();
	void clear();
	void printAll();

	int getInt(string columnLabel);
	int getInt(int columnIndex);

	float getFloat(string columnLabel);
	float getFloat(int columnIndex);

	double getDouble(string columnLabel);
	double getDouble(int columnIndex);

	string getString(string columnLabel);
	string getString(int columnIndex);

	void Serialize(POPBuffer &buf, bool pack);

private:

	list< map< string, boost::variant< int, float, double, std::string > > > databaseValues;
	list< map< string, boost::variant< int, float, double, std::string > > >::iterator list_iter;
	map< string,boost::variant< int, float, double, std::string > >::iterator map_iter;
	string colNames[20];
	int colTypes[20];
	int colNameSize;
	int colTypeSize;
	bool firstNextCall;

	string center(const string s, const int w);

};


#endif
