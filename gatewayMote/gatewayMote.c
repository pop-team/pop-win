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

/* This #define defines the maximum amount of neighbors we can remember. */
#define MAX_NEIGHBORS 16
/* This #define defines the node id corresponding to the database */
#define DATABASE 1
/* This #define defines the node id corresponding to the sender */
#define SENDER 20

static bool printConsole = true;

/* Mote global variables */
static uint8_t state;
static int tag;     //should be a list
static int weight_tag;
static int visited;
static struct unicast_message unicast_message_to_send;
static struct broadcast_message broadcast_message_to_send;
static struct unicast_message message_to_forward;
static bool unicast_ready_to_send = false;
static bool broadcast_ready_to_send = false;
static float broadcast_wait_time = 0.0;
static float unicast_wait_time = 0.0;
static rimeaddr_t unicast_target;
static rimeaddr_t weight_target;


/* This is the structure of broadcast messages. */
struct broadcast_message {
  uint8_t type;
  int tag;       //todo: should be a list 
};

/* This is the structure of unicast ping messages. */
struct unicast_message {
  uint8_t type;
  char * message;
  int tag;       //todo: should be a list
  float weight;
};

/* These are the types of unicast messages that we can send. */
enum {
  UNICAST_TYPE_MESSAGE,          //message to forward to database
  UNICAST_REPLY_WEIGHT,       //information about a mote weight
  BROADCAST_APPLY_TAG,        //telling neihbors they are tagged 
  BROADCAST_COMPUTE_WEIGHT,   //telling neighbors to compute weight    
  BROADCAST_ASK_TAG,          //asking neighbors if they are tagged
  BROADCAST_REPLY_TAG         //message telling if tagged
  
};

/* These are the states in which a mote can be during the DRW */
enum {
  HIDLE,
  COMPUTING_TAGS,
  COMPUTING_WEIGHTS,
  NEW_MESSAGE
};

/* This structure holds information about neighbors. */
struct neighbor {

  struct neighbor *next;
  rimeaddr_t addr;
  int tag;
  float weight;             // #neighbors tagged / total neihbors
  //bool knows_database;  // is he linked to database

};

/* This MEMB() definition defines a memory pool from which we allocate
   neighbor entries. */
MEMB(neighbors_memb, struct neighbor, MAX_NEIGHBORS);

/* The neighbors_list is a Contiki list that holds the neighbors we
   have seen thus far. */
LIST(neighbors_list);

/* These hold the broadcast and unicast structures, respectively. */
static struct broadcast_conn broadcast;
static struct unicast_conn unicast;

// Our error message
#define ERROR(__msg__) LOG("Error in %s:%d: %s", __FILE__, __LINE__, __msg__)
#define LOG(...)       logging(__VA_ARGS__)
#if 1 // Set to 1 to enable debug logs
#define DEBUG(...)       logging(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#define ABS(x) ((x) < 0 ? (-x) : (x))
/****************************/
/*** COMMANDS TO BE CALLED  */
/****************************/
#define NB_COMMANDS 5

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

void handlePublication();
void handleNotification();
void handleSubscription();

int sscanf ( const char * s, const char * format, ...);
size_t strlen ( const char * str );
int snprintf ( char * s, size_t n, const char * format, ... );
int atoi (const char * str);
double atof (const char* str);
void * memset ( void * ptr, int value, size_t num );

void logging(const char *format,...);

int g_id = 555; // TODO: The id should come from the mote itself
char g_busy = 0;

void sendSubscriptionSerial(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
}

// send a notification message: Only for use on the remote sensor
void sendNotificationSerial(const struct NotifyMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeNotifyMessage(msg, buf, sizeof(buf)) <= 0)
		ERROR("Cannot write message to buffer");

	// Send message via serial line on contiki
	printf(buf);
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
	msg.id              = g_id;
	msg.data            = buf;
	msg.dataSize        = strlen(buf);
	sendNotificationSerial(&msg);
}


/*---------------------------------------------------------------------------*/
/* We first declare our three processes. */
PROCESS(broadcast_process, "Broadcast process");
PROCESS(unicast_process, "Unicast process");
PROCESS(drw, "Directional Random Walk");
PROCESS(init_com_process, "Init communication process");
PROCESS(button_pressed, "blink example");

/* The AUTOSTART_PROCESSES() definition specifices what processes to
   start when this module is loaded. We put our processes there. */
