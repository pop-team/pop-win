/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @author Marco Lourenço
 * @date   November 2015
 * @brief Proxy class that handles communication with one sensor but resides on the same machine as the gateway
 *
 *
 */


#include "SensorProxy.ph"
#include "POPSensor.ph"
#include <unistd.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <regex.h>

#include <pop_object.h>
#include <pop_exception.h>

// Note: check baudrate of USB port with : stty -F /dev/ttyUSB0
#define BAUDRATE B115200

//#define EN_COUTS 1 // enable cout to std out, comment to disable

using namespace std;

/// @brief Constructor
/// @param x_id  Id to set to this object (for low-level communication)
/// @param x_url URL on which this parallel object is allocated
/// @param x_device Device name e.g. /dev/ttyUSB0
SensorProxy::SensorProxy(int x_id, const std::string& x_url, const string& x_device)
{
	m_id = x_id;
	fd_set mask;
	speed_t speed = BAUDRATE;
	m_fd = open(x_device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC );

	if (m_fd <0) {
		throw POPException("Cannot open device", x_device);
	}

	if (fcntl(m_fd, F_SETFL, 0) < 0) {
		throw POPException("Could not set fcntl");
	}

	struct termios options;
	if (tcgetattr(m_fd, &options) < 0) {
		throw POPException("Could not get options");
	}

	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	/* Enable the receiver and set local mode */
	options.c_cflag |= (CLOCAL | CREAD);

	/* Mask the character size bits and turn off (odd) parity */
	options.c_cflag &= ~(CSIZE|PARENB|PARODD);

	/* Select 8 data bits */
	options.c_cflag |= CS8;

	/* Raw input */
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* Raw output */
	options.c_oflag &= ~OPOST;

	if (tcsetattr(m_fd, TCSANOW, &options) < 0) {
		throw POPException("could not set options");
	}

	FD_ZERO(&mask);
	FD_SET(m_fd, &mask);
	FD_SET(fileno(stdin), &mask);

	if(m_fd <= 0)
	{
		throw POPException("Fail to open connection to sensor", x_device);
	}

	// m_debugOf.open("debug.txt");

	// init DB connection
	/* Create a connection */
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "toor");
	/* Connect to the MySQL test database */
	//cout<<"Setting schema" << popcendl;
	//con->setSchema("popwin_schema");
}

/// @brief Destructor
SensorProxy::~SensorProxy()
{
	// Destroy object
	cout<<"Destroy sensor proxy" << popcendl;
	close(m_fd);
	// m_debugOf.close();
	delete con;
}

/// @brief Insert a message from sensor network to the database
/// @param msg  Message received from sensor network with data like temperature, location, ...
void SensorProxy::InsertSQL(struct NotifyMessage* msg)
{
	try {
		sql::Statement *stmt;

		stmt = con->createStatement();
		std::stringstream ss;
		ss << "INSERT INTO popwin_schema.POPSensorData(type,genre,location,sensorID,value,unit) VALUES('"
				<< explainDataType(msg->dataType) << "','"
				<< explainMeasurementType(msg->measurementType) << "','"
				<< msg->location << "','"
				<< msg->id << "','"
				<< msg->data << "','"
				<< explainMeasurementUnit(msg->unit) << "')";
		std::string s = ss.str();
		stmt->executeUpdate(s);

		/* Access column data by alias or column name */
		cout << "Inserted 1 input message in DB: " << msg->data << popcendl;

		delete stmt;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "	<< __LINE__ << popcendl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << popcendl;
	}
	cout << popcendl;
}

/// @brief Send raw data to the gateway
/// @param x_data the data to be sent
void SensorProxy::SendRawData(const std::string& x_data)
{
	// m_debugOf << x_data; m_debugOf.flush();
	string datan = x_data + "\n";
	const char* data = datan.c_str();
	// printf("Sending %s on %d\n", data, m_fd);
	int n = strlen(data);
	if (n <= 0) {
		throw POPException("could not read");
	}
	else if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			if (write(m_fd, &data[i], 1) <= 0)
			{
				throw POPException("Failed to write data while sending to sensor");
			}
			else
			{
				// printf("write successfull\n");
				fflush(NULL);
				// usleep(6000);
			}
		}
	}

}

/// @brief Read data coming from the gateway
/// @param xr_ostream the output stream where read data is written
void SensorProxy::ReadData(std::ostream& xr_ostream)
{
	while(m_listening.load()) {   
		char buf[BUFFERSIZE+1];
		int n = read(m_fd, buf, sizeof(buf));
		// printf("n=%d\n", n);
		/*
		for(int i = 0; i < n; i++) {
			printf("%c", buf[i]);
		}
		 */
		if(n==0)
		{
			// read(...) is not blocking. Add a sleep to reduce the charge when inactive
			usleep(10000); // sleep 10 ms
			continue;
		}
		// note: buffer should be null terminated to be concatenated to string
		buf[n] = '\0';

		// printf("%s\n", buf);
		xr_ostream << buf;

		// Carriage return means end of message
		if(buf[n-1] == '\n')
			break;
	}
}

