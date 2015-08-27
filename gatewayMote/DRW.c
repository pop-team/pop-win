#include "contiki.h"
#include "lib/list.h"
#include "queue.h"
#include "lib/memb.h"
#include "lib/random.h"
#include "net/rime.h"
#include "node-id.h"
#include <stdbool.h>
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include "dev/cc2420.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/light.h"
#include "dev/sht11.h"
// #include "dev/tmp102.h"

#include "DRW.h"

/*---------------------------------------------------------------------------*/
/* We first declare our three processes. */

// Commented these lines: We use the version in gatewayMote.c

// PROCESS(communication_process, "Communication process");
// PROCESS(drw, "Directional Random Walk");

/* The AUTOSTART_PROCESSES() definition specifices what processes to
   start when this module is loaded. We put our processes there. */
// AUTOSTART_PROCESSES(&communication_process, &drw);
/*---------------------------------------------------------------------------*/


// PRINT INFO
/* print information when a message is sent */
static void print_info(uint8_t type) {

	if (printConsole){
		switch (type){

			case UNICAST_TYPE_MESSAGE :
				printf("Sending message to %d\n", unicast_target.u8[0]);
				break;

			case UNICAST_TYPE_WEIGHT :
				printf("Replying weight to %d\n", weight_target.u8[0]);
				break;

			case BROADCAST_MAKE_CANDIDATE :
				printf("Asking neighbors' weights\n");
				break;

			case BROADCAST_APPLY_TAG :
				printf("Asking neighbors to apply tag\n");
				break;

			case BROADCAST_ASK_TAG :
				printf("Asking neighbors' tags\n");
				break;

			case BROADCAST_REPLY_TAG :
				printf("Replying about the tag\n");
				break;
		}
	}
}

/*---------------------------------------------------------------------------*/

// Get wait time
/* return different wait time depending on message type */
static float getWaitTime(uint8_t type) {

	switch (type){

		case UNICAST_TYPE_MESSAGE :
			return 1.0;
			break;

		case UNICAST_TYPE_WEIGHT :
			return 1.0;
			break;

		case BROADCAST_MAKE_CANDIDATE :
			return 3.0;
			break;

		case BROADCAST_APPLY_TAG :
			return 1.0;
			break;

		case BROADCAST_ASK_TAG :
			return 1.0;
			break;

		case BROADCAST_REPLY_TAG :
			return 4.0;
			break;
	}
	return 1.0;
}


/*---------------------------------------------------------------------------*/

// MESSAGE TO SEND USING THE RADIO

char*
create_messagetosend(struct Message msg){

	char *message = (char*)malloc(strlen(msg.message_string) + 7);

	message[0] = msg.type;
	message[1] = msg.tag;
	message[2] = msg.message;
	message[3] = msg.nodeid;
	message[4] = msg.value;
	message[5] = msg.weight;

	printf("Create Message msg.message_string is: %s\n", msg.message_string);

	memcpy(message + 6, msg.message_string, strlen(msg.message_string));

	message[strlen(msg.message_string)+7] = '\0';

	return message;
}

struct Message
extract_messagetosend(const char* message){

	printf("Extract message string received msg [0] is: %u\n", (uint8_t)message[0]);
	printf("Extract message string received msg [1] is: %u\n", (uint8_t)message[1]);
	printf("Extract message string received msg [2] is: %u\n", (uint8_t)message[2]);
	printf("Extract message string received msg [3] is: %u\n", (uint8_t)message[3]);
	printf("Extract message string received msg [4] is: %u\n", (uint8_t)message[4]);
	printf("Extract message string received msg [5] is: %u\n", (uint8_t)message[5]);
	printf("Extract message string received msg [6] is: %u\n", (char)message[6]);
	printf("Extract message string received msg [7] is: %u\n", (char)message[7]);
	printf("Extract message string received msg [8] is: %u\n", (char)message[8]);
	printf("Extract message string received msg [9] is: %u\n", (char)message[9]);
	printf("Extract message string received msg [10] is: %u\n", (char)message[10]);
	printf("Extract message string received msg [11] is: %u\n", (char)message[11]);
	printf("Extract message string received msg [12] is: %u\n", (char)message[12]);
	printf("Extract message string received msg [13] is: %u\n", (char)message[13]);

	printf("Extract message character received msg [6] is: %u\n", (char)message[6]);
	printf("Extract message character received msg [7] is: %u\n", (char)message[7]);
	printf("Extract message character received msg [8] is: %u\n", (char)message[8]);
	printf("Extract message character received msg [9] is: %u\n", (char)message[9]);
	printf("Extract message character received msg [10] is: %u\n", (char)message[10]);
	printf("Extract message character received msg [11] is: %u\n", (char)message[11]);
	printf("Extract message character received msg [12] is: %u\n", (char)message[12]);
	printf("Extract message character received msg [13] is: %u\n", (char)message[13]);

	struct Message msgextract; 
	memset(&msgextract, 0, sizeof(msgextract));

	msgextract.type = (uint8_t)message[0];
	msgextract.tag = (uint8_t)message[1];
	msgextract.message = (uint8_t)message[2];
	msgextract.nodeid = (uint8_t)message[3];
	msgextract.value = (uint8_t)message[4];
	msgextract.weight = (uint8_t)message[5];

	int i=0;

	for (i=6;i<6+64; ++i)
	{
		msgextract.message_string[i-6] = message[i];
	}

	// printf("EXTRACTED: %s\n", msgextract.message_string);

	return msgextract;
}


