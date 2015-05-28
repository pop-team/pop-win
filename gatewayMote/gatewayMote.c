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

#include "dev/serial-line.h"
#include "dev/temperature-sensor.h"
// #include "dev/tmp102.h"
#include "dev/button-sensor.h"
#include "popwin_messages.h"
#include "queue.h"
#include "DRW.h"


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
void set_as_gateway();

// All functions are stored in a table
typedef void (*FunctionCall)(void);
#define NB_COMMANDS 9
const FunctionCall g_commands[NB_COMMANDS] = {list_functions, read_temperature, generate_test_data_double, generate_test_data_int, generate_test_data_string, print_id, send_broadcast_cmd, toggle_debug, set_as_gateway};
const char* g_commandNames[NB_COMMANDS]    = {"list_functions", "read_temperature", "generate_test_data_double", "generate_test_data_int", "generate_test_data_string", "print_id", "send_broadcast_cmd", "toggle_debug", "set_as_gateway"};

/****************************/
/*** DECLARE FUNCTIONS      */
/****************************/
void gwHandleMessage(const char* data, char fromProxy);
void gwHandlePublication(const char* data, char fromProxy);
void gwHandleNotification(const char* data, char fromProxy);
void gwHandleSubscription(const char* data, char fromProxy);

int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );
void sht11_init();

int get_id();
void logging(const char *format,...);

/****************************/
/*** GLOBAL VARIABLES       */
/****************************/
char g_debug   = 1; // Toggle debug mode
char g_gateway = 0; // Design as a gateway

// send a subscription message
void gwSendSubscriptionSerial(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer (1)");

	// Send message via serial line on contiki
	printf("%s\n", buf);
}

// send a notification message: Only for use on the remote gateway
void gwSendNotificationSerial(const struct NotifyMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeNotifyMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer (2)");

	// Send message via serial line on contiki
	printf("%s\n", buf);
}

// send a publication message: Only for use on the remote gateway
void gwSendPublicationSerial(const struct PublishMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizePublishMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer (3)");

	// Send message via serial line on contiki
	printf("%s\n", buf);
}


/// Return the id of the node
int get_id()
{
	// Id is computed from rime address (for messaging)
	return rimeaddr_node_addr.u8[0];
}

/// Log message (handled as a notification of type string): Only for use on the remote sensor
void logging(const char *format,...)
{

	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	va_list ap;
	va_start(ap, format);
	vsnprintf(msg.data, sizeof(msg.data), format, ap);
	va_end(ap);

	msg.measurementType = MSR_LOG;
	msg.dataType        = TYPE_STRING;
	msg.id              = get_id();
	msg.dataSize        = strlen(msg.data);
	gwSendNotificationSerial(&msg);
}

/*---------------------------------------------------------------------------*/
/* We first declare our processes. */
PROCESS(gateway_communication_process , "Communication to/from the gateway");
PROCESS(button_pressed                , "Button pressed");
PROCESS(communication_process         , "Communication process");
PROCESS(drw                           , "Directional Random Walk");
PROCESS(sensor_events                 , "Send events based on sensor values");
PROCESS(multihop_announce             , "Multihop communication: init and announce periodically");
PROCESS(multihop_sense                , "Take measurements and transmit via multihop");


/* The AUTOSTART_PROCESSES() definition specifices what processes to
   start when this module is loaded. We put our processes there. */

// note: we can choose here which version of the code we want to run:
#if 1
// Processes to run with routing algo of UNIGE
AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &communication_process, &drw, &sensor_events); // Processes to run with algo of UNIGE
#else
// Processes to use the routing of messages given by the multihop example
AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &multihop_announce, &multihop_sense);
#endif
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
	static char g_busy  = 0;

	while(1) {
		/* Do the rest of the stuff here. */ 
		PROCESS_WAIT_EVENT_UNTIL(ev==serial_line_event_message);
		// PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));
		// printf("RECEIVED DATA\n");
		if(data == NULL)
		{
			LOG("Received empty data");
			continue;
		}

		if(g_busy == 0)
		{
			g_busy = 1;
			DEBUG("The mote received an interruption on serial line: %s", data);

			// We received a message from gateway:
			gwHandleMessage(data, 1);
			g_busy = 0;
		}
		else ERROR("Device is busy. Cannot process message");
	}		
exit:
	LOG("Exiting process");
	leds_off(LEDS_ALL);
	PROCESS_END();
}

