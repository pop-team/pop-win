/*
 * Date: Nov 2015
 * Authors: Laurent Winkler and Cristina Munoz
 * @author Laurent Winkler
 * @author Cristina Munoz
 * @author Marco Lourenço
 *
 * This file is part of the POPWin project.
 *
 * gatewayMote: This code can be uploaded on the remote sensor that acts as a gateway between
 *     a POP-C++ process (SensorProxy) running on a PC and the remote sensor.
 *     It handles both the serial communication via USB to the PC and radio communication with
 *     the rest of the sensor network.
 *
 */

#define CONTIKIv3 // comment for Contiki 2.6 and 2.7
//#define XM1000_FLASH // uncomment to flash code for XM1000 sensor
#define DRW_FLASH 0 // 0 for HEIA-FR test code, 1 for DRW code (UNIGE)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "contiki.h"
#include "lib/list.h"
#include "lib/memb.h"
#include "lib/random.h"
#ifdef CONTIKIv3
#include "net/rime/rime.h"
#include "dev/sht11/sht11.h" // for XM1000
#include "dev/cc2420/cc2420.h" // for tx_power
#else
#include "net/rime.h"
#include "dev/sht11.h" // for XM1000
#include "dev/cc2420.h" // for tx_power
#endif
#include "node-id.h"
#include "dev/leds.h"

#include "dev/serial-line.h"
#ifndef XM1000_FLASH
#include "dev/i2cmaster.h" // for Zolteria Z1
#include "dev/tmp102.h" // for Zolteria Z1
#endif
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

//#define EN_LOGS 1 // enable log messages, comment to disable
//#define EN_DEBUG 1 // enable debug messages, comment to disable

/****************************/
/*** DECLARE FUNCTIONS      */
/****************************/
void gwHandleMessage(const char* data, char fromProxy);
void gwHandlePublication(const char* data, char fromProxy);
void gwHandleNotification(const char* data, char fromProxy);
void gwHandleSubscription(const char* data, char fromProxy);
void sensorSendNotification(struct NotifyMessage* msg);

int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );
//void sht11_init();

int get_id();
void logging(const char *format,...);

/****************************/
/*** GLOBAL VARIABLES       */
/****************************/
char g_debug   = 1; // Toggle debug mode
int g_gateway = DEFAULT_GATEWAY_ID; // ID of the gateway

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
	memset(&buf,0,BUFFERSIZE*sizeof(char));
	if(bufferizeNotifyMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer (2)");

	// Send message via serial line on contiki
	printf("%s\n", buf);
}

// send a publication message: Only for use on the remote gateway
void gwSendPublicationSerial(const struct PublishMessage* msg)
{
	char buf[BUFFERSIZE];
	memset(&buf,0,BUFFERSIZE*sizeof(char));
	if(bufferizePublishMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer (3)");

	// Send message via serial line on contiki
	printf("%s\n", buf);
}

// send notification via multi hop from sensor
void sensorSendNotification(struct NotifyMessage* msg);


/// Return the id of the node
int get_id()
{
	// Id is computed from rime address (for messaging)
	return linkaddr_node_addr.u8[0];
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
	msg.locationSize	= 0;
	sprintf(msg.location, "%s", "");
	gwSendNotificationSerial(&msg);
}

/*---------------------------------------------------------------------------*/
/* We first declare our processes. */
PROCESS(gateway_communication_process , "Communication to/from the gateway");
PROCESS(button_pressed                , "Button pressed");
PROCESS(communication_process         , "Communication process");
PROCESS(drw                           , "Directional Random Walk");
//PROCESS(sensor_events                 , "Send events based on sensor values");
PROCESS(multihop_announce             , "Multihop communication: init and announce periodically");
PROCESS(multihop_sense                , "Take measurements and transmit via multihop");


/* The AUTOSTART_PROCESSES() definition specifices what processes to
   start when this module is loaded. We put our processes there. */

// note: we can choose here which version of the code we want to run:
#if DRW_FLASH
// Processes to run with routing algo of UNIGE
AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &communication_process, &drw/*, &sensor_events*/); // Processes to run with algo of UNIGE
#define DRW_CODE 1
#else
// Processes to use the routing of messages given by the multihop example
//AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &multihop_announce, &multihop_sense);
AUTOSTART_PROCESSES(&gateway_communication_process, &multihop_announce, &multihop_sense);
#endif