/*---------------------------------------------------------------------------*/

// SEND BROADCAST
/* broadcast different types of messages */
static void send_broadcast(uint8_t type){

	printf("Send broadcast message\n");
	struct Message m = createMessage();
	m.type = type;
	m.tag = tag;
	printf("push broadcast %d %d\n", m.message, __LINE__);
	message_queue.push(&message_queue, m);
}

/*---------------------------------------------------------------------------*/

//Compute and Send weight
/* sends the computed weight to the asking mote */
static void send_weight(){

	float w = 0.0;
	int total = 0;
	bool dbb = false;

	struct neighbor *n;
	for(n = list_head(neighbors_list); n != NULL; n = list_item_next(n)) {
		total++;
		if (n->tag == 1) w++;
		if (n->tag == DATABASE_TAG) {
			dbb = true;
		}
	}

	if ((int)node_id == DATABASE) w = 0.0;	//weight is 0 if database
	else {
		if (tag == 1) w++;	//if node is part or neighborhood of DRW
		w /= (total+1);	//total + 1 to count the node itself
		if (dbb) w = 0.1;		// weight is 1 if linked to database
	}

	printf("weight is: %u, total neighbours: %d\n", (uint8_t)(10*w), total);

	unicast_target = weight_target;

	struct Message m;
	memset(&m, 0, sizeof(m));
	m.weight = (uint8_t)(10*w);
	m.type = UNICAST_TYPE_WEIGHT;
	m.tag = tag;
	snprintf(m.message_string, sizeof(m.message_string), "Empty");
	printf("push weights %d %d\n", m.message, __LINE__);
	message_queue.push(&message_queue, m);

}
/*---------------------------------------------------------------------------*/


// ADD NEIGHBOR
/* Add a neighbor on the mote's neighbor list */
static void add_neighbor(uint8_t ntag, uint8_t nweight, const rimeaddr_t *from){

	struct neighbor *n;

	/* Check if we already know this neighbor. */
	for(n = list_head(neighbors_list); n != NULL; n = list_item_next(n)) {

		/* We break out of the loop if the address of the neighbor matches
		   the address of the neighbor from which we received this
		   broadcast message. */
		if(rimeaddr_cmp(&n->addr, from)) {
			n->weight = nweight;
			n->tag = ntag;
			printf("Updating neighbor %d\n", from->u8[0]);
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
			printf("ERROR: cannot allocate a new neighbor\n");
			return;
		}

		/* Initialize the fields. */
		rimeaddr_copy(&n->addr, from);
		n->weight = nweight;
		n->tag = ntag;

		/* Place the neighbor on the neighbor list. */
		list_add(neighbors_list, n);

		printf("Added neighbor with address %d and tag %d\n",from->u8[0], ntag);
	}


}

/*---------------------------------------------------------------------------*/


