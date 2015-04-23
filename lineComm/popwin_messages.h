/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * Laurent Winkler <lwinkler888@gmail.com>. 
 */

//----------------------------------------------------------------- 
/// This file contains all messages and format that are common to both the PC and the remote sensor

#ifndef POPWIN_MESSAGES_H
#define POPWIN_MESSAGES_H

#define BUFFERSIZE 512

// Different types of messages
enum MessageType
{
	MSG_UNKNOWN   = 0, // only used for error handling
	MSG_SUBSCRIBE = 1, // subscription message
	MSG_NOTIFY    = 2, // notification message
	MSG_PUBLISH   = 3  // publication message
};

// Different types of measurement
enum MeasurementType
{
	MSR_LOG         = 0, // logging  can be seen as a notification. This is useful for debug but can be removed in the final version
	MSR_COMMAND     = 1, // commands can be seen as a notification. This is useful for debug but can be removed in the final version 
	MSR_TEMPERATURE = 2, // temperature measurement
	MSR_VIBRATION   = 3, // vibration measurement
	MSR_TEST        = 4, // test data
	MSR_ACCELERATION= 5, // acceleration measurement
	MSR_LIGHT       = 6  // acceleration measurement
	// ... //
};
enum MeasurementType measurementType(const char* x_str);
const char* explainMeasurementType(enum MeasurementType x);

// Different types of measurement
enum MeasurementUnit
{
	UNT_NONE        = 0,
	UNT_CELSIUS     = 1,
	UNT_KELVIN      = 2,
	UNT_SECONDS     = 3,
	UNT_METERS      = 4
	// ... // 
};
enum MeasurementUnit measurementUnit(const char* x_str);
const char* explainMeasurementUnit(enum MeasurementUnit x);

// Different types of publication
enum PublicationType
{
	PUB_COMMAND     = 0, // commands can be seen as publications
	PUB_LED         = 1  // blink a led
	// ... //
};

// Different types of measurement
enum DataType
{
	TYPE_UNKNOWN    = 0,
	TYPE_DOUBLE     = 1,
	TYPE_INT        = 2,
	TYPE_STRING     = 3
};


/// TYPES OF MESSAGES ///

// -------------------------------------------------------------------------------- //
// Subscription message

struct SubscribeMessage
{
	unsigned short       id;               // Id is not mandatory. Only for convenience
	enum MeasurementType measurementType;
	enum DataType        dataType;
};

// note: UnSubscribe message has the same structure as publish messages

// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeSubscribeMessage(const struct SubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
int unbufferizeSubscribeMessage(struct SubscribeMessage* xp_msg, const char* x_buffer);

// -------------------------------------------------------------------------------- //
// A structure representing a notification message

struct NotifyMessage
{
	enum MeasurementType measurementType;
	enum DataType        dataType;
	unsigned short       id;
	enum MeasurementUnit unit;
	size_t               dataSize;
	char*                data;
};

/// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param xp_data        Output: data contained in notification message gets copied here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizeNotifyMessage(struct NotifyMessage* x_msg, char* xp_data, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //
// Publication message

struct PublishMessage
{
	enum PublicationType publicationType;
	enum DataType        dataType;
	unsigned short       id;               // not mandatory, for convenience
	// enum MeasurementUnit unit;
	size_t               dataSize;
	char*                data;
};

// Note: no unpublish message for the first version. The structure will be similar to publish messages

// Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizePublishMessage(const struct PublishMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

// Read message from buffer
/// 
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param xp_data        Output: data contained in notification message gets copied here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizePublishMessage(struct PublishMessage* xp_msg, char* xp_data, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //

// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg);


#endif
