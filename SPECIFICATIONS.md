
Specifications
--------------
### Resource description
The remote sensors that the programmer wants to access are specified in the **resource.json** file. It can contain the following fields:
- **gateway**: specifies how to access the gateway sensor
	- **url**: the machine on which the gateway sensor is connected
	- **connection**: the connection between the machine and the gateway sensor (for now only usb is supported)
- **nodes**: specifies the types of measurement
	- **genre**: temperature, humidity, light, ... as specified in [gatewayMote/popwin_messages.h]
	- **direction**: "IN" for sensor or "OUT" for actuators, "INOUT" for both
	- **type**: type of the measured data: float, double, string...
	- **location**: specify which place the data comes from, ex. room A
    - **unit**: the unit of the data we want, ex. celsius, lumen, none
	
Currently, in nodes list, *type*, *location* and *unit* are read and stored but are not used in the program.

	{
		"gateways":[
			{
				"url": "localhost",
				"connection": "usb"
			}
		],
		"wsns":{
			"nodes":[
				{
					"genre": "temperature",
				    "direction":   "IN",
				    "type": "float",
				    "location": "batiment A",
				    "unit": "celsius"
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
	}

Examples of resource files can be found [resources.json](here) and [multi_gw.json](here).


### Messaging
In the low-level part of the system (that uses the publish/subscribe messaging model) the following types of models are implemented: notification, publication and subscription. Please note that unsubscriptions are not implemented yet but their structure is identical to subscription messages.

#### Messages structures
Messages can be contained into fixed-size C structures that contain the full data

- **struct SubscribeMessage**
- **struct NotifyMessage**
- **struct PublishMessage**

The messages can contain the following fields:
- **measurementType**: An enum that specifies the type of measurement: temperature, vibration, humidity, ...
- **measurementUnit**: An enum that specifies the measurement unit: Celsius, seconds, meters, ...
- **publicationType**: An enum that specifies the type of publication: e.g. led
- **id**             : The id of the sender
- **dataType**       : An enum that specifies the type of the data: double, int or string
- **data**           : A buffer of character (of size BUFFERDATASIZE=64) that contains the data. For compatibility between systems, integers and floats are printed to this buffer and not stored as bit value.
- **dataSize**       : A buffer of character (of size BUFFERDATASIZE=64) that contains the location data. For compatibility between systems, location strings are printed to this buffer.
- **location**       : The size of the location data in characters
- **locationSize**   : The size of the location data in characters


#### Notification message

	struct NotifyMessage
	{
		enum MeasurementType measurementType;
		enum DataType        dataType;
		unsigned short       id;
		enum MeasurementUnit unit;
		size_t               locationSize;
		char                 location[BUFFERDATASIZE];
		size_t               dataSize;
		char                 data[BUFFERDATASIZE];
	};

#### Publication message

	struct PublishMessage
	{
		enum PublicationType publicationType;
		unsigned short       id;               // not mandatory, for convenience
	};
	
The UnPublishMessage has the same fields, only name and purpose change.


#### Subscription message

	struct SubscribeMessage
	{
		unsigned short       id;               // Id is not mandatory. Only for convenience
		enum MeasurementType measurementType;
		enum DataType        dataType;
	};
	
The UnSubscribeMessage has the same fields, only name and purpose change.

#### Print the messages to buffer
The message structures can be written to and read from a buffer of char. This is used when the message needs to be bufferized.

This can be done with the following functions (example for notification messages):

	/// Print message to buffer
	///
	/// @param x_msg        Incoming message structure
	/// @param xp_buffer    Output buffer
	/// @param x_bufferSize Size of the buffer for safety
	int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

	// Read message from buffer
	/// 
	/// @param xp_msg        Output: message structure
	/// @param x_buffer      Input:  message is read from here
	/// @param x_maxDataSize Size of the data buffer for safety
	int unbufferizeNotifyMessage(struct NotifyMessage* x_msg, const char* x_buffer, size_t x_dataSize);

The full specification of interface can be read from [gatewayMote/popwin_messages.h](here)

