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
#include "dev/cc2420.h"

/* This #define defines the maximum amount of neighbors we can remember. */
#define MAX_NEIGHBORS 16
/* This #define defines the node id corresponding to the database */
#define DATABASE 2
#define DATABASE_TAG UINT8_MAX
/* This #define defines the node id corresponding to the sender */
#define SENDER 1
#define POWER 3

static bool printConsole = true;

/* Mote global variables */
static uint8_t state;
static uint8_t tag;     //should be a list
static bool tag_asked = false;
static int visited;

static struct Message message_to_forward;
static struct Message message_to_send;
static rimeaddr_t unicast_target;
static rimeaddr_t weight_target;

static Queue message_queue;

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
  CANDIDATE_NODE  
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

// SEND BROADCAST
/* broadcast different types of messages */
static void send_broadcast(uint8_t type){
      
      struct Message m = createMessage();
      m.type = type;
      m.tag = tag;
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
      m.weight = (uint8_t)(10*w);
      m.type = UNICAST_TYPE_WEIGHT;
      m.tag = tag;
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
/* This is the function that does the routing to the next mote in the DRW */
static void forward_message(){

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
      m.message = message_to_forward.message;
      m.type = UNICAST_TYPE_MESSAGE;
      m.tag = tag;
      message_queue.push(&message_queue, m);
      }
}
/*---------------------------------------------------------------------------*/



//BROADCAST RECEIVE
/* This function is called whenever a broadcast message is received. */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
  struct Message *msg;
  
  /* The packetbuf_dataptr() returns a pointer to the first data byte
     in the received packet. */
  msg = packetbuf_dataptr();
  
  switch (msg->type) {
      
    case BROADCAST_APPLY_TAG :
  
      printf("Asked to apply tag %d\n",msg->tag);
      if ((int)node_id != DATABASE)tag = msg->tag;
      leds_on(LEDS_RED);
      
    break;
      
    case BROADCAST_MAKE_CANDIDATE :
      
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
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/



// UNICAST RECEIVE
/* This function is called for every incoming unicast packet. */
static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
  struct Message *msg;

  /* Grab the pointer to the incoming data. */
  msg = packetbuf_dataptr();

  switch(msg->type){
  
    case UNICAST_TYPE_MESSAGE :

	    printf("Unicast ping received from %d.%d\n",
		   from->u8[0], from->u8[1]);
	    tag = msg->tag;
	    message_to_forward.message = msg->message;
            state = CURRENT_DRW_NODE;
	    
    break;
    
    case UNICAST_TYPE_WEIGHT :

      add_neighbor(msg->tag, msg->weight, from);
      
    break;

    }
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
/*---------------------------------------------------------------------------*/


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
          message_to_send = message_queue.pop(&message_queue);
        
          waitTime = getWaitTime(message_to_send.type);

          etimer_set(&et, waitTime * CLOCK_SECOND + CLOCK_SECOND * 0.1 * (int)node_id);
          PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

          //Unicast message
          if ((message_to_send.type == UNICAST_TYPE_MESSAGE) ||
              (message_to_send.type == UNICAST_TYPE_WEIGHT)){
		
                print_info(message_to_send.type);
		
		packetbuf_copyfrom(&message_to_send, sizeof(struct Message));
		unicast_send(&unicast, &unicast_target);
         
          }

          //Broadcast message
          else {
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
  leds_off(LEDS_ALL);

  static struct etimer et;
  
  if ((int)node_id == SENDER) state = NEW_MESSAGE;
  cc2420_set_txpower(POWER);

   while(1){
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
      if (state == NEW_MESSAGE){

        etimer_set(&et, CLOCK_SECOND * 2);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        printf("Starting the DRW\n");
	leds_on(LEDS_RED);
        leds_on(LEDS_BLUE);

        tag = 1;
        send_broadcast(BROADCAST_APPLY_TAG);
        
        etimer_set(&et, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
        send_broadcast(BROADCAST_MAKE_CANDIDATE);
            
        etimer_set(&et, CLOCK_SECOND * 30);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
        message_to_forward.message = "there is fire in room 22";
        forward_message();
        
        state = IDLE;
        leds_off(LEDS_BLUE);
      
     } else if (state == CURRENT_DRW_NODE){
        	    
       if ((int)node_id == DATABASE) {
	      
	 printf("Message has reached the database!\n");
	 printf("message is: %s\n", message_to_forward.message);
	 leds_on(LEDS_ALL);
       
       } else {
        
        visited++; 

        leds_on(LEDS_BLUE);
        list_init(neighbors_list);
        
        send_broadcast(BROADCAST_MAKE_CANDIDATE);
            
        etimer_set(&et, CLOCK_SECOND * 25 + random_rand() % (CLOCK_SECOND * 10));
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
           
        forward_message();
        send_broadcast(BROADCAST_APPLY_TAG);
            
        state = IDLE;
        leds_off(LEDS_BLUE);
       }
      
     } else if(state == CANDIDATE_NODE){
      
        leds_on(LEDS_GREEN);

        send_broadcast(BROADCAST_ASK_TAG);
      
        etimer_set(&et, CLOCK_SECOND * 8 + random_rand() % (CLOCK_SECOND * 10));
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
              
        send_weight();
              
        state = IDLE;
        leds_off(LEDS_GREEN);      
     } 
  }
  
  PROCESS_END();
}