/// @brief Start listening to messages coming from sensors
void SensorProxy::StartListening()
{
	m_listening.store(true);
	while(m_listening.load())
	{
		stringstream received;
		ReadData(received);
		if(received.str().size() != 0) // && received.str() != "(null)")
		{
			// We need to split the message with the character \n to avoid the case where
			// several messages arrive in the same call
			std::string msg;

			while(std::getline(received,msg,'\n')){
				//cout << "handling incoming message: " << msg << popcendl;
				HandleIncomingMessage(msg);
			}
		}
		else
		{
			printf("Received an empty message\n");
		}
	}
}

/// @brief Stop listening to messages coming from sensors
void SensorProxy::StopListening()
{
	m_listening.store(false);
}

/// @brief Handle all incoming messages
/// @param x_rawMsg the raw message (encoded in chars)
void SensorProxy::HandleIncomingMessage(const std::string& x_rawMsg)
{
	// cout << "received :" << x_rawMsg << popcendl;
	struct timeval time_now;
	gettimeofday(&time_now, NULL);
	long ms = time_now.tv_sec * 1000 + time_now.tv_usec / 1000;
	// cout << "ms" << ms << popcendl;


	try
	{
		switch(getMessageType(x_rawMsg.c_str()))
		{
		case MSG_PUBLISH:
		{
			/*struct PublishMessage msg;
			memset(&msg, 0, sizeof(msg));
			if(unbufferizePublishMessage(&msg, x_rawMsg.c_str(),x_rawMsg.size()) <= 0)
				throw POPException("Cannot unbufferize publish message");*/

			throw POPException("Publication messages are not handled yet");
		}
		break;
		case MSG_SUBSCRIBE:
		{
			/*struct SubscribeMessage msg;
			memset(&msg, 0, sizeof(msg));
			if(unbufferizeSubscribeMessage(&msg, x_rawMsg.c_str()) <= 0)
				throw POPException("Cannot unbufferize subscribe message");*/

			throw POPException("Subscription messages are not handled yet");
		}

		break;
		case MSG_NOTIFY:
		{
			struct NotifyMessage msg;
			memset(&msg,0,sizeof(msg));
			if(unbufferizeNotifyMessage(&msg, x_rawMsg.c_str(), x_rawMsg.size()) <= 0)
				throw POPException("Cannot unbufferize notify message");

			auto it = m_subscriptions.find(msg.measurementType);
			bool subscribed = it != m_subscriptions.end() && it->second;
			if(msg.measurementType == MSR_LOG)
			{
				break;
			}
			else if(!subscribed)
			{
				// Not subscribed to this type of data
#ifdef EN_COUTS
				cout<< "Proxy "<< m_id << ", unstored notification (" << explainMeasurementType(msg.measurementType) << "): '" << msg.data << "'" << popcendl;
#endif
				break;
			}
#ifdef EN_COUTS
			cout<< "Proxy "<< m_id << ", stored notification   (" << explainMeasurementType(msg.measurementType) << "): '" << msg.data << "'" << popcendl;
#endif
			InsertSQL(&msg);
		}
		break;

#ifdef EN_COUTS
		default:
			// Unknown message: print
			cout << "Proxy received raw message: '" << x_rawMsg << "'" << popcendl;
#endif
		}
	}
	catch(std::exception &e)
	{
		cout << "Error at reception of message '" << x_rawMsg << "': " << e.what() << popcendl;
	}

}

/// @brief Send a notification with int data to the gateway
/// @param x_measurementType the type of measurement we want to notify (temp, light, ...)
/// @param x_measurementUnit the unit of the measurement we want to notify (celsius, lumen, ...)
/// @param x_data the measurement we want to notify (32, 45, 0, ...)
void SensorProxy::Notify(int x_measurementType, int x_measurementUnit, int x_data)
{
	enum MeasurementType msgType = static_cast<MeasurementType>(x_measurementType);
	// Check publication rights
	auto it = m_publications.find(msgType);
	bool canPublish = (it != m_publications.end() && it->second);

	if(!canPublish)
	{
		// Not subscribed to this type of data
		cout<< "Cannot notify " << explainMeasurementType(msgType) << " OUT" << popcendl;
		return;
	}
	// note: only string messages are implemented. Other types can be trivially implemented
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(struct NotifyMessage));
	snprintf(msg.data, sizeof(msg.data), "%d", x_data);
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = TYPE_INT;
	msg.unit            = static_cast<MeasurementUnit>(x_measurementUnit);
	msg.id              = m_id;
	msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizeNotifyMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize notify message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

/// @brief Send a notification with double data to the gateway
/// @param x_measurementType the type of measurement we want to notify (temp, light, ...)
/// @param x_measurementUnit the unit of the measurement we want to notify (celsius, lumen, ...)
/// @param x_data the measurement we want to notify (32.4, 45.9, 0.1, ...)
void SensorProxy::Notify(int x_measurementType, int x_measurementUnit, double x_data)
{
	enum MeasurementType msgType = static_cast<MeasurementType>(x_measurementType);
	// Check publication rights
	auto it = m_publications.find(msgType);
	bool canPublish = (it != m_publications.end() && it->second);

	if(!canPublish)
	{
		// Not subscribed to this type of data
		cout<< "Cannot notify " << explainMeasurementType(msgType) << " OUT" << popcendl;
		return;
	}
	// note: only string messages are implemented. Other types can be trivially implemented
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(struct NotifyMessage));
	snprintf(msg.data, sizeof(msg.data), "%lf", x_data);
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = TYPE_DOUBLE;
	msg.unit            = static_cast<MeasurementUnit>(x_measurementUnit);
	msg.id              = m_id;
	msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizeNotifyMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize notify message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

