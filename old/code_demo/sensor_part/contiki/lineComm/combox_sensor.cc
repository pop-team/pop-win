/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief Implementation of the communication box for TCP/IP socket.
 *
 *
 */
 
/* 
  Deeply need refactoring: 
    POPC_ConnectionTCPIP instead of paroc_connection_sock
    POPC_ComboxTCPIP instead of paroc_combox_sock
   Need to separate connection and combox implementation in two separate file. 
 */ 



#include <regex.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#include "paroc_combox_socket.h"
#include "paroc_system.h"

#define PROTO_TCP 6
#define PROTO_UDP 17


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



/*paroc_connection_sock::paroc_connection_sock(paroc_combox *cb): paroc_connection(cb)
{
	sockfd=-1;
	signal(SIGPIPE, SIG_IGN);
}

paroc_connection_sock::paroc_connection_sock(int fd, paroc_combox *cb): paroc_connection(cb)
{
	sockfd=fd;
}

paroc_connection_sock::paroc_connection_sock(paroc_connection_sock &me): paroc_connection(me.GetCombox(), me.GetBufferFactory())
{
	sockfd=me.sockfd;
}

paroc_connection *paroc_connection_sock::Clone()
{
	return new paroc_connection_sock(*this);
}
*/


unsigned char buf[BUFSIZE];

paroc_combox_sensor::paroc_combox_sensor()
{
   speed = BAUDRATE;
   speedname = BAUDRATE_S;
	 device = MODEMDEVICE;
	 timeformat = NULL;
	 mode = MODE_START_TEXT;
	 flags = 0;
	 lastc = '\0';
	 indexS=0;
	/*peer=NULL;
	sockfd=-1;
	index=0;
	nready=0;
	isCanceled=false;
   isServer=false;*/
}

paroc_combox_sensor::~paroc_combox_sensor()
{
	Close();
}


int paroc_combox_sensor::Send(const char *JSONData,int n)
{
  if (n < 0) {
    return -1;
  } else if (n > 0) {
    if(n > 0) {
      int i;
      for (i = 0; i < n; i++) { 
        if (write(fd, &JSONData[i], 1) <= 0) {
 	        return -1;
        }  else {
          fflush(NULL);
          usleep(6000);
        }
      }
    }
  } else {
    /* End of input, exit. */
    return -1;
  }
  return 0;
}

int paroc_combox_sensor::Recv(char *JSONData,int length)
{
  int n=-1;
  int i,j;
  int b=0;
  usleep(10*1000);
  while(n != 0) {   
    n=-1; 
    i=0;
    j=0;
    i,j, n = read(fd, JSONData, sizeof(length));
 //    for(i = 0; i < n; i++) {
   //   printf("%c", buf[i]);
 //    }
  }
  return 1;
}


paroc_connection* paroc_combox_sensor::Wait()
{


}

int paroc_combox_sensor::Destroy_object(char* c,int length, char* s)
{
    const char* JSONData;
    if(s==NULL){
      JSONData=s;
    }
    else{
      JSONData=  "{\"function\":1}\n";
    }
    if(Send(JSONData, strlen(JSONData))){
      return -1;
    }
    if(Recv(c,length)){
      return -1;
    }
    fflush(stdout);
    return 0;
}

int paroc_combox_sensor::Get_data(char* c,int length, char* s)
{
    const char* JSONData;
    if(s==NULL){
      JSONData=s;
    }
    else{
      JSONData=  "{\"function\":2}\n";
    }
    if(Send(JSONData, strlen(JSONData))){
      return -1;
    }
    if(Recv(c,length)){
      return -1;
    }
    fflush(stdout);
    return 0;
}

int paroc_combox_sensor::Set_data(char* c,int length, char* s)
{
    const char* JSONData;
    if(s==NULL){
      JSONData=s;
    }
    else{
      JSONData=  "{\"function\":3, \"led\":1}\n";
    }
    if(Send(JSONData,  strlen(JSONData))){
      return -1;
    }
    if(Recv(c,length)){
      return -1;
    }
    fflush(stdout);
    return 0;
}

int paroc_combox_sensor::Init_sensor(char* c,int length, char* s)
{
    const char* JSONData;
    if(s==NULL){
      JSONData=s;
    }
    else{
      JSONData = "{\"function\":0}\n";
    }
    if(Send(JSONData,  strlen(JSONData))){
      return -1;
    }
    if(Recv(c,length)){
      return -1;
    }
    fflush(stdout); 
    return 0;
}

void paroc_combox_sensor::Close()
{
  char c[BUFSIZE];
  Destroy_object(c,BUFSIZE);
}

bool paroc_combox_sensor::Connect()
{
  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC );

  if (fd <0) {
    return -1;
  }

  if (fcntl(fd, F_SETFL, 0) < 0) {
    return -1;
  }

  if (tcgetattr(fd, &options) < 0) {
    return -1;
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
    return -1;
  }

  FD_ZERO(&mask);
  FD_SET(fd, &mask);
  FD_SET(fileno(stdin), &mask);

  indexS = 0;
	return 0;
}

/*paroc_connection_sock *paroc_combox_sensor::CreateConnection(int fd)
{
	return new paroc_connection_sock(fd, this);
}*/
