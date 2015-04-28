/*
 * Date: Jan 2015
 * Authors: Laurent Winkler and Cristina Munoz
 *
 * This file is part of the POPWin project.
 *
 * gatewayMote: This code can be uploaded on the remote sensor that acts as a gateway between
 *     a POP-C++ process (SensorProxy) running on a PC and the remote sensor.
 *     It handles both the serial communication via USB to the PC and radio communication with
 *     the rest of the sensor network.
 *
 */


#include "contiki.h"
#include "lib/list.h"
#include "lib/memb.h"
#include "lib/random.h"
#include "net/rime.h"
#include "node-id.h"
#include <stdbool.h>
#include "dev/leds.h"
#include <stdio.h>

// #include "dev/tmp102.h"
#include "dev/serial-line.h"
#include "dev/temperature-sensor.h"
#include "dev/button-sensor.h"
#include "popwin_messages.h"


/****************************/
/*** MACROS                 */
/****************************/
#define ERROR(__msg__) LOG("Error in %s:%d: %s", __FILE__, __LINE__, __msg__)
#define LOG(...)       logging(__VA_ARGS__)
#if 1 // Set to 1 to enable debug logs
#define DEBUG(...)       {if(g_debug){logging(__VA_ARGS__);}}
#else
#define DEBUG(...)
#endif

#define ABS(x) ((x) < 0 ? (-x) : (x))

/****************************/
/*** COMMANDS TO BE CALLED  */
/****************************/
void list_functions();
void read_temperature();
void generate_test_data_double();
void generate_test_data_int();
void generate_test_data_string();
void print_id();
void send_broadcast_cmd();
void toggle_debug();

// All functions are stored in a table
typedef void (*FunctionCall)(void);
#define NB_COMMANDS 8
const FunctionCall g_commands[NB_COMMANDS] = {list_functions, read_temperature, generate_test_data_double, generate_test_data_int, generate_test_data_string, print_id, send_broadcast_cmd, toggle_debug};
const char* g_commandNames[NB_COMMANDS]    = {"list_functions", "read_temperature", "generate_test_data_double", "generate_test_data_int", "generate_test_data_string", "print_id", "send_broadcast_cmd", "toggle_debug"};

/****************************/
/*** DECLARE FUNCTIONS      */
/****************************/
void handlePublication(const char* data);
void handleNotification(const char* data);
void handleSubscription(const char* data);

int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );

int get_id();
void logging(const char *format,...);

// Functions of DRW.c
static void send_broadcast(uint8_t type);

/****************************/
/*** GLOBAL VARIABLES       */
/****************************/
char g_busy  = 0;
char g_debug = 1;

