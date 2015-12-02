/*
 * Copyright (c) 2015, HEIA-FR.
 * All rights reserved.
 * Laurent Winkler <lwinkler888@gmail.com>.
 * Marco Lourenço <marco.lourenco@hefr.ch>
 */

//----------------------------------------------------------------- 
// This file contains all messages and format that are common to both the PC and the remote sensor

#ifndef POPWIN_MESSAGES_H
#define POPWIN_MESSAGES_H


// The max size of a buffer after unbuffering (on the gateway only)
#define BUFFERSIZE     512

// The size of the data itself (e.g. in Notify messages)
#define BUFFERDATASIZE 64

// the sensor with ID below will be the gateway, this does not change the ID of the sensor
#define DEFAULT_GATEWAY_ID 3

/// Different types of messages
enum MessageType
{
	MSG_UNKNOWN     = 0, // only used for error handling
	MSG_SUBSCRIBE   = 1, // subscription message
	MSG_NOTIFY      = 2, // notification message
	MSG_PUBLISH     = 3, // publication message
	MSG_UNSUBSCRIBE = 4, // unsubscription message
	MSG_UNPUBLISH	= 5  // unpublication message
};

/// Different types of measurement
enum MeasurementType
{
	MSR_LOG         = 0,  // logging  can be seen as a notification. This is useful for debug but can be removed in the final version
	MSR_COMMAND     = 1,  // commands can be seen as a notification. This is useful for debug but can be removed in the final version 
	MSR_TEMPERATURE = 2,  // temperature measurement
	MSR_VIBRATION   = 3,  // vibration measurement
	MSR_TEST        = 4,  // test data
	MSR_ACCELERATION= 5,  // acceleration measurement
	MSR_LIGHT       = 6,  // acceleration measurement
	MSR_HUMIDITY    = 7,  // humidity measurement
	MSR_INFRARED    = 8,  // infrared measurement
	MSR_EVENT       = 9,  // a specific event is sent
	MSR_LED         = 10, // led status
	MSR_SET_GW		= 11, // set sensor as GW notification
	MSR_ERROR       = 12  // error
	// ... //
};

/// Translate a measurement type from text to enum
enum MeasurementType translateMeasurementType(const char* x_str);

/// Translate a measurement type enum to text
const char* explainMeasurementType(enum MeasurementType x);

/// Different types of measurement
enum MeasurementUnit
{
	UNT_NONE        = 0,
	UNT_CELSIUS     = 1,
	UNT_KELVIN      = 2,
	UNT_SECONDS     = 3,
	UNT_METERS      = 4,
	UNT_LUX         = 5,
	UNT_PERCENT     = 6
	// ... // 
};

/// Translate a measurement unit from text to enum
enum MeasurementUnit translateMeasurementUnit(const char* x_str);

/// Translate a measurement unit enum to text
const char* explainMeasurementUnit(enum MeasurementUnit x);

/// Different types of publication
enum PublicationType
{
	PUB_UNKNOWN     = 0,    // Unknown publication
	PUB_COMMAND     = 1,    // commands can be seen as publications
	PUB_LED         = 2,    // create channel for LEDS
	PUB_SWITCH      = 3   	// on/off a switch
	// ... //
};

/// Different types of actions related to LEDs
enum LedActions
{
	LED_GREEN_TOGGLE = 0,   // toggle (invert) the green led
	LED_GREEN_OFF    = 1,   // turn off the green led
	LED_GREEN_ON	 = 2,	// turn on the green led
	LED_BLUE_TOGGLE	 = 3,
	LED_BLUE_OFF	 = 4,
	LED_BLUE_ON	 	 = 5,
	LED_RED_TOGGLE	 = 6,
	LED_RED_OFF		 = 7,
	LED_RED_ON	 	 = 8,
	LED_ALL_TOGGLE	 = 9,
	LED_ALL_OFF		 = 10,
	LED_ALL_ON	 	 = 11
	// ... //
};

/// Translate a publication type from text to enum
enum PublicationType translatePublicationType(const char* x_str);

/// Translate a measurement unit enum to text
const char* explainPublicationType(enum PublicationType x);

/// Different types of measurement
enum DataType
{
	TYPE_UNKNOWN    = 0,
	TYPE_DOUBLE     = 1,
	TYPE_INT        = 2,
	TYPE_STRING     = 3
};

/// Translate a data type from text to enum
enum DataType translateDataType(const char* x_str);

/// Translate a data type enum to text
const char* explainDataType(enum DataType x);

//
// TYPES OF MESSAGES 
//

// -------------------------------------------------------------------------------- //
/// Subscription message
struct SubscribeMessage
{
	/// Id of emitter: Id is not mandatory. Only for convenience
	unsigned short       id;
	/// Type of measurement
	enum MeasurementType measurementType;
	/// Type of the data
	enum DataType        dataType;
};

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeSubscribeMessage(const struct SubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

/// Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
int unbufferizeSubscribeMessage(struct SubscribeMessage* xp_msg, const char* x_buffer);

// -------------------------------------------------------------------------------- //
/// UnSubscription message
struct UnSubscribeMessage
{
	/// Id of emitter: Id is not mandatory. Only for convenience
	unsigned short       id;
	/// Type of measurement
	enum MeasurementType measurementType;
	/// Type of the data
	enum DataType        dataType;
};

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeUnSubscribeMessage(const struct UnSubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

/// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
int unbufferizeUnSubscribeMessage(struct UnSubscribeMessage* xp_msg, const char* x_buffer);

// -------------------------------------------------------------------------------- //
/// A structure representing a notification message

struct NotifyMessage
{
	/// Genre of data, temperature, humidity, light, ...
	enum MeasurementType measurementType;
	/// Type of data, float, double, string, int, ...
	enum DataType        dataType;
	/// Id of emitter: Id is not mandatory. Only for convenience
	unsigned short       id;
	/// Unit of measurement, e.g. celsius, lumens, none, ...
	enum MeasurementUnit unit;
	/// Size of the location data, for storage in buffer
	size_t               locationSize;
	/// Location of measurement, e.g floor 1, room 2, ...
	char 				 location[BUFFERDATASIZE];
	/// Size of the data, for storage in buffer
	size_t               dataSize;
	/// Buffer containing the data on text format
	char                 data[BUFFERDATASIZE];
};

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

/// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizeNotifyMessage(struct NotifyMessage* x_msg, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //
/// Publication message
struct PublishMessage
{
	/// Type of the publication
	enum MeasurementType publicationType;
	/// Id of emitter
	unsigned short       id;               // not mandatory, for convenience
};

// Note: no unpublish message for the first version. The structure will be similar to publish messages

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizePublishMessage(const struct PublishMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

/// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizePublishMessage(struct PublishMessage* xp_msg, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //
/// UnPublication message
struct UnPublishMessage
{
	/// Type of the publication
	enum MeasurementType publicationType;
	/// Id of emitter
	unsigned short       id;               // not mandatory, for convenience
};

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeUnPublishMessage(const struct UnPublishMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

/// Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizeUnPublishMessage(struct UnPublishMessage* xp_msg, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //

/// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg);


#endif
