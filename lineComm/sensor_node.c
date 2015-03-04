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

#include "jsontree.h"
#include "jsonparse.h"

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
#define ERROR(__msg__) printf("Error in %s:%d: %s", __FILE__, __LINE__, __msg__)


// Declaration to avoid warnings at compilation
int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );

#define POPWIN_MOTE
#include "popwin_messages.h" // Note: should be declared here


// Send an unformatted message (to use only for debug)
// Note: to send data via serial line, simply use a printf
#define SEND(...) send(__VA_ARGS__)
void send(const char *format,...)
{
	char msg[BUFFERSIZE];
	va_list ap;
	va_start(ap, format);
	vsprintf(msg, format, ap);
	va_end(ap);

	printf("%s\n", msg);
}

void sendSubscription(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, BUFFERSIZE) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

void sendNotification(const struct NotifyMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeNotifyMessage(msg, buf, BUFFERSIZE) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

/*
int send(const void* msg, size_t size)
{
	if(size > BUFFERSIZE - 2)
		return 0;

	char buf[BUFFERSIZE];
	memcpy(buf, msg, size); // Cannot do this !
	buf[size]   = '\0';
	printf("%s\n", buf);

	return 1;
}
*/

//--------------------------------------------------------------- 


/****************************/
/*** FUNCTIONS TO BE CALLED */
/****************************/

void list_functions(struct jsonparse_state parser); // Function : 0
void init_sensor   (struct jsonparse_state parser); // Function : 1
void destroy_object(struct jsonparse_state parser); // Function : 2
void get_data      (struct jsonparse_state parser); // Function : 3
void set_data      (struct jsonparse_state parser); // Function : 4

// Utility functions: TODO: What to do ?
	static void
json_copy_string(struct jsonparse_state *parser, char *string, int len)
{
	jsonparse_next(parser);
	jsonparse_next(parser);
	jsonparse_copy_value(parser, string, len);
}

	static int
json_copy_int(struct jsonparse_state *parser)
{
	jsonparse_next(parser);
	jsonparse_next(parser);
	return jsonparse_get_value_as_int(parser);
}

// All functions are stored in a table
typedef void (*FunctionCall)(struct jsonparse_state);
int g_nbFunctions = 5;
FunctionCall g_functions[]     = {&list_functions, &init_sensor, &destroy_object,&get_data,&set_data};
const char* g_functionNames[]  = {"list_functions", "init_sensor", "destroy_object", "get_data", "set_data"};
const char* g_functionInputs[] = {"{}", "{}", "{}", "{}", "{\"led\":%%d}"};


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
		// Wait for an incoming message
		if(ev == serial_line_event_message && data!=NULL) {
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
							SEND( "{\"status\":\"NOK\", \"infos\":\"Wrong function number\"}}");
						}
						break;
					} 
				}
			}
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
void list_functions(struct jsonparse_state parser)
{
	SEND( "{\"status\":\"OK\",");
	SEND("\"functions\":[");
	int i;
	for( i = 0 ; i < g_nbFunctions - 1 ; i++)
	{
		SEND("{\"id\":%d,\"description\":\"%s\",\"inputs\":%s},", i, g_functionNames[i], g_functionInputs[i]);
	}
	if(i == g_nbFunctions - 1)
	{
		SEND("{\"id\":%d,\"description\":\"%s\",\"inputs\":%s}", i, g_functionNames[i], g_functionInputs[i]);
	}
	SEND( "}");
} 


/*
 * Init sensor and search other node for the request
 */
void init_sensor(struct jsonparse_state parser){
	// connected=1;
	leds_init();
	tmp102_init();
	SEND( "{\"status\":\"OK\"}");
} 

/*
 * Remove connexion with other nodes and stop to send data
 */
void destroy_object(struct jsonparse_state parser){
	// connected=0;
	SEND( "{\"status\":\"OK\"}");
}

/*
 * Return data to the gateway
 */
void get_data(struct jsonparse_state parser){
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
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = UNT_CELSIUS;
	msg.id              = 333; // TODO ID
	msg.data            = data;
	msg.dataSize        = strlen(data);
	sendNotification(&msg);
}

/*
 * Set data like turn on/off leds
 */
void set_data(struct jsonparse_state parser){
	if(1/*connected==1*/)
	{
		int type;
		int update = 0;
		int led= 3;
		while((type = jsonparse_next(&parser)) != 0 && update == 0) {
			if(type == JSON_TYPE_PAIR_NAME) {
				if(jsonparse_strcmp_value(&parser, "led") == 0) {
					led = json_copy_int(&parser);
					update=1;  
				}  
			}
		}   
		if(led==0){
			leds_toggle(LEDS_BLUE);
			SEND( "{\"status\":\"OK\"}");
		}
		if(led==1){
			leds_toggle(LEDS_GREEN);
			SEND( "{\"status\":\"OK\"}");
		}
		if(led==2){
			leds_toggle(LEDS_RED);
			SEND( "{\"status\":\"OK\"}");
		}
		if(led==3){
			SEND( "{\"status\":\"NOK\", \"infos\":\"No leds for this ID\"}}");
		}

	}
	else{
		SEND( "{\"status\":\"NOK\", \"infos\":\"Not connected\"}}");
	}
}

/*---------------------------------------------------------------------------*/

