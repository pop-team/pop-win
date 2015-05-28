#ifndef _DRW_H_
#define _DRW_H_


/* This #define defines the maximum amount of neighbors we can remember. */
#define MAX_NEIGHBORS 16
/* This #define defines the node id corresponding to the database */
#define DATABASE 2 // Note: Set your gateway id here !!
#define DATABASE_TAG UINT8_MAX
/* This #define defines the node id corresponding to the sender */
#define SENDER 1
#define POWER 3

// Helper to print the decimal of a value
#define DEC(x) ((unsigned)(x*100 - ((int) x) * 100))



static bool printConsole = true;

/* Mote global variables */
static uint8_t state;
static uint8_t tag;     //should be a list
static bool tag_asked = false;
static int visited;
static int event_sent = 0;
static int sense_counter = 0;

static struct Message message_to_forward;
static struct Message message_to_send;
static rimeaddr_t unicast_target;
static rimeaddr_t weight_target;

static Queue message_queue;

uint16_t sense_light();
uint16_t sense_infrared();
uint16_t sense_humidity();
uint16_t sense_temperature();
float sense_humidity_float();
float sense_temperature_float();

/* These are the types of unicast messages that we can send. */
enum {
	UNICAST_TYPE_MESSAGE,       //message to forward to database
	UNICAST_TYPE_WEIGHT,       //information about a mote weight
	BROADCAST_APPLY_TAG,        //telling neihbors they are tagged 
	BROADCAST_MAKE_CANDIDATE,   //telling neighbors to compute weight    
	BROADCAST_ASK_TAG,          //asking neighbors if they are tagged
	BROADCAST_REPLY_TAG         //message telling if tagged

};

/* These are the states in which a mote can be during the DRW */
enum {
	IDLE,
	NEW_MESSAGE,
	CURRENT_DRW_NODE,
	CANDIDATE_NODE,
	SEND_EVENT,
	GW_NOTIFICATION

};

/* This structure holds information about neighbors. */
struct neighbor {

	struct neighbor *next;
	rimeaddr_t addr;
	uint8_t tag;
	uint8_t weight;             // #neighbors tagged / total neihbors

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

#endif
