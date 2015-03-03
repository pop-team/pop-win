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
void SensorProxy::SubscribeMe(POPSensor& xr_gateway)
{
	cout<<"SubscribeMe" <<popcendl;
	m_subscribed.push_back(&xr_gateway);
	// xr_gateway.Publish();
}



void SensorProxy::SendData(const std::string& JSONData)
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
	printf("SensorProxy::StartListening\n");
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
				cout << "received:" << msg << ", publish to "<< m_subscribed.size() <<popcendl;
				// Store data in table
				m_data.push_back(msg);

				/*
				for(auto elem : m_subscribed)
				{
					// A bug happens here
					// elem->Publish();
				}
				*/
			}
		}
		else
		{
			printf("Received an empty message\n");
		}
	}
	printf("End of SensorProxy::StartListening\n");
}

void SensorProxy::StopListening()
{
	printf("SensorProxy::StopListening\n");
	m_listening = false; // TODO: See if we must protect this var
}

vector<string> SensorProxy::RetrieveData()
{
	cout << "Retrieve " << m_data.size() << " records" <<popcendl;
	return m_data;
}


void SensorProxy::ClearData()
{
	m_data.clear();
}

@pack(SensorProxy);
