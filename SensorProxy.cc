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

using namespace std;

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

SensorProxy::~SensorProxy()
{
	// Destroy object
	cout<<"Destroy sensor proxy." << popcendl;
	close(m_fd);
	// m_debugOf.close();
}

void SensorProxy::SendRawData(const std::string& x_data)
{
	// m_debugOf << x_data; m_debugOf.flush();
	string data = x_data + "\n";
	const char* data = data.c_str();
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
				HandleIncomingMessage(msg);
			}
		}
		else
		{
			printf("Received an empty message\n");
		}
	}
}

void SensorProxy::StopListening()
{
	m_listening.store(false);
}

POPSensorData SensorProxy::Gather()
{
	// cout << "Retrieve " << m_doubleData.size() << " records of type double" <<popcendl;
	return m_sensorData;
}


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
			case MSG_SUBSCRIBE:
			{
				SubscribeMessage msg;
				if(unbufferizeSubscribeMessage(&msg, x_rawMsg.c_str()) <= 0)
					throw POPException("Cannot unbufferize subscribe message");

				throw POPException("Subscribtion messages not handled yet"); //TODO

				// Define here what to do on reception 
				// ...
				
			}

			break;
			case MSG_NOTIFY:
			{
				NotifyMessage msg;
				if(unbufferizeNotifyMessage(&msg, x_rawMsg.c_str(), x_rawMsg.size()) <= 0)
					throw POPException("Cannot unbufferize notify message");

				auto it = m_subscriptions.find(msg.measurementType);
				bool subscibed = it != m_subscriptions.end() && it->second;
				if(!subscibed)
				{
					// Not subscribed to this type of data
					cout<< "Unstored notification (" << explainMeasurementType(msg.measurementType) << "): '" << msg.data << "'" << popcendl;
					break;
				}
				cout<< "Stored notification   (" << explainMeasurementType(msg.measurementType) << "): '" << msg.data << "'" << popcendl;
				switch(msg.dataType)
				{
					case TYPE_DOUBLE:
						m_sensorData.dataDouble.insert(std::pair<RecordHeader, double>(RecordHeader(ms, msg), atof(msg.data))); 
						break;
					case TYPE_INT:
						m_sensorData.dataInt.insert(std::pair<RecordHeader, int>(RecordHeader(ms, msg), atoi(msg.data))); 
						break;
					case TYPE_STRING:
						m_sensorData.dataString.insert(std::pair<RecordHeader, std::string>(RecordHeader(ms, msg), std::string(msg.data))); 
						break;
					default:
						printf("Unknown data type %d in %s\n", msg.dataType, x_rawMsg.c_str());
				}
			}
			break;
			default:
				// Unknown message: print
				cout << "Received raw message: '" << x_rawMsg << "'" << popcendl;
		}
	}
	catch(std::exception &e)
	{
		cout << "Error at reception of message '" << x_rawMsg << "': " << e.what() << popcendl;
	}
	
}



void SensorProxy::Notify(int x_measurementType, int x_measurementUnit, const std::string& x_message)
{
	// note: only string messages are implemented. Other types can be trivially implemented
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
		throw POPException("Cannot bufferize publish message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

void SensorProxy::Publish(int x_publicationType, int x_data)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	sprintf(msg.data, "%d", x_data);
	msg.publicationType = static_cast<PublicationType>(x_publicationType);
	msg.dataType        = TYPE_INT;
	// msg.unit            = UNT_UNKNOWN;
	msg.id              = m_id;
	msg.dataSize        = strlen(msg.data);

	char buffer[BUFFERSIZE];
	if(bufferizePublishMessage(&msg, buffer, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message", buffer);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buffer);
}

void SensorProxy::Subscribe(int x_measurementType, int x_dataType)
{
	char buf[BUFFERSIZE];
	// cout << "subscribe to "<< x_measurementType << endl;
	m_subscriptions[x_measurementType] = true;

	struct SubscribeMessage msg;
	memset(&msg, 0, sizeof(struct SubscribeMessage));
	msg.measurementType = static_cast<MeasurementType>(x_measurementType);
	msg.dataType        = static_cast<DataType>(x_dataType);
	msg.id              = m_id;

	if(bufferizeSubscribeMessage(&msg, buf, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message");
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buf);
}

@pack(SensorProxy);
