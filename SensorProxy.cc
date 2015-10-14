/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
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

// Note: check baudrate of USB port with : stty -F /dev/ttyUSB0
#define BAUDRATE B115200

//#define EN_COUTS 1 // enable cout to std out, comment to disable

using namespace std;

/// Constructor
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
}

/// Destructur
SensorProxy::~SensorProxy()
{
	// Destroy object
	cout<<"Destroy sensor proxy" << popcendl;
	close(m_fd);
	// m_debugOf.close();
}

/// Send raw data to the gateway
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

/// Read data coming from the gateway
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

/// Start listening to messages coming from sensors
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

/// Stop listening to messages coming from sensors
void SensorProxy::StopListening()
{
	m_listening.store(false);
}

/// Return a POPSensorData structure containing the messages received from sensors
POPSensorData SensorProxy::Gather()
{
	// cout << "Retrieve " << m_doubleData.size() << " records of type double" <<popcendl;
	return m_sensorData;
}

/// Clear the stored messages
void SensorProxy::Clear()
{
	m_sensorData.Clear();
	// m_subscriptions.clear();
}

/// Handle all incoming messages
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
				#ifdef EN_COUTS
				cout << "Proxy received log from " << msg.id << " : " << msg.data << popcendl;
				#endif
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
			switch(msg.dataType)
			{
			case TYPE_DOUBLE:
			{
				std::pair<RecordHeader, double> pair(RecordHeader(ms, msg), atof(msg.data));
				m_sensorData.Insert(pair);
				break;
			}
			case TYPE_INT:
			{
				std::pair<RecordHeader, int> pair(RecordHeader(ms, msg), atoi(msg.data));
				m_sensorData.Insert(pair);
				break;
			}
			case TYPE_STRING:
			{
				std::pair<RecordHeader, std::string> pair(RecordHeader(ms, msg), std::string(msg.data));
				m_sensorData.Insert(pair);
				break;
			}
			default:
				printf("Unknown data type %d in %s\n", msg.dataType, x_rawMsg.c_str());
			}
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

/// Send a notification to the gateway
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

/// Send a notification to the gateway
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

/// Send a notification to the gateway
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

/// Send a publication to the gateway
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

/// Send a publication to the gateway
/*void SensorProxy::Publish(int x_publicationType, double x_data)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	//sprintf(msg.data, "%lf", x_data);
	msg.publicationType = static_cast<PublicationType>(x_publicationType);
	//msg.dataType        = TYPE_DOUBLE;
	msg.id              = m_id;
	//msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizePublishMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}*/

/// Send a publication to the gateway
/*void SensorProxy::Publish(int x_publicationType, const string& x_data)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	//sprintf(msg.data, "%s", x_data.c_str());
	msg.publicationType = static_cast<PublicationType>(x_publicationType);
	//msg.dataType        = TYPE_STRING;
	// msg.unit            = UNT_UNKNOWN;
	msg.id              = m_id;
	//msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizePublishMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}*/

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

/// Send a subscription to the gateway
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

/// Send a subscription to the gateway
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

/// Return the size of the stored data
int SensorProxy::GetDataSize()
{
	return m_sensorData.GetSize();
}

/// Apply a reduce operation to the stored data {size, min, max, aver, sum, stdev}
double SensorProxy::Reduce(int x_mtype, int x_dataType, int x_fct)
{
	switch(x_dataType)
	{
	case TYPE_INT:
		return m_sensorData.Reduce<int>(static_cast<enum MeasurementType>(x_mtype), static_cast<POPSensorData::POPReduceF>(x_fct));
	case TYPE_DOUBLE:
		return m_sensorData.Reduce<int>(static_cast<enum MeasurementType>(x_mtype), static_cast<POPSensorData::POPReduceF>(x_fct));
	default:
		throw POPException("No reduce operation for type " + string(explainDataType(static_cast<enum DataType>(x_dataType))));
	}
}

/// Send a notification to set the gwID GW as a GW
void SensorProxy::SetAsGateway(int gwID)
{
	Notify(MSR_SET_GW, UNT_NONE, gwID);
}

@pack(SensorProxy);
