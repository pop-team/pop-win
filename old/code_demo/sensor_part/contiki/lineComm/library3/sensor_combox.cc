#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <regex.h>

#include "sensor_combox.h"

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

static unsigned char rxbuf[2048];
struct termios options;
fd_set mask, smask;
int fd;
speed_t speed = BAUDRATE;
char *speedname = BAUDRATE_S;
char *device = MODEMDEVICE;
char *timeformat = NULL;
char buf[BUFSIZE], outbuf[HCOLS];
unsigned char mode = MODE_START_TEXT;
int nfound, flags = 0;
unsigned char lastc = '\0';
int indexS=0;

sensorcom::sensorcom()
{
}

sensorcom::~sensorcom()
{
}

int sensorcom::connection()
{
  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC );

  if (fd <0) {
    fprintf(stderr, "\n");
    perror(device);
    exit(-1);
  }

  if (fcntl(fd, F_SETFL, 0) < 0) {
    perror("SENSOR: could not set fcntl");
    exit(-1);
  }

  if (tcgetattr(fd, &options) < 0) {
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

  if (tcsetattr(fd, TCSANOW, &options) < 0) {
    perror("could not set options");
    exit(-1);
  }

  FD_ZERO(&mask);
  FD_SET(fd, &mask);
  FD_SET(fileno(stdin), &mask);

  indexS = 0;
  return 1;
}

void sensorcom::bufferSendData(char* JSONData)
{
  int n = strlen(JSONData);
    if (n < 0) {
      perror("could not read");
      exit(-1);
    } else if (n > 0) {
      if(n > 0) {
        int i;
        for (i = 0; i < n; i++) { 
          if (write(fd, &JSONData[i], 1) <= 0) {
   	    perror("write");
	    exit(1);
          }  else {
            fflush(NULL);
            usleep(6000);
          }
        }
      }
    } else {
      /* End of input, exit. */
      exit(0);
    }
   
}
void sensorcom::bufferReadData(char *in)
{
    char* s;
    int n=-1;
    int i,j;
    int b=0;
    usleep(10*1000);
    while(n != 0) {   
      n=-1;
      i=0;
      j=0;
      i,j, n = read(fd, buf, sizeof(buf));
      strcpy(in,buf);
    }
}
int sensorcom::destroy_object(char* JSONData)
{
    char info[100];
    bufferSendData(JSONData);
    bufferReadData(info);
    fflush(stdout);
    return 0;
}

int sensorcom::get_data(char* JSONData, char* InData)
{
    bufferSendData(JSONData);
    bufferReadData(InData);
    fflush(stdout);
    return 0;
}

int sensorcom::set_data(char* JSONData)
{
    char info[100];
    bufferSendData(JSONData);
    bufferReadData(info);
    fflush(stdout);
    return 0;
}

int sensorcom::init_sensor(char* JSONData)
{
    char info[100];
    bufferSendData(JSONData);
    bufferReadData(info);
    fflush(stdout); 
    return 0;
}