AUTOSTART_PROCESSES(&broadcast_process, &unicast_process, &drw, &init_com_process, &button_pressed);
/*---------------------------------------------------------------------------*/


// PRINT INFO
/* print a message when a unicast or broadcast is sent */
static void print_info(uint8_t type, const rimeaddr_t target) {
    
   if (printConsole){
      switch (type){
        
        case UNICAST_TYPE_MESSAGE :
           printf("Sending message to %d\n", target.u8[0]);
        break;
        
        case UNICAST_REPLY_WEIGHT :
           printf("Replying weight to %d\n", target.u8[0]);
        break;
        
        case BROADCAST_COMPUTE_WEIGHT :
           printf("Asking neighbors' weights\n");
        break;
    /*  
        case BROADCAST_APPLY_TAG :
           printf("Asking neighbors to apply tag\n");
        break;
      
        case BROADCAST_ASK_TAG :
           printf("Asking neighbors' tags\n");
        break;

        case BROADCAST_REPLY_TAG :
           printf("Replying about the tag\n");
        break;  */
      }
   }
}

/*---------------------------------------------------------------------------*/
//BROADCAST TAG
/* Tells the neighbor they are part of the current DRW neihgborhood */
static void broadcast_tag(){

      broadcast_message_to_send.type = BROADCAST_APPLY_TAG;
      broadcast_message_to_send.tag = tag;
      
      packetbuf_copyfrom(&broadcast_message_to_send, sizeof(struct broadcast_message));
      broadcast_send(&broadcast);
      
      print_info(BROADCAST_APPLY_TAG, unicast_target);
}


/*---------------------------------------------------------------------------*/


//BROADCAST WEIGHT
/* Ask neighbors to send their weight */
static void broadcast_weight(){

      broadcast_message_to_send.type = BROADCAST_COMPUTE_WEIGHT;
	broadcast_message_to_send.tag = tag;
      
      packetbuf_copyfrom(&broadcast_message_to_send, sizeof(struct broadcast_message));
      broadcast_send(&broadcast);
      
      print_info(BROADCAST_COMPUTE_WEIGHT, unicast_target);
}


/*---------------------------------------------------------------------------*/




//REPLY WEIGHT
/* sends the computed weight to the asking mote */
static void reply_weight(){
     
      float w = visited;
      int total = 0;
      
      if ((int)node_id != DATABASE){
   
            struct neighbor *n;
            for(n = list_head(neighbors_list); n != NULL; n = list_item_next(n)) {
                  total++;
                  if (n->tag == weight_tag) w++;
                  if (n->tag == DATABASE) {
                    w = 0;
                    goto send_reply;
                  }
            }
            
            if (tag == weight_tag) w++;
      
            w /= total;
      
      } else w = -1;
      
      send_reply:
      
      unicast_message_to_send.type = UNICAST_REPLY_WEIGHT;
      unicast_message_to_send.weight = w;
      unicast_message_to_send.tag = tag;
      unicast_target = weight_target;
      unicast_ready_to_send = true;
}
/*---------------------------------------------------------------------------*/


// ADD NEIGHBOR
/* Add a neighbor on the mote's neighbor list */
static void add_neighbor(int ntag, float nweight, const rimeaddr_t *from){
      
        struct neighbor *n;
        
        /* Check if we already know this neighbor. */
        for(n = list_head(neighbors_list); n != NULL; n = list_item_next(n)) {

          /* We break out of the loop if the address of the neighbor matches
             the address of the neighbor from which we received this
             broadcast message. */
          if(rimeaddr_cmp(&n->addr, from)) {
             n->weight = nweight;
             n->tag = ntag;
             //printf("Updating neighbor tag for %d\n", from->u8[0]);
             break;
          }
        }

         /* If n is NULL, this neighbor was not found in our list, and we
           allocate a new struct neighbor from the neighbors_memb memory
           pool. */
        if(n == NULL) {
          n = memb_alloc(&neighbors_memb);

          /* If we could not allocate a new neighbor entry, we give up. We
             could have reused an old neighbor entry, but we do not do this
             for now. */
          if(n == NULL) {
            return;
          }

          /* Initialize the fields. */
          rimeaddr_copy(&n->addr, from);
          n->weight = nweight;
          n->tag = ntag;

          /* Place the neighbor on the neighbor list. */
          list_add(neighbors_list, n);
          
       //   printf("Added neighbor with address %d and tag %d\n",from->u8[0], ntag);
        }
        
        
}

/*---------------------------------------------------------------------------*/


