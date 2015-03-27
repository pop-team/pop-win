/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "dev/leds.h"

#include "dev/serial-line.h"
#include <stdio.h> /* For printf() */
#include "dev/i2cmaster.h"
#include "dev/tmp102.h"

#include "simple-udp.h"

#define UDP_PORT 1234

#define SEND_INTERVAL		(10 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))


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


#define TMP102_READ_INTERVAL (CLOCK_SECOND*2)

//-----------------------------------------------------------------   
PROCESS(blink_timer_process, "blink with timer example");
PROCESS(hum_process, "Test temp process");
AUTOSTART_PROCESSES(&blink_timer_process,&hum_process);
//-----------------------------------------------------------------

PROCESS_THREAD(blink_timer_process, ev, data)
{   
  PROCESS_EXITHANDLER(goto exit);
  PROCESS_BEGIN();
  i2c_disable();
  /* Initialize stuff here. */ 
 
        printf("++++++++++++++++++++++++++++++\n");
	printf("+      EXAMPLE PROJECT       +\n");
	printf("++++++++++++++++++++++++++++++\n");
	printf("+     Blink/temp/hum app     +\n");
	printf("++++++++++++++++++++++++++++++\n\n");	
	
	leds_off(LEDS_ALL);
	printf("+       All leds are off     +\n\n");   
 
    while(1) {
	/* Do the rest of the stuff here. */
 
	static uint32_t ticks_r = 0;
        static uint32_t ticks_g = 0;
        static uint32_t ticks_b = 0;
	static uint32_t seconds = 5;
	static struct etimer et; // Define the timer
 
	///PROCESS_WAIT_EVENT();  // Waiting for a event, don't care which
 
	//if(ev == sensors_event) {  // If the event it's provoked by the user button, then...
        //   if(data == &button_sensor) {		
	//		etimer_set(&et, CLOCK_SECOND*seconds);  // Set the timer
	//		printf("+       Timer started        +\n");
        //   }
       // }

        PROCESS_YIELD();
 
	if(ev == serial_line_event_message && data!=NULL) {  // If the event it's provoked by the timer expiration, then...
             //   printf("received line: %s\n", (char *)data);
                if(((char *)data)[0]=='0'){
		  leds_toggle(LEDS_BLUE);
                  if (ticks_b % 2 == 0) {
			printf("+ LED BLUE .............. [ON]\n");
                  }
		  else { 
			printf("+ LED BLUE ............. [OFF]\n");
                  }
                  ticks_b++;
                }
		if(((char *)data)[0]=='1'){
		  leds_toggle(LEDS_GREEN);
                  if (ticks_g % 2 == 0) {
			printf("+ LED GREEN .............. [ON]\n");
                  }
		  else { 
			printf("+ LED GREEN ............. [OFF]\n");
                  }
                  ticks_g++;
                }
		if(((char *)data)[0]=='2'){
		  leds_toggle(LEDS_RED);
                  if (ticks_r % 2 == 0) {
			printf("+ LED RED .............. [ON]\n");
                  }
		  else { 
			printf("+ LED RED ............. [OFF]\n");
                  }
                  ticks_r++;
                }
		if(((char *)data)[0]=='3'){
		  leds_init();
                  printf("+ LED  .............. [INIT]\n");
                }
		
		//etimer_reset(&et);
		
                }	
	}		
	exit:
		leds_off(LEDS_ALL);
		PROCESS_END();
}
/*---------------------------------------------------------------------------*/
static struct etimer et;

PROCESS_THREAD(hum_process, ev, data)
{
 PROCESS_BEGIN();

  int16_t tempint;
  uint16_t tempfrac;
  int16_t raw;
  uint16_t absraw;
  int16_t sign;
  char minus = ' ';

  tmp102_init();

  while(1) {
    etimer_set(&et, TMP102_READ_INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    sign = 1;

    PRINTFDEBUG("Reading Temp...\n");
    raw = tmp102_read_temp_raw();
    absraw = raw;
    if(raw < 0) {		// Perform 2C's if sensor returned negative data
      absraw = (raw ^ 0xFFFF) + 1;
      sign = -1;
    }
    tempint = ((absraw >> 8) * sign)-3;
    tempfrac = ((absraw >> 4) % 16) * 625;	// Info in 1/10000 of degree
    minus = ((tempint == 0) & (sign == -1)) ? '-' : ' ';
    PRINTF("Temp = %c%d.%04d\n", minus, tempint, tempfrac);
  }
  PROCESS_END();
}