/*
 * Handle any incoming message on gateway
 */
void gwHandleMessage(const char* data, char fromProxy)
{
	enum MessageType mt = getMessageType(data);
	DEBUG("Handle msg type %d", getMessageType(data));
	switch(mt)
	{
		case MSG_SUBSCRIBE:
			gwHandleSubscription(data, fromProxy);
			break;
		case MSG_NOTIFY:
			gwHandleNotification(data, fromProxy);
			break;
		case MSG_PUBLISH:
			gwHandlePublication(data, fromProxy);
			break;
		default:
			ERROR("Received message of unknown type");
	}
}

/*
 * Handle notification messages from gateway
 */
void gwHandleNotification(const char* data, char fromProxy)
{
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizeNotifyMessage(&msg, data, sizeof(msg.data)) <= 0)
		ERROR("Cannot read message from buffer");

	if(fromProxy)
	{
		// 
		// note: the forwarding of messages to the network of sensors is not implemented yet
		//
		LOG("Proxy has sent notification: %s", data);
	}
	else gwSendNotificationSerial(&msg);
}

/*
 * Handle notification messages from gateway
 */
void gwHandleSubscription(const char* data, char fromProxy)
{
	struct SubscribeMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizeSubscribeMessage(&msg, data) <= 0)
	{
		ERROR("Cannot read message from buffer");
		return;
	}

	if(fromProxy)
	{
		// 
		// note: the forwarding of messages to the network of sensors is not implemented yet
		//
		LOG("Proxy has sent subscription: %s", data);
	}
	else gwSendSubscriptionSerial(&msg);
}

/*
 * Handle publication messages from gateway
 */
void gwHandlePublication(const char* data, char fromProxy)
{
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizePublishMessage(&msg, data, sizeof(data)) <= 0)
		ERROR("Cannot read message from buffer");

	DEBUG("Handle publication dataType=%d", msg.dataType);

	if(!fromProxy)
	{
		gwSendPublicationSerial(&msg);
		return;
	}
	switch(msg.dataType)
	{
		case TYPE_DOUBLE:
		{
			ERROR("notifications of type TYPE_DOUBLE are not handled yet");
		}
		break;
		case TYPE_INT:
		{
			int dataInt = atoi(msg.data);
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
	// Note: this function sends the data via serial line. It should not be used directly

	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	sprintf(msg.data, "%u", sense_temperature());
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = TYPE_INT;
	msg.unit            = UNT_CELSIUS;
	msg.id              = get_id();
	msg.dataSize        = strlen(msg.data);
	gwSendNotificationSerial(&msg);
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
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.data, "%d.%02d", d / 100, ABS(d % 100));
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_DOUBLE;
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		gwSendNotificationSerial(&msg);
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
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.data, "%d", n);
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_INT;
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		gwSendNotificationSerial(&msg);
	}
}

/*
 * Generate data for testing purposes
 */