//UNICAST FORWARD
/* This is the function that does the routing to the next mote in the DRW */
static void unicast_forward(){

      //pick neighbor with minimum weight and send a unicast message to it.
      if(list_length(neighbors_list) > 0) {
      
      struct neighbor *n, *t = list_head(neighbors_list);
      float min = MAX_NEIGHBORS;
      for(n = list_head(neighbors_list); n != NULL; n = list_item_next(n)) {
            if (n->weight <= min){
                  min = n->weight;
                  *t = *n;
            }
      }
      
      printf("Forwarding unicast to %d.%d with weight %d\n", t->addr.u8[0], t->addr.u8[1], (int)(100*t->weight));
      
      unicast_message_to_send.message = message_to_forward.message;
      unicast_message_to_send.type = UNICAST_TYPE_MESSAGE;
      unicast_message_to_send.tag = tag;
      unicast_target = t->addr;
      unicast_wait_time = 0;
      unicast_ready_to_send = true;

      }
}
/*---------------------------------------------------------------------------*/



//BROADCAST RECEIVE
/* This function is called whenever a broadcast message is received. */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
  struct broadcast_message *msg;
  
  /* The packetbuf_dataptr() returns a pointer to the first data byte
     in the received packet. */
  msg = packetbuf_dataptr();
  
  switch (msg->type) {
      
    case BROADCAST_APPLY_TAG :
  
      if ((int)node_id != DATABASE){
        tag = msg->tag;
        //printf("Asked to apply tag %d\n",tag);
        leds_on(LEDS_RED);
      }
    break;
      
    case BROADCAST_COMPUTE_WEIGHT :
      
      if (!broadcast_ready_to_send){
            if ((int)node_id != DATABASE){
                  broadcast_message_to_send.type = BROADCAST_ASK_TAG;
                  broadcast_message_to_send.tag = msg->tag;
                  broadcast_ready_to_send = true;
                  broadcast_wait_time = 1.0;
            }
            //printf("Asked to compute weight\n");
            
            weight_target = *from;
            weight_tag = msg->tag;
            state = COMPUTING_TAGS;
     }      
    break;
      
    case BROADCAST_ASK_TAG :
    
      if (!broadcast_ready_to_send){
            
            broadcast_message_to_send.type = BROADCAST_REPLY_TAG;
            broadcast_message_to_send.tag = tag;
            broadcast_ready_to_send = true;
            broadcast_wait_time = 3.0;
            //printf("Asked if tagged\n");
      } 
    break;  
    
    case BROADCAST_REPLY_TAG :
    
      add_neighbor(msg->tag, 0, from);
      
    break;
    
   }  
}
/* This is where we define what function to be called when a broadcast
   is received. We pass a pointer to this structure in the
   broadcast_open() call below. */
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/



// UNICAST RECEIVE
/* This function is called for every incoming unicast packet. */
static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
  struct unicast_message *msg;

  /* Grab the pointer to the incoming data. */
  msg = packetbuf_dataptr();

  switch(msg->type){
  
    case UNICAST_TYPE_MESSAGE :

	    printf("Unicast ping received from %d.%d\n",
		   from->u8[0], from->u8[1]);
	   
	    if ((int)node_id != DATABASE) {
	      visited++;
	      tag = msg->tag;
	      message_to_forward.message = msg->message;
	      
	      broadcast_weight();
	      state = COMPUTING_WEIGHTS;

	    } else {
	      printf("Message has reached the database!\n");
	      printf("message is: %s\n", msg->message);
	      leds_on(LEDS_ALL);
	    }       
	    
    break;
    
    case UNICAST_REPLY_WEIGHT :
      add_neighbor(msg->tag, msg->weight, from);
      
    break;

    }
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
/*---------------------------------------------------------------------------*/


