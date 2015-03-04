/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * Laurent Winkler <lwinkler888@gmail.com>. 
 */

//----------------------------------------------------------------- 
/// This file contains all messages and format that are common to both the PC and the remote sensor

#ifndef POPWIN_MESSAGES_H
#define POPWIN_MESSAGES_H

#include <stdio.h>

#define BUFFERSIZE 512

// Different types of messages
enum MessageType
{
	MSG_UNKNOWN   = 0,
	MSG_SUBSCRIBE = 1,
	MSG_NOTIFY    = 2
};

// Different types of measurement
enum MeasurementType
{
	MSR_LOG         = 0, // logging: for debug
	MSR_TEMPERATURE = 1,
	MSR_VIBRATION   = 2
};

// Different types of measurement
enum MeasurementUnit
{
	UNT_CELSIUS     = 0,
	UNT_KELVIN      = 1,
	UNT_SECONDS     = 2,
	UNT_METERS      = 3
	// ... // 
};

// Different types of measurement
enum DataType
{
	TYPE_DOUBLE     = 0,
	TYPE_INT        = 1,
	TYPE_STRING     = 2,
	TYPE_UNKNOWN    = 3 // TODO: move first pos
};


/// TYPES OF MESSAGES ///

// Subscription message
struct SubscribeMessage
{
	unsigned short       id;
	enum MeasurementType measurementType;
	enum DataType        dataType;
};

// Print message to buffer
int bufferizeSubscribeMessage(const struct SubscribeMessage* msg, char* buffer, size_t size)
{
	int ret = snprintf(buffer, size, "%02x %02x %02x %02x\n",
		MSG_SUBSCRIBE,
		msg->id,
		msg->measurementType,
		msg->dataType
	);
	return ret > 0 && ret < size;
}

#ifndef POPWIN_MOTE // TODO: comment
// Read message from buffer
int unbufferizeSubscribeMessage(struct SubscribeMessage* msg, const char* buffer, size_t size)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int ret = sscanf(buffer, "%02x %02x %02x %02x\n",
		&mtype,
		&id,
		&mt,
		&dt
	);
	if(ret == 4 && mtype == MSG_SUBSCRIBE)
	{
		msg->id              = id;
		msg->measurementType = (enum MeasurementType) mt;
		msg->dataType        = (enum DataType)        dt;
		return 1;
	}
	else return 0;
}
#endif

// Notification message
struct NotifyMessage
{
	enum MeasurementType measurementType;
	enum DataType        dataType;
	unsigned short       id;
	// note: apparently the %f format of contiki requires a double not float
	char*                data;
	size_t               dataSize;
	enum MeasurementUnit unit;
};

// Print message to buffer
int bufferizeNotifyMessage(const struct NotifyMessage* msg, char* buffer, size_t size)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(buffer, size, "%02x %02x %02x %04x %02x %04x %s\n",
		MSG_NOTIFY,
		msg->dataType,
		msg->measurementType,
		msg->id,
		msg->unit,
		(int)msg->dataSize,
		msg->data
	);
	return ret > 0 && ret < size;
}

#ifndef POPWIN_MOTE // TODO: comment
// Read message from buffer
int unbufferizeNotifyMessage(struct NotifyMessage* msg, char* data, const char* buffer, size_t size)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int un    = -1;
	int dataSize = 0;

	int ret = sscanf(buffer, "%02x %02x %02x %04x %02x %04x %s\n",
		&mtype,
		&dt,
		&mt,
		&id,
		&un,
		&dataSize,
		data
	);
	// printf("data %s --> %02x %02x %02x %04x %02x %04d %s\n", buffer, mtype, dt,mt,id,un,dataSize,data);
	if(ret == 6 && mtype == MSG_NOTIFY)
	{
		msg->measurementType = (enum MeasurementType) mt;
		msg->dataType        = (enum DataType) dt;
		msg->id              = id;
		msg->unit            = (enum MeasurementUnit) un;
		msg->dataSize        = (size_t) dataSize;
		return 1;
	}
	else return 0;
}
#endif

// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg)
{
	int type = 0;
	if(sscanf(x_msg, "%02d", &type) != 1)
		return MSG_UNKNOWN;
	else
		return (enum MessageType) type;
	
}

// Retrieve the type of data (second position)
enum DataType getDataType(const char* x_msg)
{
	int type     = 0;
	int dataType = 0;
	if(sscanf(x_msg, "%02d %02d", &type, &dataType) != 2)
		return TYPE_UNKNOWN;
	else
		return (enum DataType) dataType;
	
}


#endif
