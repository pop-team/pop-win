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

/*!
 * \brief A serializable object that can store the results of data acquisition
 *
 * This class is used to encapsulate the data returned by a user SQL request in a serializable way that POP-C++ framework can use to transfer data
 * across the network. Those POP-C++ compatibe object must inherit Serialize method from POPBase. POPSensorData is intended to work in a similar way
 * as the MySQL object <a href="https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-results.html" target="_blank">ResultSet</a>.
 *
 * \author Laurent Winkler based on work by Valentin Bourqui
 * \author Marco Lourenço
 * \date 2013-2015
 */
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
