/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * Bourqui Valentin <bourqui.valentin@gmail.com>. 
 * Modified by Laurent Winkler Dec 2014
 */

//----------------------------------------------------------------- 

/****************************/
/********* IMPORT ***********/
/****************************/

#include "contiki.h"

#if PLATFORM_HAS_LEDS
#include "dev/leds.h"
#endif

#include "dev/serial-line.h"
#include <stdio.h>
#include "dev/i2cmaster.h"
#include "dev/tmp102.h"


// #include "jsontree.h"
// #include "jsonparse.h"

#if 1
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#if 0
#define PRINTFDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTFDEBUG(...)
#endif



// Our error message
#define ERROR(__msg__) LOG("Error in %s:%d: %s", __FILE__, __LINE__, __msg__)
#define LOG(...)       logging(__VA_ARGS__)


// Declaration to avoid warnings at compilation
int sscanf ( const char * s, const char * format, ...); // {} // TODO
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );

#include "popwin_messages.h" // Note: should be declared here

// Declaration of local functions
void logging(const char *format,...);
void handleNotification();



// Send a subscribtion message
void sendSubscription(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, BUFFERSIZE) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

// send a notification message
void sendNotification(const struct NotifyMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeNotifyMessage(msg, buf, BUFFERSIZE) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

/// Log message (handled as a notification of type string)
void logging(const char *format,...)
{
	char buf[BUFFERSIZE];
	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_LOG;
	msg.dataType        = TYPE_STRING;
	msg.id              = 333; // TODO ID
	msg.data            = buf;
	msg.dataSize        = strlen(buf);
	sendNotification(&msg);
}

//--------------------------------------------------------------- 


/****************************/
/*** FUNCTIONS TO BE CALLED */
/****************************/

void list_functions();
void get_data      ();


// All functions are stored in a table
typedef void (*FunctionCall)(void);
int g_nbFunctions = 5;
const char* g_commandNames[]  = {"list_functions", "notify",                                       "publish"};
const char* g_commandInputs[] = {"{}",             "{\"temperature\":%%f,\"vibration\":%%f}",       "{\"led\":%%d}"};


//--------------------------------------------------------------- 

//--------------------------------------------------------------- 

/****************************/
/********* PROCESS **********/
/****************************/

PROCESS(init_com_process, "Init communication process");
AUTOSTART_PROCESSES(&init_com_process);

PROCESS_THREAD(init_com_process, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	/* Initialize stuff here. */
	printf("++++++++++++++++++++++++++++++\n");
	printf("+   INIT/START SERIAL COM    +\n");
	printf("++++++++++++++++++++++++++++++\n");  
	leds_off(LEDS_ALL);

	while(1) {
		/* Do the rest of the stuff here. */ 
		PROCESS_YIELD();
		// Wait for an incoming command
		if(ev == serial_line_event_message && data!=NULL) {

			// We received a message from gateway:
			// switch on the different types of message
			LOG("Handle msg type %d", getMessageType(data));
			switch(getMessageType(data))
			{
				case MSG_SUBSCRIBE:
				{
					struct SubscribeMessage msg;
					memset(&msg, 0, sizeof(msg));
					if(unbufferizeSubscribeMessage(&msg, data, BUFFERSIZE) <= 0)
						ERROR("Cannot read message from buffer");

					// Define here what to do on reception 
					// ...
				}

				break;
				case MSG_NOTIFY:
				{
					handleNotification(data);
				}

					break;
				default:
					// Unknown message: print
					ERROR("Unknown message type");
			}

			/*
			struct jsonparse_state parser;
			jsonparse_setup(&parser, data, strlen(data));
			int type;
			while((type = jsonparse_next(&parser)) != 0) {
				if(type == JSON_TYPE_PAIR_NAME) {
					if(jsonparse_strcmp_value(&parser, "function") == 0) {
						// Call the function that was asked
						int id = json_copy_int(&parser);
						if(id >= 0 && id < g_nbFunctions)
						{
							g_functions[id](parser);
						}
						else
						{
							ERROR("Error: Wrong function number");
						}
						break;
					} 
				}
			}
			*/
		}	
	}		
exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

//----------------------------------------------------------------- 

/****************************/
/******** FUNCTIONS *********/
/****************************/

/*
 * List the available functionalities on sensor
 */
void list_functions()
{
	int i;
	char buf[BUFFERSIZE];
	snprintf(buf, BUFFERSIZE, "{\"functions\":[");
	for( i = 0 ; i < g_nbFunctions - 1 ; i++)
	{
		snprintf(buf, BUFFERSIZE, "%s{\"id\":%d,\"description\":\"%s\",\"inputs\":%s},", buf, i, g_commandNames[i], g_commandInputs[i]);
	}
	if(i == g_nbFunctions - 1)
	{
		snprintf(buf, BUFFERSIZE, "%s{\"id\":%d,\"description\":\"%s\",\"inputs\":%s}", buf, i, g_commandNames[i], g_commandInputs[i]);
	}
	snprintf(buf, BUFFERSIZE, "%s}", buf);
	LOG(buf, strlen(buf));
} 


/*
 * Init sensor and search other node for the request
 */
 /*
void init_sensor(struct jsonparse_state parser){
	// connected=1;
	leds_init();
	tmp102_init();
	SEND( "{\"status\":\"OK\"}");
} 
*/

/*
 * Remove connexion with other nodes and stop to send data
 */
 /*
void destroy_object(struct jsonparse_state parser){
	// connected=0;
	SEND( "{\"status\":\"OK\"}");
}
*/

/*
 * Return data to the gateway
 */
void get_data(){
	int16_t sign    = 1;
	int16_t  raw    = tmp102_read_temp_raw();
	uint16_t absraw = raw;
	if(raw < 0) {
		absraw = (raw ^ 0xFFFF) + 1;
		sign = -1;
	}
	int16_t  tempint  = ((absraw >> 8) * sign)-3;
	uint16_t tempfrac = ((absraw >> 4) % 16) * 625;	
	char     minus    = ((tempint == 0) & (sign == -1)) ? '-' : ' ';

	// Create a temperature notification and send
	// SEND( "{\"status\":\"OK\", \"infos\":{\"temperature\":\"%c%d.%04d\"}}", minus, tempint, tempfrac);

	char data[32];
	sprintf(data, "%c%d.%04d", minus, tempint, tempfrac);
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = TYPE_INT;
	msg.id              = 333; // TODO ID
	msg.data            = data;
	msg.dataSize        = strlen(data);
	sendNotification(&msg);
}

/*
 * Set data like turn on/off leds
 */
void handleNotification(const char* data)
{
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	char dataBuffer[32];
	if(unbufferizeNotifyMessage(&msg, dataBuffer, data, BUFFERSIZE) <= 0)
		ERROR("Cannot read message from buffer");

	LOG("Handle notification dataType=%d", msg.dataType);

	switch(msg.dataType)
	{
		case TYPE_DOUBLE:
		{
			// double d = atof(dataBuffer);
			ERROR("no notification TYPE_DOUBLE");
		}
		break;
		case TYPE_INT:
		{
			// TODO: publication not notification + type=LED
			int led = atoi(dataBuffer);
			LOG("Blink led %d", led);
			switch(led)
			{
				case 0:
					leds_toggle(LEDS_BLUE);
					break;
				case 1:
					leds_toggle(LEDS_GREEN);
					break;
				case 2:
					leds_toggle(LEDS_RED);
					break;
				default:
					ERROR("Unknown led number");
			}
		}
		break;
		case TYPE_STRING:
		{
			ERROR("no notification TYPE_STRING");
		}
		break;
		default:
			ERROR("Unknown data type");
	}
}

/*---------------------------------------------------------------------------*/

// Include the sources for sscanf. Not in Contiki
#include "scanf.c"
