/*
 * Date: Mai 2016
 * @author Steve Nuoffer
 *
 * This file is part of the POPWin project.
 *
 * smart_environment_pro: This code can be uploaded on the remote sensor that acts as a gateway between
 *     a POP-C++ process (SensorProxy) running on a PC and the remote sensor.
 *     It handles both the serial communication via USB to the PC.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "/home/ubuntu/pop-win/gatewayMote/popwin_messages.c"

#include <WaspSensorGas_Pro.h>
#include <WaspOPC_N2.h>
#include <WaspFrame.h>

// Define socket for sensor: CO2
Gas CO2(SOCKET_F);

float concentration;	// Stores the concentration level in ppm
float temperature;	// Stores the temperature in ºC
float humidity;		// Stores the realitve humidity in %RH
float pressure;		// Stores the pressure in Pa

float pm1;              // Stores the pm 1 from particle matter
float pm25;             // Stores the pm 2.5 from particle matter
float pm10;             // Stores the pm 10 from particle matter

long decimal1;          // Get the integer part before comma
float fractional;       // Get fractional part
long decimal2;          // Get the integer part after comma


// Variables for the Particle Matter
char info_string[61];
int status;
int measure;

struct NotifyMessage msg;  // Stores the message


void setup()
{
  // Open the USB connection
  USB.ON();
  
  // Activate the Particle matter
  status = OPC_N2.ON();
  if (status == 1)
  {
      status = OPC_N2.getInfoString(info_string);
      if (status == 1)
      {
          //USB.println(F("Information string extracted:"));
          //USB.println(info_string);

      }
      else
      {
          //USB.println(F("Error reading the particle sensor"));
      }

      OPC_N2.OFF();
  }
  else
  {
      //USB.println(F("Error starting the particle sensor"));
  }

}

void loop()
{
    ///////////////////////////////////////////
    // 1. Power on CO2 sensor
    ///////////////////////////////////////////  

    // Power on the CO2 sensor. 
    // If the gases PRO board is off, turn it on automatically.
    CO2.ON();

    // CO2 gas sensor needs a warm up time at least 60 seconds	
    // To reduce the battery consumption, use deepSleep instead delay
    // After 1 minute, Waspmote wakes up thanks to the RTC Alarm
    PWR.deepSleep("00:00:02:00", RTC_OFFSET, RTC_ALM1_MODE1, ALL_ON);


    ///////////////////////////////////////////
    // 2. Read sensors
    ///////////////////////////////////////////  

    // 2.1 Read the CO2 sensor and compensate with the temperature internally
    concentration = CO2.getConc();

    // 2.2 Read enviromental variables
    temperature = CO2.getTemp(1);
    humidity = CO2.getHumidity();
    pressure = CO2.getPressure();
    
    // 2.3 Compute the temperature
    decimal1 = temperature;                // Get the integer part before comma
    fractional = temperature - decimal1;   // Get fractional part
    decimal2 = trunc(fractional * 10000);  // Get the integer part after comma
    
    // 2.4 Send the Temperature
    memset(&msg, 0, sizeof(msg));
    msg.measurementType = MSR_TEMPERATURE;
    msg.dataType        = TYPE_DOUBLE;
    msg.unit            = UNT_CELSIUS;
    msg.id              = 1;
    sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
    msg.dataSize        = strlen(msg.data);
    sprintf(msg.location,"%s","d2015");
    msg.locationSize	= strlen(msg.location);
    gwSendNotificationSerial(&msg);
    
    // 2.5 Compute the humidty
    decimal1 = humidity;                    // Get the integer part before comma
    fractional = humidity - decimal1;       // Get fractional part
    decimal2 = trunc(fractional * 10000);   // Get the integer part after comma
    
    // 2.6 Send the Humidity
    memset(&msg, 0, sizeof(msg));
    msg.measurementType = MSR_HUMIDITY;
    msg.dataType        = TYPE_DOUBLE;
    msg.unit            = UNT_PERCENT;
    msg.id              = 2;
    sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
    msg.dataSize        = strlen(msg.data);
    sprintf(msg.location,"%s","d2015");
    msg.locationSize	= strlen(msg.location);
    gwSendNotificationSerial(&msg);
    
    // 2.7 Compute the pressure
    decimal1 = pressure;                    // Get the integer part before comma
    fractional = pressure - decimal1;       // Get fractional part
    decimal2 = trunc(fractional * 10000);   // Get the integer part after comma
    
    // 2.8 Send the pressure
    memset(&msg, 0, sizeof(msg));
    msg.measurementType = MSR_PRESSURE;
    msg.dataType        = TYPE_DOUBLE;
    msg.unit            = UNT_PASCAL;
    msg.id              = 3;
    sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
    msg.dataSize        = strlen(msg.data);
    sprintf(msg.location,"%s","d2015");
    msg.locationSize	= strlen(msg.location);
    gwSendNotificationSerial(&msg);
    
    // 2.9 Compute the CO2
    decimal1 = concentration;                 // Get the integer part before comma
    fractional = concentration - decimal1;    // Get fractional part
    decimal2 = trunc(fractional * 10000);     // Get the integer part after comma
    
    // 2.10 Send the CO2 Concentration
    memset(&msg, 0, sizeof(msg));
    msg.measurementType = MSR_CO2;
    msg.dataType        = TYPE_DOUBLE;
    msg.unit            = UNT_PPM;
    msg.id              = 4;
    sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
    msg.dataSize        = strlen(msg.data);
    sprintf(msg.location,"%s","d2015");
    msg.locationSize	= strlen(msg.location);
    gwSendNotificationSerial(&msg);


    ///////////////////////////////////////////
    // 3. Power off CO2 sensor
    ///////////////////////////////////////////  

    // Power off the CO2 sensor. If there aren't more gas sensors powered,
    // turn off the board automatically
    CO2.OFF();
    
     ///////////////////////////////////////////
    // 4. Turn on the Particle Matter sensor
    /////////////////////////////////////////// 

    // Power on the OPC_N2 sensor. 
    // If the gases PRO board is off, turn it on automatically.
    status = OPC_N2.ON();
    if (status == 1)
    {
        //USB.println(F("Particle sensor started"));

    }
    else
    {
        //USB.println(F("Error starting the particle sensor"));
    }

    ///////////////////////////////////////////
    // 5. Read sensors
    ///////////////////////////////////////////  

    if (status == 1)
    {
        // 5.1 Power the fan and the laser and perform a measure of 5 seconds
        measure = OPC_N2.getPM(5000);
        if (measure == 1)
        {
            
            // 5.2 Get the sensor values
            pm1 = OPC_N2._PM1;
            pm25 = OPC_N2._PM2_5;
            pm10 = OPC_N2._PM10;
            
            // 5.3 Compute the PM1
            decimal1 = pm1;                          // Get the integer part before comma
            fractional = pm1 - decimal1;             // Get fractional part
            decimal2 = trunc(fractional * 10000);    // Get the integer part after comma
            
            // 5.4 Send the PM1
            memset(&msg, 0, sizeof(msg));
            msg.measurementType = MSR_PARTICLE1;
            msg.dataType        = TYPE_DOUBLE;
            msg.unit            = UNT_MICROGRAM;
            msg.id              = 5;
            sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
            msg.dataSize        = strlen(msg.data);
            sprintf(msg.location,"%s","d2015");
            msg.locationSize	= strlen(msg.location);
            gwSendNotificationSerial(&msg);
            
            // 5.5 Compute the PM2_5
            decimal1 = pm25;                        // Get the integer part before comma
            fractional = pm25 - decimal1;           // Get fractional part
            decimal2 = trunc(fractional * 10000);   // Get the integer part after comma
            
            
            // 5.6 Send the PM2_5
            memset(&msg, 0, sizeof(msg));
            msg.measurementType = MSR_PARTICLE2_5;
            msg.dataType        = TYPE_DOUBLE;
            msg.unit            = UNT_MICROGRAM;
            msg.id              = 6;
            sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
            msg.dataSize        = strlen(msg.data);
            sprintf(msg.location,"%s","d2015");
            msg.locationSize	= strlen(msg.location);
            gwSendNotificationSerial(&msg);
            
            // 5.7 Compute the PM10
            decimal1 = pm10;                        // Get the integer part before comma
            fractional = pm10 - decimal1;           // Get fractional part
            decimal2 = trunc(fractional * 10000);   // Get the integer part after comma
          
            // 5.8 Send the PM10
            memset(&msg, 0, sizeof(msg));
            msg.measurementType = MSR_PARTICLE10;
            msg.dataType        = TYPE_DOUBLE;
            msg.unit            = UNT_MICROGRAM;
            msg.id              = 7;
            sprintf(msg.data, "%ld.%04ld", decimal1, decimal2);
            msg.dataSize        = strlen(msg.data);
            sprintf(msg.location,"%s","d2015");
            msg.locationSize	= strlen(msg.location);
            gwSendNotificationSerial(&msg);

        }
        else
        {
            /*USB.print(F("Error performing the measure. Error code:"));
            USB.println(measure, DEC);*/
        }
    }


    ///////////////////////////////////////////
    // 6. Turn off the Particle Matter sensor
    /////////////////////////////////////////// 

    // Power off the OPC_N2 sensor. If there aren't other sensors powered, 
    // turn off the board automatically
    OPC_N2.OFF();
  
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




