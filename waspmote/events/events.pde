/*
 * Date: Mai 2016
 * @author Steve Nuoffer
 *
 * This file is part of the POPWin project.
 *
 * events: This code can be uploaded on the remote sensor that acts as a gateway between
 *     a POP-C++ process (SensorProxy) running on a PC and the remote sensor.
 *     It handles both the serial communication via USB to the PC.
 *
 */
     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "/home/ubuntu/pop-win/gatewayMote/popwin_messages.c"

#include <WaspSensorEvent_v20.h>

float luminosityRaw;	// Stores the luminosity in volt
float luminosity;	// Stores the luminosity in ohm
float hallEffectRaw;	// Stores the hall effect in volt
float hallEffect;	// Stores the hall effect in ohm
float temperature;	// Stores the temperature in ºC
float humidity;		// Stores the humidity in %
float presence;		// Stores the presence in volt

long decimal1;          // Get the integer part before comma
float fractional;       // Get fractional part
long decimal2;          // Get the integer part after comma

struct NotifyMessage msg;  // Stores the message

void setup() {
  // 1. Initialization of the modules
  
  // Turn on the USB
  USB.ON();
  
  // Turn on the sensor board
  SensorEventv20.ON();
  
  // Firstly, wait for signal stabilization  
  while( digitalRead(DIGITAL5) == 1 )
  {    
    //USB.println(F("...wait for stabilization"));
    delay(1000);
  }
  
  // Turn on the RTC
  RTC.ON();
  

}


void loop() {
  ///////////////////////////////////////
  // 1. Read the sensor LDR voltage output
  ///////////////////////////////////////
  
  // 1.1. Read the sensor voltage output
  luminosityRaw = SensorEventv20.readValue(SENS_SOCKET1);
  
  decimal1 = luminosityRaw;                // Get the integer part before comma
  fractional = luminosityRaw - decimal1;   // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 1.2 Send Luminosity Raw (Volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_LUMINOSITY;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 8;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);

  // 1.3. Read the sensor output
  luminosity = SensorEventv20.readValue(SENS_SOCKET1, SENS_RESISTIVE);
  
  luminosity = luminosity * 1000;          // Convert kohm to ohm
  
  decimal1 = luminosity;                   // Get the integer part before comma
  fractional = luminosity - decimal1;      // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 1.4 Send Luminosity Ohm
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_LUMINOSITY;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_OHM;
  msg.id              = 9;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  
  ///////////////////////////////////////
  // 2. Read the sensor hall effect voltage output
  ///////////////////////////////////////

  // 2.1. Read the sensor voltage output
  hallEffectRaw = SensorEventv20.readValue(SENS_SOCKET2);
  
  decimal1 = hallEffectRaw;                // Get the integer part before comma
  fractional = hallEffectRaw - decimal1;   // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 2.2 Send Hall Effect Raw (Volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_HALLEFFECT;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 10;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);

  // 2.3. Read the sensor output
  hallEffect = SensorEventv20.readValue(SENS_SOCKET2, SENS_RESISTIVE);

  hallEffect = hallEffect * 1000;          // Convert kohm to ohm
  
  decimal1 = hallEffect;                   // Get the integer part before comma
  fractional = hallEffect - decimal1;      // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 2.4 Send Hall Effect Ohm
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_HALLEFFECT;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_OHM;
  msg.id              = 11;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 3. Read the sensor temperature output
  ///////////////////////////////////////
  
  // 3.1 Read the sensor output
  temperature = SensorEventv20.readValue(SENS_SOCKET5, SENS_TEMPERATURE);
    
  decimal1 = temperature;                  // Get the integer part before comma
  fractional = temperature - decimal1;     // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 3.2 Send temperature
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_TEMPERATURE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_CELSIUS;
  msg.id              = 12;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 4. Read the sensor humidity output
  ///////////////////////////////////////
  
  // 4.1. Read the sensor output
  humidity = SensorEventv20.readValue(SENS_SOCKET6, SENS_HUMIDITY);

  decimal1 = humidity;                      // Get the integer part before comma
  fractional = humidity - decimal1;         // Get fractional part
  decimal2 = trunc(fractional * 10000);     // Get the integer part after comma
  
  // 4.2 Send the humidity
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_HUMIDITY;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_PERCENT;
  msg.id              = 13;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 5. Read the sensor presence voltage output
  ///////////////////////////////////////
  
  // 5.1 Read the sensor voltage output
  presence = SensorEventv20.readValue(SENS_SOCKET7);
  
  decimal1 = presence;                    // Get the integer part before comma
  fractional = presence - decimal1;       // Get fractional part
  decimal2 = trunc(fractional * 10000);   // Get the integer part after comma
  
  // Send the Presence Volt
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_PRESENCE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 14;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  
  ///////////////////////////////////////
  // 6. Go to deep sleep mode  
  ///////////////////////////////////////
  //PWR.deepSleep("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);
  PWR.deepSleep("00:00:00:05",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);

}


/*!
 * \brief send a subscription message through the serial line
 * \param msg the SubscribeMessage to send
 */
void gwSendSubscriptionSerial(const struct SubscribeMessage* msg)
{
	char buf[BUFFERSIZE];
	if(bufferizeSubscribeMessage(msg, buf, sizeof(buf)) <= 0)
        {
        }
		//ERROR("Cannot write message to buffer (1)");

	// Send message via serial line on waspmote
	USB.printf("%s\n", buf);
}

/*!
 * \brief Send a notification message: Only for use on the remote gateway
 * \param msg the NotifyMessage to send
 */
void gwSendNotificationSerial(const struct NotifyMessage* msg)
{
	char buf[BUFFERSIZE];
	memset(&buf,0,BUFFERSIZE*sizeof(char));
	if(bufferizeNotifyMessage(msg, buf, sizeof(buf)) <= 0)
        {
        }
	  //ERROR("Cannot write message to buffer (2)");

	// Send message via serial line on waspmote
	USB.printf("%s\n", buf);
}

/*!
* \brief Send a publication message: Only for use on the remote gateway
* \param msg the PublishMessage to send
*/
void gwSendPublicationSerial(const struct PublishMessage* msg)
{
	char buf[BUFFERSIZE];
	memset(&buf,0,BUFFERSIZE*sizeof(char));
	if(bufferizePublishMessage(msg, buf, sizeof(buf)) <= 0)
        {
        }
		//ERROR("Cannot write message to buffer (3)");

	// Send message via serial line on waspmote
	USB.printf("%s\n", buf);
}
