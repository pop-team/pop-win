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

#include "lineComm/popwin_messages.h"

#define BAUDRATE B115200
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

using namespace std;

SensorProxy::SensorProxy(const std::string& x_url)
{
	fd_set mask;
	speed_t speed = BAUDRATE;
	const char *device = MODEMDEVICE;
	m_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC );

	if (m_fd <0) {
		throw POPException("Cannot open device", device);
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
		throw POPException("Fail to open "MODEMDEVICE);
	}

	printf("port is open on %d\n", m_fd);
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
		char buf[BUFSIZE+1];
		int n = read(m_fd, buf, BUFSIZE);
		// printf("n=%d\n", n);
		/*
		for(int i = 0; i < n; i++) {
			printf("%c", buf[i]);
		}
		*/
		if(n==0)
		{
			// read(...) is not blocking. Add a sleep to reduce the charge when inactive // TODO: Improve ?
			usleep(100000); // sleep 100 ms
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

vector<double> SensorProxy::RetrieveDataDouble()
{
	cout << "Retrieve " << m_doubleData.size() << " records of type double" <<popcendl;
	return m_doubleData;
}

vector<int> SensorProxy::RetrieveDataInt()
{
	cout << "Retrieve " << m_intData.size() << " records of type int" <<popcendl;
	return m_intData;
}

vector<string> SensorProxy::RetrieveDataString()
{
	cout << "Retrieve " << m_stringData.size() << " records of type string" <<popcendl;
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
	switch(getMessageType(x_rawMsg.c_str()))
	{
		case MSG_SUBSCRIBE:
		{
			SubscribeMessage msg;
			unbufferizeSubscribeMessage(&msg, x_rawMsg.c_str(), x_rawMsg.size());

			cout << "Subscribtion messages not handled yet" << endl;

			// Define here what to do on reception 
			// ...
			
		}

		break;
		case MSG_NOTIFY:
		{
			NotifyMessage msg;
			char data[32];
			unbufferizeNotifyMessage(&msg, data, x_rawMsg.c_str(), x_rawMsg.size());
			switch(msg.dataType)
			{
				case TYPE_DOUBLE:
					m_doubleData.push_back(atof(data));
				break;
				case TYPE_INT:
					m_intData.push_back(atoi(data));
				break;
				case TYPE_STRING:
				{
					if(msg.measurementType == MSR_LOG)
					{
						cout<< "Remote log message: " << data << popcendl;
					}
					else
					{
						m_stringData.push_back(data); 
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
			cout << "Received raw message: " << x_rawMsg << popcendl;
	}
	
}

void SensorProxy::Publish(int x_publicationType, int x_data)
{
	char dataBuffer[32];
	char buf[BUFFERSIZE];
	sprintf(dataBuffer, "%d", x_data);
	// sprintf(message, "{\"function\":4,\"led\":%d}\n", led);

	struct PublishMessage msg;
	memset(&msg, 0, sizeof(struct PublishMessage));
	msg.publicationType = (PublicationType) x_publicationType;
	msg.dataType        = TYPE_INT;
	// msg.unit            = UNT_UNKNOWN;
	msg.id              = 111; // TODO ID
	msg.dataSize        = strlen(dataBuffer);
	msg.data            = dataBuffer;

	bufferizePublishMessage(&msg, buf, BUFFERSIZE);
	// cout<< "Sending " << buf << popcendl;
	SendRawData(buf);
}

@pack(SensorProxy);
