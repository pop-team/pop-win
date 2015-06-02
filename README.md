POPWin
======
- **Author:** Laurent Winkler
- **Date:**   Decembre 2014

This document contains a general description of the POPWin project, specifications and examples of programmation. For a more technical documentation (installation, troubleshooting, ...) the developer documentation can be found here [here](INSTALL.md)

Contacts
--------
- EIFR
	- Pierre Kuonen <pierre.kuonen@hefr.ch>
	- Laurent Winkler <laurent.winkler@hefr.ch>
	- Lu Yao <yao.lu@edu.hefr.ch>
- UNI GE
	- Pierre Leone <Pierre.Leone@unige.ch>
	- Cristina Munoz Illescas <Cristina.Munoz@unige.ch>

System overview
---------------
POPWin provides a set of tools (based on POP-C++) to build communication between a remote machine and a network of sensors. 

The complete system is devided into two parts. The **high-level** part handles the communication between different parallel objects that can run on different machines using the POP-C++ framework. The **low-level** part contains the sensor that communicate using radio waves. Inbetween the two parts a GatewaySensor must be used. It will forward the messages between the SensorProxy object (running on a PC) and the rest of the sensor network. Communication between the SensorProxy and GatewaySensor is done via USB.


	                                                                        ************
	main -------------- POPSensor -- SensorProxy1 === GatewaySensor ***   *Sensor network*
	                            \                                           ************  
	                            |                                           
	                            |
	                            |                                                         
	                            \                                           ************  
	                             --- SensorProxy2 === GatewaySensor ***   *Sensor network*
	                                                                        ************
	                                     ...

	--- TCPIP communication
	=== USB communication
	*** Radio communication

As explained in the documentation of the POPWin project the communication uses different messaging models:
- High level: the POP model is used
- Low level: the publish/subscribe model is used

Examples of programmation
-------------------------

### main_example.cpp
This file contains the a simple example on how to uses the different classes of the POPWin project. The main code of this examples is:

	// Create a POPSensor object on the local machine with resource description
	POPSensor popSensor("localhost", "resource.json");

	// Start listening to the remote sensors
	popSensor.StartListening();

	// wait ...

	// Stop listening
	popSensor.StopListening();

	// Print the gathered data
	POPSensorData data(popSensor.Gather());
	data.Print();

	// Clear data
	popSensor.Clear();

As shown here the POPSensor object can be used in a very simple and efficient manner. For the programmer this class can be used transparently.

To compile and run this example use:

	cd pop-win
	make
	popcrun objects.map ./main_example resources.json


### main.cpp
This file contains a more complete use of the POPWin objects. It also instantiates a POPSensor objects and lets the user send commands via the keyboard. This example can be used to test the system.

To compile and run this example use:

	cd pop-win
	make
	popcrun objects.map ./main resources.json


Specifications
--------------
### Resource description
The resources (or sensors) that the programmer wants to access are specified in the **resource.json** file. It can contain the following fields:
- **gateway**: specifies how to access the gateway sensor
	- **url**: the machine on which the gateway sensor is connected
	- **connection**: the connection between the machine and the gateway sensor (for now only usb is supported)
- **nodes**: specifies the types of measurement
	- **measurement type**: temperature, humidity, light, ... as specified in [gatewayMote/popwin_messages.h]
	- **direction**: "IN" for sensor or "OUT" for actuators


TODO: Discuss with PK -> array of gateways

	{
		"gateways":[
			{
				"url": "localhost",
				"connection": "usb"
			}
		],
		"nodes":[
			{
				"measurementType": "temperature",
				"direction":   "IN"
			},
			{
				"measurementType": "humidity",
				"direction":   "IN"
			},
			{
				"measurementType": "light",
				"direction":   "IN"
			}
		]
	}


### Messaging
In the low-level part of the system (that uses the publish/subscribe messaging model) the following types of models are implemented: notification, publication and subscription. Please note that unsubscriptions are not implemented yet but their structure is identical to subscription messages.

The messages can contain the following fields:
- **MeasurementType**: An enum that specifies the type of measurement: temperature, vibration, humidity, ...
- **MeasurementUnit**: An enum that specifies the measurement unit: Celsius, seconds, meters, ...
- **PublicationType**: An enum that specifies the type of publication: e.g. led
- **ID**             : The id of the sender
- **DataType**       : An enum that specifies the type of the data: double, int or string
- **data**           : A buffer of character (of size BUFFERDATASIZE=64) that contains the data. For compatibility between systems integers and floats are printed to this buffer and not stored as bit value.
- **DataSize**       : The size of the above data in characters

#### Notification message

	struct NotifyMessage
	{
		enum MeasurementType measurementType;
		enum DataType        dataType;
		unsigned short       id;
		enum MeasurementUnit unit;
		size_t               dataSize;
		char                 data[BUFFERDATASIZE];
	};

#### Publication message

	struct PublishMessage
	{
		enum PublicationType publicationType;
		enum DataType        dataType;
		unsigned short       id;               // not mandatory, for convenience
		size_t               dataSize;
		char                 data[BUFFERDATASIZE];
	};


#### Subscription message

	struct SubscribeMessage
	{
		unsigned short       id;               // Id is not mandatory. Only for convenience
		enum MeasurementType measurementType;
		enum DataType        dataType;
	};


