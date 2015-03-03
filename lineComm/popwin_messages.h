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

// Different types of messages
enum MessageType
{
	MSG_SUBSCRIBE = 0,
	MSG_NOTIFY    = 1
};

// Different types of measurement
enum MeasurementType
{
	MSR_TEMPERATURE = 0,
	MSR_VIBRATION   = 1
};

// Different types of measurement
enum MeasurementUnit
{
	UNT_CELSIUS     = 0,
	UNT_KELVIN      = 1,
	UNT_SECONDS     = 2,
	UNT_METERS      = 3
};

// Different types of measurement
enum DataType
{
	TYPE_DOUBLE     = 0,
	TYPE_INT        = 1,
	TYPE_STRING     = 2
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

// Notification message
struct NotifyMessage
{
	enum MeasurementType measurementType;
	enum DataType        dataType;
	unsigned short       id;
	// note: apparently the %f format of contiki requires a double not float
	double               data;
	enum MeasurementUnit unit;
};

// Print message to buffer
int bufferizeNotifyMessage(const struct NotifyMessage* msg, char* buffer, size_t size)
{
	int ret = snprintf(buffer, size, "%02x %02x %02x %02x %02x %f\n",
		MSG_NOTIFY,
		msg->measurementType,
		msg->dataType,
		msg->id,
		msg->unit,
		msg->data
	);
	return ret > 0 && ret < size;
}

// Read message from buffer
int unbufferizeNotifyMessage(struct NotifyMessage* msg, const char* buffer, size_t size)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int un    = -1;
	int ret = sscanf(buffer, "%02x %02x %02x %02x %02x %f\n",
		&mtype,
		&mt,
		&dt,
		&id,
		&un,
		&msg->data
	);
	if(ret == 6 && mtype == MSG_NOTIFY)
	{
		msg->measurementType = (enum MeasurementType) mt;
		msg->dataType        = (enum DataType) dt;
		msg->id              = id;
		msg->unit            = (enum MeasurementUnit) un;
	}
	else return 0;
}



#endif
