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
#include <time.h>

#include <pop_object.h>
#include <pop_exception.h>

#include <json/json.h>
#include <curl/curl.h>

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
		ss << "INSERT INTO popwin_schema.POPSensorData(type,genre,location,sensorID,value,unit, timestamp) VALUES('"
				<< explainDataType(msg->dataType) << "','"
				<< explainMeasurementType(msg->measurementType) << "','"
				<< msg->location << "','"
				<< msg->id << "','"
				<< msg->data << "','"
				<< explainMeasurementUnit(msg->unit) << "','"
				<< msg->timestampData << "')";
		std::string s = ss.str();
		cout << s << popcendl;
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

/// @brief Read the callback message from curl request
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/// @brief Insert a message from sensor network to the database
/// @param msg  Message received from sensor network with data like temperature, location, ...
void SensorProxy::InsertKAFKA(struct NotifyMessage* msg)
{
	// KAFKA - Create JSON
	Json::Value kafka_json;   
	kafka_json["timestamp"] = msg->timestampData;
	kafka_json["sensor-id"] = msg->id;
	kafka_json["value"] = msg->data;

	// Take the correct token for sensor
	switch(msg->id)
	{
		case 1: {
			kafka_json["token"] = "5b9e6a693c6b9c84bc8a2bfef61b5480c35b2d0786ab58829fa651f2e5633c42";
			break;
		}
		case 2: {
			kafka_json["token"] = "f034f33c493e0e04e826a3d6d67ba317d41186e280f77d54658546181febc1e8";
			break;
		}
		case 3: {
			kafka_json["token"] = "311115d1ef6ecba3786bb6195f00e123aafb98c9c8f981e40f26a8433fe21223";
			break;
		}
		case 4: {
			kafka_json["token"] = "69f0bf044f5c2d7e8655a37f31d9566a15d6fa3a6e3afb6a1c7bd6532025c4e4";
			break;
		}
		case 5: {
			kafka_json["token"] = "1e407bb9462fde71b722f2108d542650ebf462c05be3006b6e16efa504fabdc0";
			break;
		}
		case 6: {
			kafka_json["token"] = "a61a588dbf2fda363bc97a21a44c7dbde22c54b6a9ba8b40b6040e46839cb847";
			break;
		}
		case 7: {
			kafka_json["token"] = "6249f8c1f92a9d72cf7507a15c024d56def0c6de2683c2dcc740584c2873bb06";
			break;
		}
		case 8: {
			kafka_json["token"] = "0420235dd66a60878b9c92b271def023cac07498fa792114076c40f5eb517f8d";
			break;
		}
		case 9: {
			kafka_json["token"] = "9b8fd13f9ec69bb7200a2ba986e0742ea3b0b0737f6406a3c6dd69122b8dbdd5";
			break;
		}
		case 10: {
			kafka_json["token"] = "d5a4129d4df5240621a74ad29db3bcc89175ef48e937c2248e8afcc79d175170";
			break;
		}
		case 11: {
			kafka_json["token"] = "34d98906d27c98c03b5615fa86ec83884828056039ff2ab70918394cb26ccb3b";
			break;
		}
		case 12: {
			kafka_json["token"] = "a01ea52f4f53e76ea0b2af5b5a3891461b1f88e5ac83d4828f41ba83e714da32";
			break;
		}
		case 13: {
			kafka_json["token"] = "f9e9a4bacae574d66c72090b80369e630c063067ab18bd44b486d986ad3abca6";
			break;
		}
		case 14: {
			kafka_json["token"] = "e3bbfa95bee3d6f8616eeb8c8f9c2aa2a165a10280f3a68a591ef257bf9f4ec1";
			break;
		}
		case 15: {
			kafka_json["token"] = "dfefed92c173a0da536079c75f76ac2c01696e16c06e336144d1592159d678ff";
			break;
		}
		case 16: {
			kafka_json["token"] = "185cc1faaedd8e0ab68aea0e9f854eb17b06c52fedee1bcceb34b6a22c323020";
			break;
		}
		case 17: {
			kafka_json["token"] = "00b6d6fd33c48ea8b79354aaee7a0875ca4aa58ecf63ef344b72de8c4794394a";
			break;
		}
		case 18: {
			kafka_json["token"] = "a1a35baed73a6beab58ae21b8c8dff23632d1938b40f0e6dbaf4e549ab4f671b";
			break;
		}
		case 19: {
			kafka_json["token"] = "43dde717264de3ae2f1d1ef44b46a9850ac7480fb01e130f998c459e202086b0";
			break;
		}
		case 20: {
			kafka_json["token"] = "a7a685527cbd9520b66a7a5b03f384af3021e5a731d47ab1c36b84b625a07d00";
			break;
		}
		case 21: {
			kafka_json["token"] = "0a57e1783db6c544b3e57081217c5bdb7cdb1a15c43c573918e16e56a4d908c4";
			break;
		}
		case 22: {
			kafka_json["token"] = "f497a250c9a0b4e77f9d078f2850442e3618320b84e4fc4feec80bbca6f67684";
			break;
		}
		case 23: {
			kafka_json["token"] = "74e34962e7dade9b01b300b3fe243547159b928065560452fc002d8ade528786";
			break;
		}
	}



	// Create string from json object
	Json::FastWriter fastWriter;
	std::string output = fastWriter.write(kafka_json);
	std::string err_tag = "errors";

	CURL *curl;
	CURLcode res;

	// Create string for callback
	std::string readBuffer;

	struct curl_slist *headers=NULL; // init to NULL is important

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");

	// get a curl handle
	curl = curl_easy_init();
	if(curl) 
	{
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */ 
		curl_easy_setopt(curl, CURLOPT_URL, "localhost:12345/input-api/measures");
		// Now specify the POST data  
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, output.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, output.length());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

	    	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	    	res = curl_easy_perform(curl);

		// Perform the request, res will get the return code
		// Check for errors
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
	      		curl_easy_strerror(res));
		} else {
			// Check if KAFKA return errors
			if (readBuffer.find(err_tag) != std::string::npos) {
			    throw POPException("Error with the data sended to KAFKA");
			} else {
				cout << "Inserted 1 input message in DB KAFKA: Value: " << msg->data << popcendl;
			}
		}
		// always cleanup 
		curl_easy_cleanup(curl);

	}
	curl_global_cleanup();
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

			// Variables to get timestamp
			char buffer[45];
			time_t curtime;
			struct tm *loctime;

			// Get the current time.
			curtime = time (NULL);

			// Convert it to local time representation.
			loctime = localtime (&curtime);

			// Format RFC3339
			strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S%z", loctime);
			strcpy(buffer+22, ":00");

			// Add timestamp to msg
			strcpy(msg.timestampData,buffer);

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
			InsertKAFKA(&msg);

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