void generate_test_data_string(){
	int i = 0;
	for(i = 0 ; i < 10 ; i++)
	{
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		sprintf(msg.data, "Test string number %d !", i);
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_STRING;
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		gwSendNotificationSerial(&msg);
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

/*
 * Design the mote as a gateway
 */
void set_as_gateway(){
	g_gateway = 1; // !g_gateway;
	LOG("Design as gateway %s", g_gateway ? "on" : "off");
}


#include "example-multihop.c"
/*---------------------------------------------------------------------------*/
// For communication based on multihop: init and announce periodically


PROCESS_THREAD(multihop_announce, ev, data)
{
	PROCESS_BEGIN();

	// Init for multihop -----
	/* Initialize the memory for the neighbor table entries. */
	memb_init(&neighbor_mem);

	/* Initialize the list used for the neighbor table. */
	list_init(neighbor_table);

	/* Open a multihop connection on Rime channel CHANNEL. */
	multihop_open(&multihop, CHANNEL, &multihop_call);

	static struct etimer et;

	while(1){
		etimer_set(&et, 22 * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		leds_on(LEDS_GREEN);
		/* Register an announcement with the same announcement ID as the
		   Rime channel we use to open the multihop connection above. */
		announcement_register(&example_announcement,
				CHANNEL,
				received_announcement);

		/* Set a dummy value to start sending out announcments. */
		announcement_set_value(&example_announcement, 0);
		leds_off(LEDS_GREEN);
	}
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
// For communication based on multihop: read sensor data

PROCESS_THREAD(multihop_sense, ev, data)
{
	PROCESS_BEGIN();


	static struct etimer et;

	static uint8_t push = 0;	// Keeps the number of times the user pushes the button sensor

	while(1){
		etimer_set(&et, 30 * CLOCK_SECOND);
		// PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et) || ((ev==sensors_event) && (data == &button_sensor)));
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		leds_on(LEDS_BLUE);

		// Send a broadcast message
		// send_broadcast_cmd();

		/*---- start of multihop call ----*/
		rimeaddr_t to;
		to.u8[0] = GATEWAY_ID; // ID of our gateway
		to.u8[1] = 0;

		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));

		// Alternate the type of measurement
		float fl = 0;
		switch(push%4)
		{
			case 0:
				msg.measurementType = MSR_TEMPERATURE;
				msg.dataType        = TYPE_DOUBLE;
				msg.unit            = UNT_CELSIUS;
				fl                  = sense_temperature_float();
				sprintf(msg.data, "%d.%u", (int)fl, DEC(fl));
				break;
			case 1:
				msg.measurementType = MSR_HUMIDITY;
				msg.dataType        = TYPE_DOUBLE;
				msg.unit            = UNT_PERCENT;
				fl                  = sense_humidity_float();
				sprintf(msg.data, "%d.%u", (int)fl, DEC(fl));
				break;
			case 2:
				msg.measurementType = MSR_LIGHT;
				msg.dataType        = TYPE_INT;
				msg.unit            = UNT_LUX;
				sprintf(msg.data, "%d", sense_light());
				break;
			case 3:
				msg.measurementType = MSR_INFRARED;
				msg.dataType        = TYPE_INT;
				msg.unit            = UNT_LUX;
				sprintf(msg.data, "%d", sense_infrared());
				break;
		}
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		push++;

		if(g_gateway)
		{
			// We are on the gateway: send via serial to PC
			gwSendNotificationSerial(&msg);
		}
		else
		{
			char buf[BUFFERSIZE];
			if(bufferizeNotifyMessage(&msg, buf, sizeof(buf)) <= 0)
				ERROR("Cannot write message to buffer");

			// Use the multihop method to send a message
			/* Copy the "Hello" to the packet buffer. */
			packetbuf_copyfrom(buf, strlen(buf) + 1);

			/* Send the packet. */
			multihop_send(&multihop, &to);
		}

		leds_off(LEDS_BLUE);
	}
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
// Button pressed routine

PROCESS_THREAD(button_pressed, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	/* Initialize stuff here. */ 

	SENSORS_ACTIVATE(button_sensor);
	leds_on(LEDS_ALL);

	// Initialization of sensor
	sht11_init();
	// tmp102_init();


	while(1) {
		/* Do the rest of the stuff here. */

		static uint8_t push = 0;	// Keeps the number of times the user pushes the button sensor
		PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));

		// Send a broadcast message
		// send_broadcast_cmd();

		/*---- start of multihop call ----*/
		rimeaddr_t to;
		to.u8[0] = GATEWAY_ID; // ID of our gateway
		to.u8[1] = 0;

		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		int d = 5567;
		sprintf(msg.data, "%d.%02d", d / 100, ABS(d % 100));
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_DOUBLE;
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);

		if(g_gateway)
		{
			// We are on the gateway: send via serial to PC
			gwSendNotificationSerial(&msg);
		}
		else
		{
			char buf[BUFFERSIZE];
			if(bufferizeNotifyMessage(&msg, buf, sizeof(buf)) <= 0)
				ERROR("Cannot write message to buffer");

			// Use the multihop method to send a message
			/* Copy the "Hello" to the packet buffer. */
			packetbuf_copyfrom(buf, strlen(buf) + 1); // +1 is for the null char

			/* Send the packet. */
			multihop_send(&multihop, &to);
		}
		/*---- end of multihop call ----*/

		//if ((int)node_id == SENDER) // TODO: CM Maybe remove this
		{
			// If we are on the sender, go to message state
			state = NEW_MESSAGE;
		}

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


		LOG("Sensor has id %d (%d)", get_id(), node_id);

		// To test the sensor: read value and printf
		// sense_temperature_float();
		// sense_humidity_float();
		// sense_light(); 
		// sense_infrared(); 
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

// Include the sources of external files for compilation
#include "scanf.c"
#include "popwin_messages.c"
#include "DRW.c"

