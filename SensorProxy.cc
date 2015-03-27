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
/*
#define BAUDRATE_S "115200"
#ifdef linux
#define MODEMDEVICE "/dev/ttyUSB0"
#define MODEMDEVICETWO "/dev/ttyS0"
#else
#define MODEMDEVICE "/dev/com1"
#endif 

#define SLIP_END     0300
#define SLIP_ESC     0333
#define SLIP_ESC_END 0334
#define SLIP_ESC_ESC 0335

#define CSNA_INIT 0x01

#define BUFSIZE 64
#define HCOLS 20
#define ICOLS 18


#define MODE_START_DATE	0
#define MODE_DATE	1
#define MODE_START_TEXT	2
#define MODE_TEXT	3
#define MODE_INT	4
#define MODE_HEX	5
#define MODE_SLIP_AUTO	6
#define MODE_SLIP	7
#define MODE_SLIP_HIDE	8
*/

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
}

SensorProxy::~SensorProxy()
{
	// Destroy object
	cout<<"Destroy sensor proxy." << popcendl;
	close(m_fd);
}

/*
int SensorProxy::SetData(const char* JSONData)
{
	BufferSendData(JSONData);
	BufferReadData();
	fflush(stdout);
	return 0;
}

void SensorProxy::Publish()
{
	for(auto elem : m_subscribed)
	{
		elem->Publish("new publisher !!");
	}
}
*/


/// A gateway subscribes to a sensor to be sent the data
/*
void SensorProxy::SubscribeMe(POPSensor& xr_gateway)
{
	cout<<"SubscribeMe" <<popcendl;
	m_subscribed.push_back(&xr_gateway);
	// xr_gateway.Publish();
}
*/



void SensorProxy::SendRawData(const std::string& JSONData)
{

	const char* data = JSONData.c_str();
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
	while(m_listening == true) {   
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
	m_listening = true;
	while(m_listening == true)
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
	m_listening = false; // TODO: See if we must protect this var
}

map<RecordHeader, double> SensorProxy::RetrieveDataDouble()
{
	// cout << "Retrieve " << m_doubleData.size() << " records of type double" <<popcendl;
	return m_doubleData;
}

map<RecordHeader, int> SensorProxy::RetrieveDataInt()
{
	// cout << "Retrieve " << m_intData.size() << " records of type int" <<popcendl;
	return m_intData;
}

map<RecordHeader, string> SensorProxy::RetrieveDataString()
{
	// cout << "Retrieve " << m_stringData.size() << " records of type string" <<popcendl;
	return m_stringData;
}


void SensorProxy::ClearData()
{
	m_doubleData.clear();
	m_intData.clear();
	m_stringData.clear();
}

/// Handle all incoming messages
void SensorProxy::HandleIncomingMessage(const std::string& x_rawMsg)
{
	// cout << "received" << x_rawMsg << popcendl;
	struct timeval time_now;
	gettimeofday(&time_now, NULL);
	unsigned int ms = time_now.tv_sec * 1000 + time_now.tv_usec / 1000;
	ms = ms % 1000000; // TODO: Fix buffering problem with int, short, long
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
				char data[BUFFERSIZE];
				if(unbufferizeNotifyMessage(&msg, data, x_rawMsg.c_str(), sizeof(data)) <= 0)
					throw POPException("Cannot unbufferize notify message");
				switch(msg.dataType)
				{
					case TYPE_DOUBLE:
						m_doubleData.insert(std::pair<RecordHeader, double>(RecordHeader(ms, msg), atof(data))); 
					break;
					case TYPE_INT:
						m_intData.insert(std::pair<RecordHeader, int>(RecordHeader(ms, msg), atoi(data))); 
					break;
					case TYPE_STRING:
					{
						if(msg.measurementType == MSR_LOG)
						{
							cout<< "Remote log message: '" << data << "'" << popcendl;
						}
						else
						{
							std::string str(data);
							m_stringData.insert(std::pair<RecordHeader, std::string>(RecordHeader(ms, msg), str)); 
						}
					}
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

void SensorProxy::Publish(int x_publicationType, int x_data)
{
	char dataBuffer[BUFFERSIZE];
	char buf[BUFFERSIZE];
	sprintf(dataBuffer, "%d", x_data);
	// sprintf(message, "{\"function\":4,\"led\":%d}\n", led);

	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	msg.publicationType = (PublicationType) x_publicationType;
	msg.dataType        = TYPE_INT;
	// msg.unit            = UNT_UNKNOWN;
	msg.id              = m_id;
	msg.dataSize        = strlen(dataBuffer);
	msg.data            = dataBuffer;

	if(bufferizePublishMessage(&msg, buf, BUFFERSIZE) <= 0)
		throw POPException("Cannot bufferize publish message", dataBuffer);
	cout<< "Sending " << buf << popcendl;
	SendRawData(buf);
}

@pack(SensorProxy);
