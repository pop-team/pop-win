/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief socket declaration of combox
 *
 */

#ifndef POPC_COMBOX_SOCKET_H
#define POPC_COMBOX_SOCKET_H

#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <ctype.h>

#include <paroc_array.h>

#include "paroc_combox.h"
/**
 * @class paroc_connection_sock
 * @brief Socket connection, used by POP-C++ runtime.
 * @author Tuan Anh Nguyen
 *
 */
class paroc_connection_sock: public  paroc_connection
{
public:
	paroc_connection_sock(paroc_combox *cb);
	paroc_connection_sock(int fd, paroc_combox *cb);
	paroc_connection_sock(paroc_connection_sock &me);

	virtual paroc_connection *Clone();
  virtual void reset() {};	

	int sockfd;
};

/**
 * @class paroc_combox_socket
 * @brief Socket declaration of combox, used by POP-C++ runtime.
 * @author Tuan Anh Nguyen
 *
 */
class paroc_combox_socket:public paroc_combox
{
public:
	paroc_combox_socket();

	virtual ~paroc_combox_socket();

	virtual bool Create(int port=0, bool server=false);
	virtual bool Create(const char *address, bool server=false) { return false; };	

	virtual bool Connect(const char *url);

	virtual int Send(const char *s,int len);
	virtual int Send(const char *s,int len, paroc_connection *connection);
	virtual paroc_connection* get_connection() { if(!peer){ return NULL;} return peer; };


	virtual int Recv(char *s,int len);
	virtual int Recv(char *s,int len, paroc_connection *connection);

	virtual paroc_connection *Wait();

	virtual void Close();

	/**
	 * @brief Returns URL of object
	 * @param accesspoint Returned URL (protocol://host:port)
	 * @return true if success
	 */
	virtual bool GetUrl(paroc_string & accesspoint);
	virtual bool GetProtocol(paroc_string & protocolName);

protected:
	virtual paroc_connection_sock *CreateConnection(int fd);
	bool CloseSock(int fd);
	bool Connect(const char *host,int port);

	int GetSockInfo(sockaddr &info,socklen_t &len);
	int GetPort();

	int GetOpt(int level, int opt, char *buf, socklen_t &len);
	int SetOpt(int level, int opt, char *buf, socklen_t len);

protected:
	int sockfd;
	bool isServer;
	bool isCanceled;

	paroc_connection_sock *peer;

	//Only used by combox server...
	paroc_array<pollfd> pollarray;
	paroc_array<paroc_connection_sock *> connarray;
	int index;
	int nready;
};


/**
 * @class paroc_combox_sensor
 * @brief sensor by terminal declaration of combox, used by POP-C++ runtime.
 * @author Bourqui Valentin
 *
 */
class paroc_combox_sensor:public paroc_combox
{
public:
	paroc_combox_sensor();

	virtual ~paroc_combox_sensor();


	virtual int Send(const char *s,int len);
	virtual int Recv(char *s,int len);

	virtual paroc_connection* get_connection() { if(!peer){ return NULL;} return peer; };




	virtual paroc_connection *Wait();

	virtual void Close();
  virtual int Destroy_object(char* c,int length, char* s=NULL);
  virtual int Get_data(char* c,int length, char* s=NULL);
  virtual int Set_data(char* c,int length, char* s=NULL);
  virtual int Init_sensor(char* c,int length, char* s=NULL);
  
	/**
	 * @brief Returns URL of object
	 * @param accesspoint Returned URL (protocol://host:port)
	 * @return true if success
	 
	virtual bool GetUrl(paroc_string & accesspoint);
	virtual bool GetProtocol(paroc_string & protocolName);
*/
protected:
	//virtual paroc_connection_sock *CreateConnection(int fd);
	bool Connect();



protected:
	/*int sockfd;
	bool isServer;
	bool isCanceled;

	paroc_connection_sock *peer;

	//Only used by combox server...
	paroc_array<pollfd> pollarray;
	paroc_array<paroc_connection_sock *> connarray;
	int index;
	int nready;
	*/
	paroc_connection_sock *peer;
	//***************** RECENT ADD ***************3
	
	static unsigned char rxbuf[2048];
	struct termios options;
	fd_set mask, smask;
	int fd;
	speed_t speed;
	POPString speedname ;
	POPString device ;
	char *timeformat;
	unsigned char mode;
	int nfound, flags;
	unsigned char lastc;
	int indexS;
};


#endif







