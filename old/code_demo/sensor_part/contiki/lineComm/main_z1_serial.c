/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * Bourqui Valentin <bourqui.valentin@gmail.com>. 
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
#include <string.h>

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

#define BUFFERSIZE 200

//--------------------------------------------------------------- 


/****************************/
/***** INIT FUNCTIONS *******/
/****************************/

void init_sensor(struct jsonparse_state parser); // Function : 0
void destroy_object(struct jsonparse_state parser); // Function : 1
void get_data(struct jsonparse_state parser); // Function : 2
void set_data(struct jsonparse_state parser); // Function : 3

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

//--------------------------------------------------------------- 

typedef void (*FunctionCall)(jsonparse_state);
FunctionCall functions[] = {&init_sensor, &destroy_object,&get_data,&set_data};
int connected=0;
//--------------------------------------------------------------- 

/****************************/
/********* PROCESS **********/
/****************************/

PROCESS(init_com_process, "Incit communication process");
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
    // PRINTF("Temp = %s\n", data);
     if(ev == serial_line_event_message && data!=NULL) {  // If the event it's provoked by the timer expiration, then...
       struct jsonparse_state parser;
       jsonparse_setup(&parser, data, strlen(data));
       int type;
       int update = 0;
       while((type = jsonparse_next(&parser)) != 0 && update == 0) {
         if(type == JSON_TYPE_PAIR_NAME) {
	   if(jsonparse_strcmp_value(&parser, "function") == 0) {
             functions[json_copy_int(&parser)](parser);
             update=1;  
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
* Init sensor and search other node for the request
*/
void init_sensor(struct jsonparse_state parser){
  connected=1;
  leds_init();
  tmp102_init();
  PRINTF( "{\"Status\":\"OK\"}");
} 
 
/*
* Remove connexion with other nodes and stop to send data
*/
void destroy_object(struct jsonparse_state parser){
  connected=0;
  PRINTF( "{\"Status\":\"OK\"}");
}

/*
* Return data to the gateway
*/

 

void get_data(struct jsonparse_state parser){
  if(connected==1){
    

    int16_t tempint;
    uint16_t tempfrac;
    int16_t raw;
    uint16_t absraw;
    int16_t sign;
    char minus = ' ';
    sign = 1;
    raw = tmp102_read_temp_raw();
    absraw = raw;
    if(raw < 0) {		
      absraw = (raw ^ 0xFFFF) + 1;
      sign = -1;
    }
    tempint = ((absraw >> 8) * sign)-3;
    tempfrac = ((absraw >> 4) % 16) * 625;	
    minus = ((tempint == 0) & (sign == -1)) ? '-' : ' ';
    PRINTF( "{\"Status\":\"OK\", \"Infos\":{\"Temperature\":\"%c%d.%04d\"}}", minus, tempint, tempfrac);
  }
  else{
    PRINTF( "{\"Status\":\"NOK\", \"Infos\":\"Not connected\"}}");
  }
}

/*
* Set data like turn on/off leds
*/
void set_data(struct jsonparse_state parser){
 if(connected==1){
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
     PRINTF( "{\"Status\":\"OK\"}");
   }
   if(led==1){
     leds_toggle(LEDS_GREEN);
     PRINTF( "{\"Status\":\"OK\"}");
   }
   if(led==2){
     leds_toggle(LEDS_RED);
     PRINTF( "{\"Status\":\"OK\"}");
   }
   if(led==3){
     PRINTF( "{\"Status\":\"NOK\", \"Infos\":\"No leds for this ID\"}}");
   }
   
  }
  else{
    PRINTF( "{\"Status\":\"NOK\", \"Infos\":\"Not connected\"}}");
  }
}

/*---------------------------------------------------------------------------*/