// send a subscription message
void sendSubscriptionSerial(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

// send a notification message: Only for use on the remote sensor
void sendNotificationSerial(const struct NotifyMessage* msg, const char* data)
{
	char buf[BUFFERSIZE];
	if(bufferizeNotifyMessage(msg, data, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

/// Return the id of the node
int get_id()
{
	// Id is computed from rime address (for messaging)
	// normal format is "a.b". We transform it into an integer as a * 256 + b
	/*
	int id = rimeaddr_node_addr.u8[0];
	id *= 256; //  Note: there seems to be a problem on the z1 mote with this line
	id += rimeaddr_node_addr.u8[1];
	*/
	return rimeaddr_node_addr.u8[0];
}

/// Log message (handled as a notification of type string): Only for use on the remote sensor
void logging(const char *format,...)
{
	char buf[BUFFERSIZE];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);

	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_LOG;
	msg.dataType        = TYPE_STRING;
	msg.id              = get_id();
	msg.dataSize        = strlen(buf);
	sendNotificationSerial(&msg, buf);
}

/*---------------------------------------------------------------------------*/
/* Commented by LW: We use the version in DRW.c
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	  LOG("broadcast message received from %d.%d: '%s'",
	           from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
*/
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* We first declare our processes. */
PROCESS(gateway_communication_process, "Communication to/from the gateway");
PROCESS(button_pressed,   "Button pressed");
PROCESS(communication_process, "Communication process");
PROCESS(drw, "Directional Random Walk");


/* The AUTOSTART_PROCESSES() definition specifices what processes to
   start when this module is loaded. We put our processes there. */
AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &communication_process, &drw);
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/****************************/
/********* RECEIVE MSG FROM GATEWAY **********/
/****************************/


PROCESS_THREAD(gateway_communication_process, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	/* Initialize stuff here. */
	printf("++++++++++++++++++++++++++++++\n");
	printf("+   INIT/START SERIAL COM    +\n");
	printf("++++++++++++++++++++++++++++++\n");  
	leds_off(LEDS_ALL);

	// LOG("set broadcast callback");
	// broadcast_open(&broadcast, 129, &broadcast_call); // Commented LW : use the version in DRW.c

	while(1) {
		/* Do the rest of the stuff here. */ 
		PROCESS_WAIT_EVENT_UNTIL((ev==serial_line_event_message));
		if(data == NULL)
		{
			DEBUG("Received empty data");
			continue;
		}

		if(g_busy == 0)
		{
			g_busy = 1;
			DEBUG("The mote received an interruption on serial line: %s", data);

			// We received a message from gateway:
			// switch on the different types of message
			DEBUG("Handle msg type %d", getMessageType(data));
			switch(getMessageType(data))
			{
				case MSG_SUBSCRIBE:
				{
					handleSubscription(data);
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
			g_busy = 0;
		}
		else ERROR("Device is busy. Cannot process message");
	}		
exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

/*
 * Handle notification messages from gateway
 */
void handleNotification(const char* data)
{
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	char data2[BUFFERSIZE];
	if(unbufferizeNotifyMessage(&msg, data2, data, sizeof(data2)) <= 0)
		ERROR("Cannot read message from buffer");

	// 
	// Define here what to do on reception
	// ...
	LOG("Gateway has sent notification: %s", data);

	// note: no action. See if it makes sense to handle notifications from gateway
}

/*
 * Handle notification messages from gateway
 */
void handleSubscription(const char* data)
{
	struct SubscribeMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizeSubscribeMessage(&msg, data) <= 0)
	{
		ERROR("Cannot read message from buffer");
		return;
	}

	// 
	// Define here what to do on reception  TODO for UNIGE
	// ...
	LOG("Gateway has subscribed");
}

/*
 * Handle publication messages from gateway
 */
void handlePublication(const char* data)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(msg));
	char dataBuffer[32];
	if(unbufferizePublishMessage(&msg, dataBuffer, data, sizeof(dataBuffer)) <= 0)
		ERROR("Cannot read message from buffer");

	DEBUG("Handle publication dataType=%d", msg.dataType);

	switch(msg.dataType)
	{
		case TYPE_DOUBLE:
		{
			ERROR("notifications of type TYPE_DOUBLE are not handled yet");
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
			ERROR("notifications of type TYPE_STRING are not handled yet");
		}
		break;
		default:
			ERROR("Unknown data type");
	}
}

/****************************/
/******** COMMANDS  *********/
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
 * Return data to the gateway
 */
void read_temperature(){
	/* TODO
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
	*/

	char data[32];
	sprintf(data, "-15.98");
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = TYPE_DOUBLE;
	msg.unit            = UNT_CELSIUS;
	msg.id              = get_id();
	msg.dataSize        = strlen(data);
	sendNotificationSerial(&msg, data);
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
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_DOUBLE;
		msg.id              = get_id();
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg, data);
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
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_INT;
		msg.id              = get_id();
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg, data);
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
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_STRING;
		msg.id              = get_id();
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg, data);
	}
}

/*
 * Print the id of the node
 */
void print_id(){
	LOG("ID of node is %d (= %d.%d)", get_id(), rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
}

/*
 * Send a broad_cast message
 */
void send_broadcast_cmd(){
	packetbuf_copyfrom("Hello", 6);
	broadcast_send(&broadcast);
	LOG("broadcast message sent");
}

/*
 * Turn debug messages on/off
 */
void toggle_debug(){
	g_debug = !g_debug;
	LOG("Debug messages %s", g_debug ? "on" : "off");
}


//-----------------------------------------------------------------   
// A simple thread to check that the mote is working: print a message and change led state
//-----------------------------------------------------------------
PROCESS_THREAD(button_pressed, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	/* Initialize stuff here. */ 

	SENSORS_ACTIVATE(button_sensor);
	leds_on(LEDS_ALL);

	while(1) {
		/* Do the rest of the stuff here. */

		static uint8_t push = 0;	// Keeps the number of times the user pushes the button sensor
		PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));

		// Send a broadcast message
		send_broadcast_cmd();

		// Toggle the LEDS
		if (push % 2 == 0) { 
			leds_off(LEDS_ALL);
			LOG("Button pressed [%d] TURNING OFF ALL LEDS ... [DONE]", push);
			push++;
		} else {
			leds_on(LEDS_ALL);
			LOG("Button pressed [%d] TURNING ON ALL LEDS ...   [DONE]", push);
			push++;
		}
		if (push == 255){ push = 0;} // Prevents overflowing		 
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

// Include the sources of external files for compilation
#include "scanf.c"
#include "popwin_messages.c"
#include "DRW.c"

