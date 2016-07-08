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
     
#include <WaspSensorGas_v20.h>

#define GAIN_CO2  7      //GAIN of the CO2 sensor stage

#define GAIN_VOC  1      // GAIN of the VOC sensor stage
#define RESISTOR_VOC 3   // LOAD RESISTOR of the VOC sensor stage

#define GAIN_NH3  1      // GAIN of the NH3 sensor stage
#define RESISTOR_NH3 10  // LOAD RESISTOR of the NH3 sensor stage

#define GAIN_CH4  1      //GAIN of the CH4 sensor stage
#define RESISTOR_CH4 20  //LOAD RESISTOR of the CH4 sensor stage


float temperature;	// Stores the temperature in ºC
float co2;    	        // Stores the co2 in volt
float vocRaw;	        // Stores the voc in volt
float voc;	        // Stores the voc in ohm
float pressure;	        // Stores the pressure in Pa
float ammoniaRaw;       // Stores the ammonia in volt
float ammonia;		// Stores the ammonia in ohm
float methaneRaw;       // Stores the methane in volt
float methane;		// Stores the methane in ohm

long decimal1;          // Get the integer part before comma
float fractional;       // Get fractional part
long decimal2;          // Get the integer part after comma

struct NotifyMessage msg;  // Stores the message


void setup() {
  //Turn on the USB
  USB.ON();
  
  // Turn on the SensorGase
  SensorGasv20.ON();
  
  // Configure the CO2 sensor socket
  SensorGasv20.configureSensor(SENS_CO2, GAIN_CO2);

  // Turn on the CO2 sensor and wait for stabilization
  SensorGasv20.setSensorMode(SENS_ON, SENS_CO2);
  
  // Configure the VOC sensor socket
  SensorGasv20.configureSensor(SENS_SOCKET3B, GAIN_VOC, RESISTOR_VOC);
  
  // Turn on the VOC sensor on socket 3B and wait for stabilization
  SensorGasv20.setSensorMode(SENS_ON, SENS_SOCKET3B);
  
  // Configure the NH3 sensor on socket 3
  SensorGasv20.configureSensor(SENS_SOCKET3NH3, GAIN_NH3, RESISTOR_NH3);
  
  // Configure the 2A sensor socket
  SensorGasv20.configureSensor(SENS_SOCKET2A, GAIN_CH4, RESISTOR_CH4);

  // Turn on the sensor on socket 2A and wait for stabilization
  SensorGasv20.setSensorMode(SENS_ON, SENS_SOCKET2A);
  
  // Turn on the RTC
  RTC.ON();

}