/// @brief Send a notification with string data to the gateway
/// @param x_measurementType the type of measurement we want to notify (temp, light, ...)
/// @param x_measurementUnit the unit of the measurement we want to notify (celsius, lumen, ...)
/// @param x_data the measurement we want to notify ("hot", "cold", "wet", ...)
void SensorProxy::Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message)
{
	enum MeasurementType msgType = static_cast<MeasurementType>(x_measurementType);
	// Check publication rights
	auto it = m_publications.find(msgType);
	bool canPublish = (it != m_publications.end() && it->second);

	if(!canPublish)
	{
		// Not subscribed to this type of data
		cout<< "Cannot notify " << explainMeasurementType(msgType) << " OUT" << popcendl;
		return;
	}

	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(struct NotifyMessage));
	snprintf(msg.data, sizeof(msg.data), "%s\r\n", x_message.c_str());
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = TYPE_STRING;
	msg.unit            = static_cast<MeasurementUnit>(x_measurementUnit);
	msg.id              = m_id;
	msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizeNotifyMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize notify message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

/// @brief Send a message to tell that we start publishing
/// @param x_measurementType the type of measurement we want to start publishing (temp, light, ...)
void SensorProxy::Publish(int x_measurementType)
{
	m_publications[x_measurementType] = true;

	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	//sprintf(msg.data, "%d", x_data);
	msg.publicationType = static_cast<MeasurementType>(x_measurementType);
	//msg.dataType        = TYPE_INT;
	msg.id              = m_id;
	//msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	memset(&buffer, 0, BUFFERSIZE*sizeof(char));
	if(bufferizePublishMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message", buffer);
	//cout << "Bufferized publish to buffer size: " << strlen(buffer) << " | " << buffer << popcendl;
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

/// @brief Send a message to tell that we stop publishing
/// @param x_measurementType the type of measurement we want to stop publishing (temp, light, ...)
void SensorProxy::UnPublish(int x_measurementType)
{
	m_publications[x_measurementType] = false;

	struct UnPublishMessage msg;
	memset(&msg, 0, sizeof(struct UnPublishMessage));
	//sprintf(msg.data, "%d", x_data);
	msg.publicationType = static_cast<MeasurementType>(x_measurementType);
	//msg.dataType        = TYPE_INT;
	msg.id              = m_id;
	//msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	memset(&buffer, 0, BUFFERSIZE*sizeof(char));
	if(bufferizeUnPublishMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize unpublish message", buffer);
	//cout << "Bufferized publish to buffer size: " << strlen(buffer) << " | " << buffer << popcendl;
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

/// @brief Send a message to tell we start subscription to new data
/// @param x_measurementType the type of measurement we want to subscribe to (temp, light, ...)
/// @param x_dataType the type of data we want to subscribe to (int, double, string)
void SensorProxy::Subscribe(int x_measurementType, int x_dataType)
{
	char buf[BUFFERSIZE];
	memset(buf,0,BUFFERSIZE*sizeof(char));
	// cout << "subscribe to "<< x_measurementType << endl;
	m_subscriptions[x_measurementType] = true;

	struct SubscribeMessage msg;
	memset(&msg, 0, sizeof(struct SubscribeMessage));
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = static_cast<DataType>(x_dataType);
	msg.id              = m_id;

	// Bufferize message and send to gateway
	if(bufferizeSubscribeMessage(&msg, buf, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize subscribe message");
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buf);
}

/// @brief Send a message to tell we stop subscription to data
/// @param x_measurementType the type of measurement we want to unsubscribe from (temp, light, ...)
/// @param x_dataType the type of data we want to unsubscribe from (int, double, string)
void SensorProxy::UnSubscribe(int x_measurementType, int x_dataType)
{
	char buf[BUFFERSIZE];
	memset(buf,0,BUFFERSIZE*sizeof(char));
	// cout << "subscribe to "<< x_measurementType << endl;
	m_subscriptions[x_measurementType] = false;

	struct UnSubscribeMessage msg;
	memset(&msg, 0, sizeof(struct UnSubscribeMessage));
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = static_cast<DataType>(x_dataType);
	msg.id              = m_id;

	// Bufferize message and send to gateway
	if(bufferizeUnSubscribeMessage(&msg, buf, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize unsubscribe message");
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buf);
}

/// @brief Send a notification to transform a sensor into the gateway of the sensor network
/// @param gwID the ID of the sensor that will become gateway
void SensorProxy::SetAsGateway(int gwID)
{
	Notify(MSR_SET_GW, UNT_NONE, gwID);
}

@pack(SensorProxy);