static void
broadcast_recvGWM(struct broadcast_conn *c, const linkaddr_t *from)
{
	//printf("broadcast message received from %d.%d: '%s'\n",from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	gwHandleMessage((char *)packetbuf_dataptr(), 1); // careful with parameter fromProxy ! (here 1)
}

static const struct broadcast_callbacks broadcast_callGWM = {broadcast_recvGWM};

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/****************************/
/********* RECEIVE MSG FROM GATEWAY **********/
/****************************/


PROCESS_THREAD(gateway_communication_process, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	cc2420_set_txpower(MAX_POWER);

	/* Initialize stuff here. */
	printf("++++++++++++++++++++++++++++++\n");
	printf("+   INIT/START SERIAL COM    +\n");
	printf("++++++++++++++++++++++++++++++\n");  
	//leds_off(LEDS_ALL);

	broadcast_open(&broadcast, 129, &broadcast_callGWM);

	static char g_busy  = 0;
	print_id();

	while(1) {
		/* Do the rest of the stuff here. */ 
		PROCESS_WAIT_EVENT_UNTIL(ev==serial_line_event_message);
		// PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));
		// printf("RECEIVED DATA\n");
		if(data == NULL)
		{
#ifdef EN_LOGS
			LOG("Received empty data");
#endif
			continue;
		}

		if(g_busy == 0)
		{
			g_busy = 1;
#ifdef EN_DEBUG
			DEBUG("The mote received an interruption on serial line: %s", data);
#endif
			// We received a message from gateway:
			gwHandleMessage(data, 1);
			g_busy = 0;
		}
		else ERROR("Device is busy. Cannot process message");
	}		
	exit:
#ifdef EN_LOGS
	LOG("Exiting process gateway_communication_process");
#endif
	leds_off(LEDS_ALL);
	broadcast_close(&broadcast);
	PROCESS_END();
}

/*
 * Handle any incoming message on gateway
 */
void gwHandleMessage(const char* data, char fromProxy)
{
	enum MessageType mt = getMessageType(data);
#ifdef EN_DEBUG
	DEBUG("Handle msg type %d", getMessageType(data));
#endif
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

	int is_SETGW_MSG = 0;

	// Proxy: the SensorProxy of POPWin
	if(fromProxy)
	{
		switch(msg.dataType)
		{
		case TYPE_DOUBLE:
		{
			ERROR("publication of type TYPE_DOUBLE are not handled yet");
		}
		break;
		case TYPE_INT:
		{
			int dataInt = atoi(msg.data);
			switch(msg.measurementType)
			{
			case MSR_LED:
			{
				switch(dataInt)
				{
				case LED_BLUE_TOGGLE:
					leds_toggle(LEDS_BLUE);
					break;
				case LED_GREEN_TOGGLE:
					leds_toggle(LEDS_GREEN);
					break;
				case LED_GREEN_OFF:
					leds_off(LEDS_GREEN);
					break;
				case LED_GREEN_ON:
					leds_on(LEDS_GREEN);
					break;
				case LED_RED_TOGGLE:
					leds_toggle(LEDS_RED);
					break;
				case LED_ALL_OFF:
					leds_off(LEDS_ALL);
					break;
				case LED_ALL_ON:
					leds_on(LEDS_ALL);
					break;
				case LED_ALL_TOGGLE:
					leds_toggle(LEDS_ALL);
					break;
				default:
					LOG("unknown PUB_LED command");
				}
			}
			break;
			case MSR_SET_GW:
			{
				set_as_gateway(dataInt);
				is_SETGW_MSG = 1; // TODO: check again if needed
			}
			break;
			default:
				ERROR("Unknown notification type");
				printf("MSR msg code: %d\n",msg.measurementType);
			}
		}
		break;
		case TYPE_STRING:
		{
			ERROR("publication of type TYPE_STRING are not handled yet");
		}
		break;
		default:
			ERROR("Unknown data type");
			printf("data type msg code: %d\n",msg.dataType);
		}

		// If on GW and the message comes from the Proxy --> forward it
		// do not rebroadcast set_as_gateway command
		if(g_gateway == get_id() /*&& !is_SETGW_MSG*/)
		{
			packetbuf_copyfrom(data, strlen(data) + 1);
			broadcast_send(&broadcast);
		}
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
		ERROR(data);
		return;
	}

	if(fromProxy)
	{
		// If the message comes from the GW --> forward it // Future work

		// 
		// note: the forwarding of messages to the network of sensors is not implemented yet
		//
	}
	else gwSendSubscriptionSerial(&msg);
}

