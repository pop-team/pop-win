#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <ctype.h>


class sensorcom
{
	public:
	sensorcom();
	~sensorcom();
	int connection();
	void bufferSendData(char* JSONData);
	void bufferReadData(char* in);
	int destroy_object(char* JSONData);
	int get_data(char* JSONData, char* InData);
	int set_data(char* JSONData);
	int init_sensor(char* JSONData);
};


