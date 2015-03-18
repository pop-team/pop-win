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
	MSR_VIBRATION   = 3  // vibration measurement
	// ... //
};
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
const char* explainMeasurementUnit(enum MeasurementUnit x);

// Different types of publication
enum PublicationType
{
	PUB_COMMAND     = 0, // commands can be seen as publications
	PUB_LED         = 1 
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
	unsigned short       id;
	enum MeasurementType measurementType;
	enum DataType        dataType;
};

// Print message to buffer
int bufferizeSubscribeMessage(const struct SubscribeMessage* msg, char* buffer, size_t size);

// Read message from buffer
int unbufferizeSubscribeMessage(struct SubscribeMessage* msg, const char* buffer, size_t size);

// -------------------------------------------------------------------------------- //
// Notification message

struct NotifyMessage
{
	enum MeasurementType measurementType;
	enum DataType        dataType;
	unsigned short       id;
	enum MeasurementUnit unit;
	size_t               dataSize;
	char*                data;
};

// Print message to buffer
int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, char* xp_buffer, size_t x_bufferSize);

// Read message from buffer
int unbufferizeNotifyMessage(struct NotifyMessage* x_msg, char* xp_data, const char* x_buffer, size_t x_dataSize);

// -------------------------------------------------------------------------------- //
// Publication message

struct PublishMessage
{
	enum PublicationType publicationType;
	enum DataType        dataType;
	unsigned short       id;
	// enum MeasurementUnit unit;
	size_t               dataSize;
	char*                data;
};

// Print message to buffer
int bufferizePublishMessage(const struct PublishMessage* msg, char* buffer, size_t size);

// Read message from buffer
int unbufferizePublishMessage(struct PublishMessage* msg, char* data, const char* buffer, size_t size);

// -------------------------------------------------------------------------------- //

// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg);


#endif