/*
 * Handle publication messages from gateway
 */
void gwHandlePublication(const char* data, char fromProxy)
{
	//printf("handle publication with data: %s\n",data);
	struct PublishMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizePublishMessage(&msg, data, sizeof(data)) <= 0)
	{
		ERROR("Cannot read message from buffer");
		return;
	}

	if(!fromProxy) //TODO check if really needed
	{
		gwSendPublicationSerial(&msg);
		return;
	}

	int isGWCmd = 0; // used to not rebroadcast set_as_gateway command

	// If on GW and the message comes from the Proxy --> forward it
	// do not rebroadcast set_as_gateway command
	if(g_gateway == get_id() && !isGWCmd)
	{
		packetbuf_copyfrom(data, strlen(data) + 1);
		broadcast_send(&broadcast);
		//printf("broadcast message sent from GW to sensors: %d\n",++countSentBroadcast);
	}
}

/*
 * Print the id of the node
 */
void print_id(){
	LOG("ID of node is %d (= %d.%d)", get_id(), linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
}

/*
 * Design the mote as a gateway
 */
void set_as_gateway(int id){
	g_gateway = id; // !g_gateway;
	LOG("Design %d as gateway %s", id, g_gateway == get_id() ? "on" : "off");
}

#define CHANNEL 135

struct example_neighbor {
	struct example_neighbor *next;
	linkaddr_t addr;
	struct ctimer ctimer;
};

#define NEIGHBOR_TIMEOUT 60 * CLOCK_SECOND
#define MAX_NEIGHBORS 16
LIST(neighbor_table);
MEMB(neighbor_mem, struct example_neighbor, MAX_NEIGHBORS);
/*---------------------------------------------------------------------------*/
/*
 * This function is called by the ctimer present in each neighbor
 * table entry. The function removes the neighbor from the table
 * because it has become too old.
 */
static void
remove_neighbor(void *n)
{
	struct example_neighbor *e = n;

	list_remove(neighbor_table, e);
	memb_free(&neighbor_mem, e);
}
/*---------------------------------------------------------------------------*/
/*
 * This function is called when an incoming announcement arrives. The
 * function checks the neighbor table to see if the neighbor is
 * already present in the list. If the neighbor is not present in the
 * list, a new neighbor table entry is allocated and is added to the
 * neighbor table.
 */
static void
received_announcement(struct announcement *a,
		const linkaddr_t *from,
		uint16_t id, uint16_t value)
{
	struct example_neighbor *e;

	/* We received an announcement from a neighbor so we need to update
     the neighbor list, or add a new entry to the table. */
	for(e = list_head(neighbor_table); e != NULL; e = e->next) {
		if(linkaddr_cmp(from, &e->addr)) {
			/* Our neighbor was found, so we update the timeout. */
			ctimer_set(&e->ctimer, NEIGHBOR_TIMEOUT, remove_neighbor, e);
			return;
		}
	}

	/* The neighbor was not found in the list, so we add a new entry by
     allocating memory from the neighbor_mem pool, fill in the
     necessary fields, and add it to the list. */
	e = memb_alloc(&neighbor_mem);
	if(e != NULL) {
		linkaddr_copy(&e->addr, from);
		list_add(neighbor_table, e);
		ctimer_set(&e->ctimer, NEIGHBOR_TIMEOUT, remove_neighbor, e);
	}
}
static struct announcement example_announcement;
/*---------------------------------------------------------------------------*/
/*
 * This function is called at the final recepient of the message.
 */
static void
recv(struct multihop_conn *c, const linkaddr_t *sender,
		const linkaddr_t *prevhop,
		uint8_t hops)
{
	gwHandleMessage((char *)packetbuf_dataptr(), 0);
}
/*
 * This function is called to forward a packet. The function picks a
 * random neighbor from the neighbor list and returns its address. The
 * multihop layer sends the packet to this address. If no neighbor is
 * found, the function returns NULL to signal to the multihop layer
 * that the packet should be dropped.
 */
static linkaddr_t *
forward(struct multihop_conn *c,
		const linkaddr_t *originator, const linkaddr_t *dest,
		const linkaddr_t *prevhop, uint8_t hops)
{
	/* Find a random neighbor to send to. */
	int num, i;
	struct example_neighbor *n;

	if(list_length(neighbor_table) > 0) {
		num = random_rand() % list_length(neighbor_table);
		i = 0;
		for(n = list_head(neighbor_table); n != NULL && i != num; n = n->next) {
			++i;
		}
		if(n != NULL) {
			return &n->addr;
		}
	}
	printf("%d.%d: did not find a neighbor to foward to\n",
			linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
	return NULL;
}

static const struct multihop_callbacks multihop_call = {recv, forward};
static struct multihop_conn multihop;

void sensorSendNotification(struct NotifyMessage* msg)
{
	// declare destination
	linkaddr_t to;
	to.u8[0] = g_gateway; // ID of our gateway
	to.u8[1] = 0;

	// if on sensor
	if(g_gateway != get_id())
	{
		char buf[BUFFERSIZE];
		memset(&buf,0,BUFFERSIZE*sizeof(char));
		if(bufferizeNotifyMessage(msg, buf, sizeof(buf)) <= 0)
			ERROR("Cannot write message to buffer");

		// Use the multihop method to send a message
		/* Copy the "Hello" to the packet buffer. */
		packetbuf_copyfrom(buf, strlen(buf) + 1);

		/* Send the packet. */
		multihop_send(&multihop, &to);
	}
	else // on GW
	{
		gwSendNotificationSerial(msg);
	}
}

unsigned char sense_leds()
{
	unsigned char value = leds_get();
	return value;
}

/*---------------------------------------------------------------------------*/
// For communication based on multihop: init and announce periodically


PROCESS_THREAD(multihop_announce, ev, data)
{
	PROCESS_EXITHANDLER(multihop_close(&multihop);)
									PROCESS_BEGIN();
	printf("Launching process multihop_announce\n");

	// Init for multihop -----
	/* Initialize the memory for the neighbor table entries. */
	memb_init(&neighbor_mem);

	/* Initialize the list used for the neighbor table. */
	list_init(neighbor_table);

	/* Open a multihop connection on Rime channel CHANNEL. */
	multihop_open(&multihop, CHANNEL, &multihop_call);

	static struct etimer et;
	const unsigned int WAIT_SHORT = 5;
	const unsigned int WAIT_LONG = 10;

	while(1){

		leds_on(LEDS_RED);
		/* Register an announcement with the same announcement ID as the
		   Rime channel we use to open the multihop connection above. */
		announcement_register(&example_announcement,
				CHANNEL,
				received_announcement);

		/* Set a dummy value to start sending out announcments. */
		announcement_set_value(&example_announcement, 0);
		leds_off(LEDS_RED);

		etimer_set(&et, WAIT_SHORT * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
// For communication based on multihop: read sensor data

PROCESS_THREAD(multihop_sense, ev, data)
{
	//PROCESS_EXITHANDLER(multihop_close(&multihop);)
	PROCESS_BEGIN();
	printf("Launching process multihop_sense\n");

#ifndef XM1000_FLASH
	tmp102_init(); // Z1
#else
	sht11_init(); // XM1000
#endif

	static struct etimer et;

	const unsigned int WAIT_SENSE = 5;

	while(1){
		etimer_set(&et, WAIT_SENSE * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		/*---- start of multihop call ----*/
		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));

		// Alternate the type of measurement
		float fl = 0;

		msg.measurementType = MSR_TEMPERATURE;
		msg.dataType        = TYPE_DOUBLE;
		msg.unit            = UNT_CELSIUS;
#ifndef XM1000_FLASH
		fl					= sense_temperature_float_Z1(); // Z1
#else
		fl					= sense_temperature_float(); // XM1000
#endif
		sprintf(msg.data, "%d.%u", (int)fl, DEC(fl)); // for XM1000
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		sprintf(msg.location,"%s","d2015");
		msg.locationSize	= strlen(msg.location);
		sensorSendNotification(&msg);

#ifdef XM1000_FLASH
		memset(&msg, 0, sizeof(msg));
		fl = 0;
		msg.measurementType = MSR_HUMIDITY;
		msg.dataType        = TYPE_DOUBLE;
		msg.unit            = UNT_PERCENT;
		fl                  = sense_humidity_float();// XM1000
		sprintf(msg.data, "%d.%u", (int)fl, DEC(fl));// XM1000
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		sprintf(msg.location,"%s","d2015");
		msg.locationSize	= strlen(msg.location);
		sensorSendNotification(&msg);

		memset(&msg, 0, sizeof(msg));
		fl = 0;
		msg.measurementType = MSR_LIGHT;
		msg.dataType        = TYPE_INT;
		msg.unit            = UNT_LUX;
		sprintf(msg.data, "%d", sense_light());// XM1000
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		sprintf(msg.location,"%s","d2015");
		msg.locationSize	= strlen(msg.location);
		sensorSendNotification(&msg);

		memset(&msg, 0, sizeof(msg));
		fl = 0;
		msg.measurementType = MSR_INFRARED;
		msg.dataType        = TYPE_INT;
		msg.unit            = UNT_LUX;
		sprintf(msg.data, "%d", sense_infrared());// XM1000
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		sprintf(msg.location,"%s","d2015");
		msg.locationSize	= strlen(msg.location);
		sensorSendNotification(&msg);
#endif

		memset(&msg, 0, sizeof(msg));
		fl = 0;
		msg.measurementType = MSR_LED;
		msg.dataType        = TYPE_INT;
		msg.unit            = UNT_NONE;
		sprintf(msg.data, "%d", sense_leds());// XM1000 || Z1
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);
		sprintf(msg.location,"%s","d2015");
		msg.locationSize	= strlen(msg.location);
		sensorSendNotification(&msg);

	}
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
// Button pressed routine

PROCESS_THREAD(button_pressed, ev, data)
{   
	PROCESS_EXITHANDLER(goto exit);
	PROCESS_BEGIN();

	printf("Launching process button_pressed\n");

	/* Initialize stuff here. */ 

	SENSORS_ACTIVATE(button_sensor);
	//leds_on(LEDS_ALL);

	while(1) {
		/* Do the rest of the stuff here. */

		static uint8_t push = 0;	// Keeps the number of times the user pushes the button sensor
		PROCESS_WAIT_EVENT_UNTIL((ev==sensors_event) && (data == &button_sensor));
		printf("Button event !\n");

		/*---- start of multihop call ----*/
		linkaddr_t to;
		to.u8[0] = g_gateway; // ID of our gateway
		to.u8[1] = 0;

		struct NotifyMessage msg;
		memset(&msg, 0, sizeof(msg));
		int d = 5567;
		sprintf(msg.data, "%d.%02d", d / 100, ABS(d % 100));
		msg.measurementType = MSR_TEST;
		msg.dataType        = TYPE_DOUBLE;
		msg.id              = get_id();
		msg.dataSize        = strlen(msg.data);

		// In case we are on the gateway: send via serial to PC
		gwSendNotificationSerial(&msg);
#ifndef DRW_CODE
		if(g_gateway != get_id())
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
#endif
		/*---- end of multihop call ----*/

		//if ((int)node_id == SENDER) // TODO: CM Maybe remove this
		{
			// If we are on the sender, go to message state
			state = NEW_MESSAGE;
		}

		// Toggle the LEDS
		if (push % 2 == 0) { 
			//leds_off(LEDS_ALL);
			LOG("Button pressed [%d] TURNING OFF ALL LEDS ... [DONE]", push);
			push++;
		} else {
			//leds_on(LEDS_ALL);
			LOG("Button pressed [%d] TURNING ON ALL LEDS ...   [DONE]", push);
			push++;
		}
		if (push == 255){ push = 0;} // Prevents overflowing		


		LOG("Sensor has id %d (%d)", get_id(), node_id);

		// To test the sensor: read value and printf
#ifndef XM1000_FLASH
		sense_temperature_float_Z1(); // Z1
#else
		sense_temperature_float(); // XM1000
		sense_humidity_float();
		sense_light();
		sense_infrared();
#endif

	}

	exit:
	printf("exit button pressed\n");
	//leds_off(LEDS_ALL);
	PROCESS_END();
}

// Include the sources of external files for compilation
#include "scanf.c"
#include "popwin_messages.c"
#include "DRW.c"