//Forward message
/* This is the function that does the routing to the next mote in the DRW when the DRW is in construction*/
static void forward_message(const struct Message *msg){

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

		printf("Forwarding unicast to %d.%d with weight %u\n", t->addr.u8[0], t->addr.u8[1], t->weight);

		unicast_target = t->addr;

		struct Message m;
		memset(&m, 0, sizeof(m));
		m.message = msg->message; printf("Message %u\n", m.message);
		m.value = msg->value; printf("Value %u\n", m.value);
		m.nodeid = msg->nodeid; printf("Nodeid %u\n", m.nodeid);
		m.type = UNICAST_TYPE_MESSAGE; printf("Type %u\n", m.type);
		m.tag = tag; printf("Message %u\n", m.tag);
		strcpy(m.message_string, msg->message_string); 

		printf("push forward msg %d %d\n", m.message, __LINE__);
		message_queue.push(&message_queue, m);
	}
	else printf("ERROR: cannot forward message, no neighbor found\n");
}
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/



//BROADCAST RECEIVE
/* This function is called whenever a broadcast message is received. */
	static void // TODO : check why broadcast_recv would be duplicated in gatewayMote.c, 
		    // why DRW.c included in gatewayMote.c
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	struct Message *msg;

	/* The packetbuf_dataptr() returns a pointer to the first data byte
	   in the received packet. */
	msg = packetbuf_dataptr();
	printf("Received broadcast message\n");

	switch (msg->type) {

		case BROADCAST_APPLY_TAG :

			printf("Asked to apply tag %d\n",msg->tag);
			if ((int)node_id != DATABASE)tag = msg->tag;
			//leds_on(LEDS_RED);

			break;

		case BROADCAST_MAKE_CANDIDATE :

			printf("Empty the list of neighbors\n");
			list_init(neighbors_list);
			weight_target = *from;
			add_neighbor(msg->tag, 0, from);	//add the current note of DRW
			state = CANDIDATE_NODE;

			break;

		case BROADCAST_ASK_TAG :

			if (state != CURRENT_DRW_NODE && state != NEW_MESSAGE)
				send_broadcast(BROADCAST_REPLY_TAG);
			tag_asked = true;

			break;

		case BROADCAST_REPLY_TAG :

			if (state == CANDIDATE_NODE)
				add_neighbor(msg->tag, 0, from);

			break;

	}
}
/* This is where we define what function to be called when a broadcast
   is received. We pass a pointer to this structure in the
   broadcast_open() call below. */
//static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/