void loop() {
  ///////////////////////////////////////
  // 1. Read the sensor temperature output
  ///////////////////////////////////////
  
  // 1.1. Read the sensor output
  temperature = SensorGasv20.readValue(SENS_TEMPERATURE);
  
  decimal1 = temperature;                  // Get the integer part before comma
  fractional = temperature - decimal1;     // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 1.2 Send temperature
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_TEMPERATURE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_CELSIUS;
  msg.id              = 15;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 2. Read the sensor CO2 output
  ///////////////////////////////////////
  
  // 2.1. Read the sensor output
  co2 = SensorGasv20.readValue(SENS_CO2);
  
  decimal1 = co2;                          // Get the integer part before comma
  fractional = co2 - decimal1;             // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 2.2 Send co2 (volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_CO2;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 16;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 3. Read the sensor VOC output
  ///////////////////////////////////////
  
  // 3.1 Read the voc sensor raw output 
  vocRaw = SensorGasv20.readValue(SENS_SOCKET3B);
  
  decimal1 = vocRaw;                       // Get the integer part before comma
  fractional = vocRaw - decimal1;          // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma

  // 3.2 Send VOC Raw (Volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_VOC;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 17;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  // 3.3 Conversion from voltage into kiloohms
  voc = SensorGasv20.calculateResistance(SENS_SOCKET3B, vocRaw, GAIN_VOC, RESISTOR_VOC);
  
  voc = voc * 1000;                        // Convert kohm to ohm
  
  decimal1 = voc;                          // Get the integer part before comma
  fractional = voc - decimal1;             // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 3.4 Send voc Ohm
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_VOC;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_OHM;
  msg.id              = 18;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  
  ///////////////////////////////////////
  // 4. Read the sensor pressure output
  ///////////////////////////////////////
  
  // 4.1 Turn on the atmospheric pressure sensor and wait for stabilization
  SensorGasv20.setSensorMode(SENS_ON, SENS_PRESSURE);
  delay(20); 
 
  // 4.2 Read the sensor
  pressure = SensorGasv20.readValue(SENS_PRESSURE);
  
  // 4.3 Turn off the atmospheric pressure sensor
  SensorGasv20.setSensorMode(SENS_OFF, SENS_PRESSURE);
    
  pressure = pressure * 1000;              // Convert kPa to Pa
  
  decimal1 = pressure;                     // Get the integer part before comma
  fractional = pressure - decimal1;        // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
  
  // 4.4 Send pressure Pa
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_PRESSURE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_PASCAL;
  msg.id              = 19;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);

  ///////////////////////////////////////
  // 5. Read the sensor ammonia output
  ///////////////////////////////////////

  // 5.1 Read the sensor voltage output
  ammoniaRaw = SensorGasv20.readValue(SENS_SOCKET3NH3);
  
  decimal1 = ammoniaRaw;                   // Get the integer part before comma
  fractional = ammoniaRaw - decimal1;      // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 5.2 Send ammonia Raw (Volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_AMMONIA;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 20;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  // 5.3 Conversion from voltage into kiloohms  
  ammonia = SensorGasv20.calculateResistance(SENS_SOCKET3NH3, ammoniaRaw, GAIN_NH3, RESISTOR_NH3);
  
  ammonia = ammonia * 1000;                // Convert kohm to ohm
  
  decimal1 = ammonia;                      // Get the integer part before comma
  fractional = ammonia - decimal1;         // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 5.4 Send ammonia Ohm
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_AMMONIA;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_OHM;
  msg.id              = 21;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  ///////////////////////////////////////
  // 6. Read the sensor methane output
  ///////////////////////////////////////
  
  // 6.1 Read the sensor 
  methaneRaw = SensorGasv20.readValue(SENS_SOCKET2A);
  
  decimal1 = methaneRaw;                   // Get the integer part before comma
  fractional = methaneRaw - decimal1;      // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
      
  // 6.2 Send ammonia Raw (Volt)
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_METHANE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_VOLT;
  msg.id              = 22;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  // 6.3 Conversion from voltage into kiloohms
  methane = SensorGasv20.calculateResistance(SENS_SOCKET2A, methaneRaw, GAIN_CH4, RESISTOR_CH4);
  
  methane = methane * 1000;                // Convert kohm to ohm
  
  decimal1 = methane;                      // Get the integer part before comma
  fractional = methane - decimal1;         // Get fractional part
  decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
    
  // 6.4 Send methane Ohm
  memset(&msg, 0, sizeof(msg));
  msg.measurementType = MSR_METHANE;
  msg.dataType        = TYPE_DOUBLE;
  msg.unit            = UNT_OHM;
  msg.id              = 23;
  sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
  msg.dataSize        = strlen(msg.data);
  sprintf(msg.location,"%s","d2015");
  msg.locationSize	= strlen(msg.location);
  gwSendNotificationSerial(&msg);
  
  //Turn off the sensor board
  //SensorGasv20.OFF();
  
  ///////////////////////////////////////
  // 7. Go to deep sleep mode  
  ///////////////////////////////////////
  PWR.deepSleep("00:00:01:00",RTC_OFFSET,RTC_ALM1_MODE1,SOCKET0_OFF);

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
