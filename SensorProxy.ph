/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @author Marco Lourenço
 * @date   November 2015
 * @brief Proxy class that handles communication with one sensor but resides on the same machine as the gateway. Each proxy is connected to one sensor on the network.
 *
 *
 */

#ifndef SENSOR_PROXY_H
#define SENSOR_PROXY_H

#include <atomic>
#include <fstream>


#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "POPSensor.ph"
#include "lineComm/popwin_messages.h"


parclass SensorProxy {
	classuid(1903);

public:
	SensorProxy(int x_id, const std::string& x_url, const std::string& x_device) @{ od.url(x_url); };
	~SensorProxy();

	async seq void Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message);
	async seq void Notify(int x_measurementType, int x_measurementUnit, double x_data);
	async seq void Notify(int x_measurementType, int x_measurementUnit, int x_data);

	async seq void Publish(int x_measurementType);

	async seq void UnPublish(int x_measurementType);

	async seq void Subscribe(int x_measurementType, int x_dataType);

	async seq void UnSubscribe(int x_measurementType, int x_dataType);

	async seq void SetAsGateway(int gwID);

	async conc void StartListening();
	async conc void StopListening();

private:
	void SendRawData(const std::string& x_data);
	void ReadData(std::ostream& xr_ostream);
	void HandleIncomingMessage(const std::string& x_msg);

	void InsertSQL(struct NotifyMessage* msg);

	int m_fd;
	int m_id;
	std::atomic<bool>  m_listening;
	std::map<int,bool> m_subscriptions;
	std::map<int,bool> m_publications;

	sql::Driver *driver;
	sql::Connection *con;
};

#endif
