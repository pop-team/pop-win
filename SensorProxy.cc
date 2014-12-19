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

#define BUFSIZE 100
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

SensorProxy::SensorProxy(POPString x_url)
{
	fd_set mask;
	speed_t speed = BAUDRATE;
	const char *device = MODEMDEVICE;
	m_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC );

	if (m_fd <0) {
		fprintf(stderr, "Cannot open device\n");
		perror(device);
		exit(-1);
	}

	if (fcntl(m_fd, F_SETFL, 0) < 0) {
		perror("SENSOR: could not set fcntl");
		exit(-1);
	}

	struct termios options;
	if (tcgetattr(m_fd, &options) < 0) {
		perror("SENSOR: could not get options");
		exit(-1);
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
		perror("could not set options");
		exit(-1);
	}

	FD_ZERO(&mask);
	FD_SET(m_fd, &mask);
	FD_SET(fileno(stdin), &mask);

	if(m_fd <= 0)
	{
		perror("Fail to open "MODEMDEVICE);
		exit(-1);
	}

	printf("port is open on %d\n", m_fd);
}

SensorProxy::~SensorProxy()
{
	// Destroy object
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
*/


/// Subscribe to a sensor
void SensorProxy::SubscribeMe(const string& x_link)
{
}



void SensorProxy::SendData(POPString JSONData)
{
	const char* data = JSONData.c_str();
	printf("Sending %s on %d\n", data, m_fd);
	int n = strlen(data);
	if (n < 0) {
		perror("could not read");
		exit(-1);
	}
	else if (n > 0)
	{
		if(n > 0)
		{
			for (int i = 0; i < n; i++)
			{
				if (write(m_fd, &data[i], 1) <= 0)
				{
					perror("write");
					exit(1);
				}
				else
				{
					fflush(NULL);
					usleep(6000);
				}
			}
		}
	}
	else
	{
		/* End of input, exit. */
		exit(0);
	}

}
void SensorProxy::ReadData()
{
	char buf[BUFSIZE], outbuf[HCOLS];
	printf("BufferReadData:\n");
	char* s;
	int n=-1;
	int i,j;
	int b=0;
	usleep(10*1000);  // TODO remove
	while(n != 0) {   
		n=-1;
		i=0;
		j=0;
		i,j, n = read(m_fd, buf, sizeof(buf));
		for(i = 0; i < n; i++) {
			printf("%c", buf[i]);
		}
	}
	printf("\n- end of BufferReadData\n");

}



@pack(SensorProxy);
