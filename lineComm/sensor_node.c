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


// Our error message
#define ERROR(__msg__) LOG("Error in %s:%d: %s", __FILE__, __LINE__, __msg__)
#define LOG(...)       logging(__VA_ARGS__)
#if 0 // Set to 1 to enable debug logs
#define DEBUG(...)       logging(__VA_ARGS__)
#else
#define DEBUG(...)
#endif
#define ABS(x) ((x) < 0 ? (-x) : (x))


// Declaration to avoid warnings at compilation
int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );

#include "popwin_messages.h" // Note: should be declared here

// Declaration of local functions
void logging(const char *format,...);
void handlePublication();
void handleNotification();

// Id of the mote
int   g_id = 444;


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
	msg.id              = g_id;
	msg.data            = buf;
	msg.dataSize        = strlen(buf);
	sendNotification(&msg);
}

//--------------------------------------------------------------- 


/****************************/
/*** FUNCTIONS TO BE CALLED */
/****************************/

void list_functions();
void read_temperature();
void generate_test_data_double();
void generate_test_data_int();
void generate_test_data_string();


// All functions are stored in a table
typedef void (*FunctionCall)(void);
#define NB_COMMANDS 5
const FunctionCall g_commands[NB_COMMANDS] = {list_functions, read_temperature, generate_test_data_double, generate_test_data_int, generate_test_data_string};
const char* g_commandNames[NB_COMMANDS]    = {"list_functions", "read_temperature", "generate_test_data_double", "generate_test_data_int", "generate_test_data_string"};


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
			DEBUG("Handle msg type %d", getMessageType(data));
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
				case MSG_PUBLISH:
				{
					handlePublication(data);
				}
				break;
				case MSG_NOTIFY:
				{
					handleNotification(data);
				}
				break;
				default:
					// Unknown message type
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
						if(id >= 0 && id < NB_COMMANDS)
						{
							g_commands[id](parser);
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
	for( i = 0 ; i < NB_COMMANDS ; i++)
	{
		LOG("Command %d: %s", i, g_commandNames[i]);
	}
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
void read_temperature(){
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

	DEBUG("Temp %d %d %d  --> %d %d %d", sign, raw, absraw, tempint, tempfrac, (int)minus);

	// Create a temperature notification and send
	// SEND( "{\"status\":\"OK\", \"infos\":{\"temperature\":\"%c%d.%04d\"}}", minus, tempint, tempfrac);

	char data[32];
	sprintf(data, "%c%d.%04d", minus, tempint, tempfrac);
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = TYPE_INT;
	msg.unit            = UNT_CELSIUS;
	msg.id              = g_id;
	msg.data            = data;
	msg.dataSize        = strlen(data);
	sendNotification(&msg);
}

/*
 * Generate data for testing purposes
 */
void generate_test_data_double(){

	int d = 100;
	int i = 0;
	for(i = 0 ; i < 10 ; i++)
	{
		// Due to a bug of Contiki we cannot print doubles
		d *= 3;
		char data[32];
		sprintf(data, "%d.%02d", d / 100, ABS(d % 100));
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		msg.measurementType = MSR_TEMPERATURE;
		msg.dataType        = TYPE_DOUBLE;
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotification(&msg);
	}
}

/*
 * Generate data for testing purposes
 */
void generate_test_data_int(){
	int n = -32000;
	int i = 0;
	for(i = 0 ; i < 10 ; i++)
	{
		n += 1e4;
		char data[32];
		sprintf(data, "%d", n);
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		msg.measurementType = MSR_TEMPERATURE;
		msg.dataType        = TYPE_INT;
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotification(&msg);
	}
}

/*
 * Generate data for testing purposes
 */
void generate_test_data_string(){
	int i = 0;
	for(i = 0 ; i < 10 ; i++)
	{
		char data[32];
		sprintf(data, "Test string number %d !", i);
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		msg.measurementType = MSR_TEMPERATURE;
		msg.dataType        = TYPE_STRING;
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotification(&msg);
	}
}

/*
 * Handle notification messages from gateway
 */
void handleNotification(const char* data)
{
	// To be written
	
}

/*
 * Handle publication messages from gateway
 */
void handlePublication(const char* data)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(msg));
	char dataBuffer[32];
	if(unbufferizePublishMessage(&msg, dataBuffer, data, BUFFERSIZE) <= 0)
		ERROR("Cannot read message from buffer");

	DEBUG("Handle publication dataType=%d", msg.dataType);

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
			int dataInt = atoi(dataBuffer);
			switch(msg.publicationType)
			{
				case PUB_LED:
				{
					DEBUG("Blink led %d", dataInt);
					switch(dataInt)
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
							leds_toggle(LEDS_ALL);
					}
				}
				break;
				case PUB_COMMAND:
				{
					// Commands can be seen as a type of publication
					DEBUG("Call command %d", dataInt);
					if(dataInt >= 0 && dataInt < NB_COMMANDS)
						g_commands[dataInt]();
					else
						ERROR("Unknown command number");
				}
				break;
				default:
					ERROR("Unknown publication type");
			}
		}
		break;
		case TYPE_STRING:
		{
			ERROR("no publication TYPE_STRING");
		}
		break;
		default:
			ERROR("Unknown data type");
	}
}

/*---------------------------------------------------------------------------*/

// Include the sources for sscanf. Not in Contiki
#include "scanf.c"
#include "popwin_messages.c"