// UNICAST RECEIVE
/* This function is called for every incoming unicast packet. */
	static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{

	/* Grab the pointer to the incoming data. */
	char* msgrec;
	msgrec = packetbuf_dataptr();

	printf("Message string received msg [0] is: %u\n", (uint8_t)msgrec[0]);
	printf("Message string received msg [1] is: %u\n", (uint8_t)msgrec[1]);
	printf("Message string received msg [2] is: %u\n", (uint8_t)msgrec[2]);
	printf("Message string received msg [3] is: %u\n", (uint8_t)msgrec[3]);
	printf("Message string received msg [4] is: %u\n", (uint8_t)msgrec[4]);
	printf("Message string received msg [5] is: %u\n", (uint8_t)msgrec[5]);
	printf("Message string received msg [6] is: %u\n", (char)msgrec[6]);
	printf("Message string received msg [7] is: %u\n", (char)msgrec[7]);
	printf("Message string received msg [8] is: %u\n", (char)msgrec[8]);
	printf("Message string received msg [9] is: %u\n", (char)msgrec[9]);
	printf("Message string received msg [10] is: %u\n", (char)msgrec[10]);
	printf("Message string received msg [11] is: %u\n", (char)msgrec[11]);
	printf("Message string received msg [12] is: %u\n", (char)msgrec[12]);
	printf("Message string received msg [13] is: %u\n", (char)msgrec[13]);

	printf("Message character received msg [6] is: %u\n", (char)msgrec[6]);
	printf("Message character received msg [7] is: %u\n", (char)msgrec[7]);
	printf("Message character received msg [8] is: %u\n", (char)msgrec[8]);
	printf("Message character received msg [9] is: %u\n", (char)msgrec[9]);
	printf("Message character received msg [10] is: %u\n", (char)msgrec[10]);
	printf("Message character received msg [11] is: %u\n", (char)msgrec[11]);
	printf("Message character received msg [12] is: %u\n", (char)msgrec[12]);
	printf("Message character received msg [13] is: %u\n", (char)msgrec[13]);

	message_to_forward = extract_messagetosend(msgrec);


	printf("RECEIVED Type is: %u\n", message_to_forward.type);
	printf("RECEIVED Nodeid is: %u\n", message_to_forward.nodeid);
	printf("RECEIVED Message is: %u\n", message_to_forward.message);
	printf("RECEIVED Value is: %u\n", message_to_forward.value);
	printf("RECEIVED Nodeid of publisher is: %u\n", message_to_forward.nodeid);

	switch(message_to_forward.type){

		case UNICAST_TYPE_MESSAGE :

			printf("Unicast ping received from %d.%d\n",
					from->u8[0], from->u8[1]);

			state = CURRENT_DRW_NODE;

			break;

		case UNICAST_TYPE_WEIGHT :

			printf("Received unicast message: adding neighbor\n");

			add_neighbor(message_to_forward.tag, message_to_forward.weight, from);

			break;

	}
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
/*---------------------------------------------------------------------------*/

uint16_t sense_light(){
	SENSORS_ACTIVATE(light_sensor);
	unsigned int value = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
#ifdef EN_DEBUG
	DEBUG("Light value: %u", value);
#endif
	SENSORS_DEACTIVATE(light_sensor);

	return value;
}

uint16_t sense_infrared(){
	SENSORS_ACTIVATE(light_sensor);
	unsigned int value = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
#ifdef EN_DEBUG
	DEBUG("Infrared value: %u", value);
#endif
	SENSORS_DEACTIVATE(light_sensor);

	return value;
}

uint16_t sense_temperature(){
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
	// note: in the present situation, all messages are sent as 8 bits integers for simplicity
	DEBUG("Temp %d %d %d  --> %d %d %d", sign, raw, absraw, tempint, tempfrac, (int)minus);
	 */
	// Create a temperature notification and send
	// SEND( "{\"status\":\"OK\", \"infos\":{\"temperature\":\"%c%d.%04d\"}}", minus, tempint, tempfrac);

	unsigned int value = (unsigned int) (-39.60 + 0.01 * sht11_temp());
#ifdef EN_DEBUG
	DEBUG("sense temperature %u", value);
#endif
	return value;
}

float sense_temperature_float(){
	/* note: this code is unstable on z1 (which uses tmp102 sensor) and unstable on xm1000
	   unsigned u = sht11_temp();
	   float value = 0;
	   if(u == 65535)
	   {
	   int16_t tempint;
	   uint16_t tempfrac;
	   int16_t raw;
	   uint16_t absraw;
	   int16_t sign;
	   char minus = ' ';

	   sign = 1;
	   raw = tmp102_read_temp_raw();
	   absraw = raw;
	   if(raw < 0) {               // Perform 2C's if sensor returned negative data
	   absraw = (raw ^ 0xFFFF) + 1;
	   sign = -1;
	   }
	   tempint = (absraw >> 8) * sign;
	   tempfrac = ((absraw >> 4) % 16) * 625;      // Info in 1/10000 of degree
	   minus = ((tempint == 0) & (sign == -1)) ? '-' : ' ';
	   DEBUG("Temp = %c%d.%04d", minus, tempint, tempfrac);
	   value = tempint + tempfrac * 0.01;
	   if(sign == -1)
	   value *= -1;
	   }
	   else
	   value = -39.60 + 0.01 * sht11_temp();
	 */
	float value = -39.60 + 0.01 * sht11_temp();
#ifdef EN_DEBUG
	DEBUG("sense temperature %d.%u (%u)", (int)value, DEC(value), sht11_temp());
#endif
	return value;
}

uint16_t sense_humidity(){
	unsigned int rh = sht11_humidity();
	unsigned int value = -4 + 0.0405*rh - 2.8e-6*(rh*rh);
#ifdef EN_DEBUG
	DEBUG("sense humidity %u percent (%u)", value, rh);
#endif
	return value;
}

float sense_humidity_float(){
	unsigned int rh = sht11_humidity();
	float value = -4 + 0.0405*rh - 2.8e-6*(rh*rh);
#ifdef EN_DEBUG
	DEBUG("sense humidity %d.%u percent (%u)", (int)value, DEC(value), rh);
#endif
	return value;
}

void send_event(char *message, uint8_t event){

	if ((int)node_id == SENDER){
		LOG("Sending message event: %s", (const char *)message);
		char buf[16];
		sprintf(buf, "%s", message);
		printf("Message string event is: %s\n", buf);
		uint8_t typemessage = event;
		message_to_forward.message = typemessage;
		message_to_forward.nodeid = node_id;
		message_to_forward.value = 0;


		snprintf(message_to_forward.message_string, sizeof(message_to_forward.message_string), message);

		printf("SEND EVENT Message string 0 event is: %c, %c\n", message_to_forward.message_string[0], message[0]);
		printf("SEND EVENT Message string 1 event is: %c, %c\n", message_to_forward.message_string[1], message[1]);
		printf("SEND EVENT Message string 2 event is: %c, %c\n", message_to_forward.message_string[2], message[2]);
		printf("SEND EVENT Message string 3 event is: %c, %c\n", message_to_forward.message_string[3], message[3]);
		printf("SEND EVENT Message string 4 event is: %c, %c\n", message_to_forward.message_string[4], message[4]);
		printf("SEND EVENT Message string 5 event is: %c, %c\n", message_to_forward.message_string[5], message[5]);

		state = SEND_EVENT;
	}
}


//Communication Process
PROCESS_THREAD(communication_process, ev, data)
{
	static struct etimer et;

	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
		PROCESS_EXITHANDLER(unicast_close(&unicast);)

		PROCESS_BEGIN();

	broadcast_open(&broadcast, 129, &broadcast_call);
	unicast_open(&unicast, 146, &unicast_callbacks);

	float waitTime;

	while(1){
		etimer_set(&et,CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		if (message_queue.size > 0){
			struct Message message_to_send;
			memset(&message_to_send, 0, sizeof(message_to_send));
			message_to_send = message_queue.pop(&message_queue); // TODO: Use memcpy
			printf("pop %d %d\n", message_to_send.message, __LINE__);

			waitTime = getWaitTime(message_to_send.type);

			etimer_set(&et, waitTime * CLOCK_SECOND + CLOCK_SECOND * 0.1 * (int)node_id);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			//Unicast message
			if ((message_to_send.type == UNICAST_TYPE_MESSAGE) ||
					(message_to_send.type == UNICAST_TYPE_WEIGHT)){
				printf("Sending unicast message\n");

				print_info(message_to_send.type);

				char sending[64]; //  = (char*)malloc(strlen(message_to_send.message_string) + 7);

				printf("Sending unicast message before package with data %s\n", message_to_send.message_string);

				printf("Message to send nodeid %u\n", message_to_send.nodeid);

				printf("Sending unicast message %u %u\n", message_to_send.message, message_to_send.type);
				printf("Sending unicast message %s\n", message_to_send.message_string);

				char* tmp = create_messagetosend(message_to_send);
				memcpy(sending, tmp, strlen(message_to_send.message_string) + 6 + 1);

				printf("Sending unicast message with data sending [0] %u %u\n", sending[0], tmp[0]);
				printf("Sending unicast message with data sending [1] %u %u\n", sending[1], tmp[1]);
				printf("Sending unicast message with data sending [2] %u %u\n", sending[2], tmp[2]);
				printf("Sending unicast message with data sending [3] %u %u\n", sending[3], tmp[3]);
				printf("Sending unicast message with data sending [4] %u %u\n", sending[4], tmp[4]);
				printf("Sending unicast message with data sending [5] %u %u\n", sending[5], tmp[5]);
				printf("Sending unicast message with data sending [6] %u\n", sending[6]);
				printf("Sending unicast message with data sending [7] %u\n", sending[7]);
				printf("Sending unicast message with data sending [8] %u\n", sending[8]);
				printf("Sending unicast message with data sending [9] %u\n", sending[9]);
				printf("Sending unicast message with data sending [6] %c\n", sending[6]);
				printf("Sending unicast message with data sending [7] %c\n", sending[7]);
				printf("Sending unicast message with data sending [8] %c\n", sending[8]);
				printf("Sending unicast message with data sending [9] %c\n", sending[9]);
				printf("Sending unicast message with data sending [10] %c\n", sending[10]);
				printf("Sending unicast message with data sending [11] %c\n", sending[11]);	

				packetbuf_copyfrom(sending, strlen((const char*)message_to_send.message_string) + 6 + 1);
				unicast_send(&unicast, &unicast_target);

				free(tmp);
			}

			//Broadcast message
			else {
				printf("Sending broadcast message\n");
				print_info(message_to_send.type);

				packetbuf_copyfrom(&message_to_send, sizeof(struct Message));
				broadcast_send(&broadcast);
				if (message_to_send.type == BROADCAST_REPLY_TAG) tag_asked = false;
			}
		}
	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
//PROCESS DEFINITION: Poll the sensors and check if events happened
PROCESS_THREAD(sensor_events, ev, data)
{

	PROCESS_BEGIN();

	// Initialization of sensor
	sht11_init();
	// tmp102_init();

	static struct etimer et;

	while(1){
		etimer_set(&et, 3 * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		static char event_hot = 0;
		uint8_t temp = sense_temperature();

		if(temp >= 40u){
			if(event_hot == 0){
				send_event("It is hot !", MSR_EVENT);
				event_hot = 1;
				event_sent = 1;
			}
		}
		else event_hot = 0;

		static char event_humid = 0;
		uint8_t humid = sense_humidity();

		if(humid > 60u){
			if(event_humid == 0){
				send_event("It is wet !", MSR_EVENT);
				event_humid = 1;
				event_sent = 1;
			}
		}
		else event_humid = 0;

		static char event_dark = 0;
		uint8_t light = sense_light();

		if(light < 20u){
			if(event_dark == 0){
				send_event("It is dark !", MSR_EVENT);
				event_dark = 1;
				event_sent = 1;
			}
		}
		else event_dark = 0;

	}
	PROCESS_END();

}

/*---------------------------------------------------------------------------*/
//PROCESS DEFINITION: DRW
PROCESS_THREAD(drw, ev, data)
{


	PROCESS_BEGIN();

	state = IDLE;
	list_init(neighbors_list);
	message_queue = createQueue();

	visited = 0;
	tag = 0;	//not part of DRW
	if ((int)node_id == DATABASE) tag = DATABASE_TAG;
	//leds_off(LEDS_ALL);

	static struct etimer et;

	if ((int)node_id == SENDER) state = NEW_MESSAGE;
	cc2420_set_txpower(POWER);

	while(1){
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		//TO READ FROM THE SERIAL IF WE RECEIVE A GW_NOTIFICATION TO GO TO THE APPROPIATE STATE


		if (state == NEW_MESSAGE){
			struct Message message;
			memset(&message, 0, sizeof(message));

			etimer_set(&et, CLOCK_SECOND * 2);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			printf("Starting the DRW\n");
			//leds__on(LEDS_RED);
			//leds_on(LEDS_BLUE);

			tag = 1;
			send_broadcast(BROADCAST_APPLY_TAG);

			etimer_set(&et, CLOCK_SECOND * 1);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			send_broadcast(BROADCAST_MAKE_CANDIDATE);

			etimer_set(&et, CLOCK_SECOND * 30);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			// Sense one measure and send
			if(sense_counter % 3 == 0){
				uint8_t typemessage = MSR_LIGHT;
				message.message = typemessage;
				message.value = sense_light();
			}
			else if(sense_counter % 3 == 1){
				uint8_t typemessage = MSR_TEMPERATURE;
				message.message = typemessage;
				message.value = sense_temperature();
			}
			else{
				uint8_t typemessage = MSR_HUMIDITY;
				message.message = typemessage;
				message.value = sense_humidity();
			}
			sense_counter++;

			message.nodeid = node_id;	
			snprintf(message.message_string, sizeof(message.message_string), "Empty");
			printf("Message string  when sensing is: %s\n", message.message_string);

			forward_message(&message);
			//leds_off(LEDS_BLUE);
			//leds_toggle(LEDS_ALL);

			state = IDLE;


		} else if (state == CURRENT_DRW_NODE){

			// Id DATABASE is the id of the gateway
			if ((int)node_id == DATABASE) {
				//leds_on(LEDS_RED);
				printf("Message has reached the gateway!\n");
				printf("Message is: %u\n", message_to_forward.message);
				printf("Value is: %u\n", message_to_forward.value);
				char buff[16];
				sprintf(buff, "%s", message_to_forward.message_string);
				printf("Message string is: %s\n", buff);
				printf("Nodeid of publisher is: %u\n", message_to_forward.nodeid);

				// Send a notification to POP-C++
				struct NotifyMessage msg;
				memset(&msg, 0, sizeof(msg));
				msg.measurementType = message_to_forward.message;

				if(message_to_forward.message == MSR_EVENT)
				{
					snprintf(msg.data, sizeof(msg.data), "%s", message_to_forward.message_string);
					msg.dataType        = TYPE_STRING;
				}else
				{
					snprintf(msg.data, sizeof(msg.data), "%d", message_to_forward.value);
					msg.dataType        = TYPE_INT;
				}
				msg.id              = message_to_forward.nodeid;
				msg.dataSize        = strlen(msg.data);
				gwSendNotificationSerial(&msg);
				// printf("message sent to gw %d\n",message_to_forward.message);

				state=IDLE;
				//leds_off(LEDS_RED);

			} else if (event_sent == 1 && message_to_forward.message == 100) {
				// if an event is present and message == 100 (an event is produced)

				printf("Message has reached the node that sent a string event!\n");
				printf("Message is: %u\n", message_to_forward.message);
				printf("Value is: %u\n", message_to_forward.value);
				char buff[16];
				sprintf(buff, "%s", (char *)message_to_forward.message_string);
				printf("Message string is: %s\n", buff);
				printf("Nodeid of publisher is: %u\n", message_to_forward.nodeid);

				//Toggle the LEDs
				//leds_toggle(LEDS_ALL);
				//leds_toggle(LEDS_ALL);

				event_sent  = 0;

				state=IDLE;
				//leds_on(LEDS_ALL);

			}else {

				visited++;
				struct Message sendevent;
				memcpy(&sendevent, &message_to_forward, sizeof(message_to_forward)); // copy as a safety

				//leds_on(LEDS_BLUE);
				list_init(neighbors_list);

				send_broadcast(BROADCAST_MAKE_CANDIDATE);

				etimer_set(&et, CLOCK_SECOND * 25 + random_rand() % (CLOCK_SECOND * 10));
				PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


				forward_message(&sendevent);
				send_broadcast(BROADCAST_APPLY_TAG);

				state = IDLE;
				//leds_off(LEDS_BLUE);
			}

		} else if(state == CANDIDATE_NODE){

			//leds_on(LEDS_GREEN);

			send_broadcast(BROADCAST_ASK_TAG);

			etimer_set(&et, CLOCK_SECOND * 8 + random_rand() % (CLOCK_SECOND * 10));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			send_weight();

			state = IDLE;
			//leds_off(LEDS_GREEN);

		} else if(state == SEND_EVENT){

			static struct etimer et;
			struct Message sendevent;
			memcpy(&sendevent, &message_to_forward, sizeof(message_to_forward));

			etimer_set(&et, CLOCK_SECOND * 2);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			printf("Starting the DRW for event\n");
			//leds_on(LEDS_RED);
			//leds_on(LEDS_BLUE);
			tag = 1;
			send_broadcast(BROADCAST_APPLY_TAG);

			etimer_set(&et, CLOCK_SECOND * 1);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			send_broadcast(BROADCAST_MAKE_CANDIDATE);

			etimer_set(&et, CLOCK_SECOND * 30);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			forward_message(&sendevent);
			//leds_off(LEDS_BLUE);
			//leds_toggle(LEDS_ALL);

			state = IDLE;
		} else if(state == GW_NOTIFICATION){

			static struct etimer et;

			etimer_set(&et, CLOCK_SECOND * 2);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			printf("Starting the DRW for toggle LEDs due to event\n");
			//leds_on(LEDS_RED);
			//leds_on(LEDS_BLUE);
			tag = 1;
			send_broadcast(BROADCAST_APPLY_TAG);

			etimer_set(&et, CLOCK_SECOND * 1);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			send_broadcast(BROADCAST_MAKE_CANDIDATE);

			etimer_set(&et, CLOCK_SECOND * 30);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


			message_to_forward.message = 100;
			message_to_forward.value = 100;			
			message_to_forward.nodeid = node_id;	
			snprintf(message_to_forward.message_string, sizeof(message_to_forward.message_string), "Toggle");
			forward_message(&message_to_forward);
			//leds_off(LEDS_BLUE);
			//leds_toggle(LEDS_ALL);

			state = IDLE;
		}
	}

	PROCESS_END();
}