//PROCESS DEFINITION : BROADCAST
PROCESS_THREAD(broadcast_process, ev, data)
{
  static struct etimer et;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  broadcast_open(&broadcast, 129, &broadcast_call);
  
  while(1){
        etimer_set(&et,CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  
      if (broadcast_ready_to_send){
        etimer_set(&et, broadcast_wait_time * CLOCK_SECOND + CLOCK_SECOND * 0.1 * (int)node_id);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        print_info(broadcast_message_to_send.type, unicast_target);
        
        packetbuf_copyfrom(&broadcast_message_to_send, sizeof(struct broadcast_message));
        broadcast_send(&broadcast);
        broadcast_ready_to_send = false;
      }
  }
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/


//PROCESS DEFINITION: UNICAST
PROCESS_THREAD(unicast_process, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&unicast);)
    
  PROCESS_BEGIN();

  unicast_open(&unicast, 146, &unicast_callbacks);


  static struct etimer et;
        
  while(1){
        etimer_set(&et,CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  
      if (unicast_ready_to_send){
        etimer_set(&et, unicast_wait_time * CLOCK_SECOND + CLOCK_SECOND * 0.1 * (int)node_id);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
      
        print_info(unicast_message_to_send.type, unicast_target);
        
        packetbuf_copyfrom(&unicast_message_to_send, sizeof(struct unicast_message));
        unicast_send(&unicast, &unicast_target);
        unicast_ready_to_send = false;
      }
      
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
//PROCESS DEFINITION: DRW
PROCESS_THREAD(drw, ev, data)
{
  
    
  PROCESS_BEGIN();

  state = HIDLE;
  list_init(neighbors_list);
  broadcast_ready_to_send = false;
  unicast_ready_to_send = false;
  visited = 0;
  tag = (int)node_id;
  leds_off(LEDS_ALL);

  static struct etimer et;
  
  // if ((int)node_id == SENDER) state = NEW_MESSAGE;

   while(1){
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	
	/* Sense temperature */
	/*
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
	*/

	// Create a temperature notification and send
	// SEND( "{\"status\":\"OK\", \"infos\":{\"temperature\":\"%c%d.%04d\"}}", minus, tempint, tempfrac);

	char data[32];
	sprintf(data, "%c%d.%04d", '-', 25, 99 );
	struct NotifyMessage msg;
	memset(&msg, 0, sizeof(msg));
	msg.measurementType = MSR_TEMPERATURE;
	msg.dataType        = TYPE_DOUBLE;
	msg.unit            = UNT_CELSIUS;
	msg.id              = g_id;
	msg.data            = data;
	msg.dataSize        = strlen(data);
	sendNotificationSerial(&msg);





	continue;



        
      if (state == NEW_MESSAGE){
        
        etimer_set(&et, CLOCK_SECOND * 2);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        printf("Starting the DRW\n");
        broadcast_tag();
        leds_on(LEDS_RED);
        
        etimer_set(&et, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
        broadcast_weight();
        
        leds_on(LEDS_BLUE);
        list_init(neighbors_list);
            
        etimer_set(&et, CLOCK_SECOND * 12);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
        message_to_forward.message = "there is fire in room 22";
        unicast_forward();
        
        state = HIDLE;
        leds_off(LEDS_BLUE);
      
      } else if(state == COMPUTING_TAGS){
      
            leds_on(LEDS_GREEN);
            list_init(neighbors_list);
      
            etimer_set(&et, CLOCK_SECOND * 6 + CLOCK_SECOND * 0.1 * (int)node_id);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
              
            reply_weight();
              
            state = HIDLE;
            leds_off(LEDS_GREEN);
            
      } else if (state == COMPUTING_WEIGHTS){
      
            leds_on(LEDS_BLUE);
            list_init(neighbors_list);
            
            etimer_set(&et, CLOCK_SECOND * 12 + CLOCK_SECOND * 0.1 * (int)node_id);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
            
            unicast_forward();
            broadcast_tag();
            
            state = HIDLE;
            leds_off(LEDS_BLUE);
      }
  }
  
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/****************************/
/********* RECEIVE MSG FROM GATEWAY **********/
/****************************/


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
					handleSubscription();
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
	// To be written
	
}

/*
 * Handle notification messages from gateway
 */
void handleSubscription(const char* data)
{
	struct SubscribeMessage msg;
	memset(&msg, 0, sizeof(msg));
	if(unbufferizeSubscribeMessage(&msg, data) <= 0)
		ERROR("Cannot read message from buffer");

	// Define here what to do on reception 
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
	msg.id              = g_id;
	msg.data            = data;
	msg.dataSize        = strlen(data);
	sendNotificationSerial(&msg);
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
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg);
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
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg);
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
		msg.id              = g_id;
		msg.data            = data;
		msg.dataSize        = strlen(data);
		sendNotificationSerial(&msg);
	}
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

		if (push % 2 == 0) { 
			leds_toggle(LEDS_ALL);
			LOG("Button pressed [%d] TURNING OFF ALL LEDS ... [DONE]", push);
			push++;
		} else {
			leds_toggle(LEDS_ALL);
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

