/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * Laurent Winkler <lwinkler888@gmail.com>. 
 */

//----------------------------------------------------------------- 
/// This file contains all messages and format that are common to both the PC and the remote sensor

#include <stdio.h>
#include <string.h>
#include "popwin_messages.h"

/// TYPES OF MESSAGES ///

// -------------------------------------------------------------------------------- //
// Subscription message

// Print message to buffer
int bufferizeSubscribeMessage(const struct SubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x\n",
		MSG_SUBSCRIBE,
		x_msg->dataType,
		x_msg->measurementType,
		x_msg->id
	);
	return ret > 0 && ret < x_bufferSize;
}

// Read message from buffer
int unbufferizeSubscribeMessage(struct SubscribeMessage* xp_msg, const char* x_buffer)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int ret = sscanf(x_buffer, "%02x %02x %02x %04x",
		&mtype,
		&dt,
		&mt,
		&id
	);
	if(ret == 4 && mtype == MSG_SUBSCRIBE)
	{
		xp_msg->id              = id;
		xp_msg->measurementType = (enum MeasurementType) mt;
		xp_msg->dataType        = (enum DataType)        dt;
		return 1;
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //
// Notification message

// Print message to buffer
int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, const char* x_data, char* xp_buffer, size_t x_bufferSize)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x %02x %04x %s\n",
		MSG_NOTIFY,
		x_msg->dataType,
		x_msg->measurementType,
		x_msg->id,
		x_msg->unit,
		(int)x_msg->dataSize,
		x_data
	);
	return ret > 0 && ret < x_bufferSize;
}

/// Read message from buffer
int unbufferizeNotifyMessage(struct NotifyMessage* xp_msg, char* xp_data, const char* x_buffer, size_t x_maxDataSize)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int un    = -1;
	int dataSize = 0;

	int ret = sscanf(x_buffer, "%02x %02x %02x %04x %02x %04x",
		&mtype,
		&dt,
		&mt,
		&id,
		&un,
		&dataSize
	);
	// printf("data %s --> %02x %02x %02x %04x %02x %04d (data) (%d)\n", x_buffer, mtype, dt,mt,id,un,dataSize,/*data,*/ ret);
	if(ret == 6 && mtype == MSG_NOTIFY)
	{
		xp_msg->measurementType = (enum MeasurementType) mt;
		xp_msg->dataType        = (enum DataType) dt;
		xp_msg->id              = id;
		xp_msg->unit            = (enum MeasurementUnit) un;
		xp_msg->dataSize        = (size_t) dataSize;
		if(dataSize + 1 > x_maxDataSize)
		{
			printf("ERROR: Buffer has insufficient size %d > %d\n", dataSize + 1, (int)x_maxDataSize);
			return 0;
		}
		int s = snprintf(xp_data, x_maxDataSize, "%s", x_buffer + 21 + 1);
		if(s == dataSize)
		{
			return 1;
		}
		else
		{
			printf("ERROR: Data has the wrong size %d!=%d\n", s, dataSize);
			return 0;
		}
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //
// Publication message

// Print message to buffer
int bufferizePublishMessage(const struct PublishMessage* x_msg, const char* x_data, char* xp_buffer, size_t x_bufferSize)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x %04x %s\n",
		MSG_PUBLISH,
		x_msg->dataType,
		x_msg->publicationType,
		x_msg->id,
		// msg->unit,
		(int)x_msg->dataSize,
		x_data
	);
	return ret > 0 && ret < x_bufferSize;
}

// Read message from buffer
int unbufferizePublishMessage(struct PublishMessage* xp_msg, char* xp_data, const char* x_buffer, size_t x_maxDataSize)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	// int un    = -1;
	int dataSize = 0;

	int ret = sscanf(x_buffer, "%02x %02x %02x %04x %04x",
		&mtype,
		&dt,
		&mt,
		&id,
		// &un,
		&dataSize
	);
	// printf("data %s --> %02x %02x %02x %04x %02x %04d %s\n", buffer, mtype, dt,mt,id,un,dataSize,data);
	if(ret == 5 && mtype == MSG_PUBLISH)
	{
		xp_msg->publicationType = (enum PublicationType) mtype;
		xp_msg->dataType        = (enum DataType) dt;
		xp_msg->publicationType = (enum PublicationType) mt;
		xp_msg->id              = id;
		// msg->unit            = (enum PublicationUnit) un;
		xp_msg->dataSize        = (size_t) dataSize;
		if(dataSize + 1 > x_maxDataSize)
		{
			printf("ERROR: Buffer has insufficient size %d > %d\n", dataSize + 1, (int)x_maxDataSize);
			return 0;
		}
		int s = snprintf(xp_data, x_maxDataSize, "%s", x_buffer + 18 + 1);
		if(s == dataSize)
		{
			return 1;
		}
		else
		{
			printf("ERROR: Data has the wrong size %d!=%d\n", s, dataSize);
			return 0;
		}
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //

// Retrieve the type of data (second position)
/*
enum DataType getDataType(const char* x_msg)
{
	int type     = 0;
	int dataType = 0;
	if(sscanf(x_msg, "%02d %02d", &type, &dataType) != 2)
		return TYPE_UNKNOWN;
	else
		return (enum DataType) dataType;
	
}*/

// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg)
{
	int type = 0;
	if(sscanf(x_msg, "%02d", &type) != 1)
		return MSG_UNKNOWN;
	else
		return (enum MessageType) type;
	
}

// -------------------------------------------------------------------------------- //

// Return measurement type from string
enum MeasurementType measurementType(const char* x_str)
{
	if(!strcmp(x_str, "logging"))     return MSR_LOG;
	if(!strcmp(x_str, "command"))     return MSR_COMMAND;
	if(!strcmp(x_str, "temperature")) return MSR_TEMPERATURE;
	if(!strcmp(x_str, "vibration"))   return MSR_VIBRATION;
	if(!strcmp(x_str, "test"))        return MSR_TEST;

	return MSR_LOG;
}

// String to explain measurement type
const char* explainMeasurementType(enum MeasurementType x)
{
	switch(x)
	{
		case MSR_LOG:         return "logging";
		case MSR_COMMAND:     return "command";
		case MSR_TEMPERATURE: return "temperature";
		case MSR_VIBRATION:   return "vibration";
		case MSR_TEST:        return "test";
		default:              return "unknown";
	}
}

// Return measurement units from string
enum MeasurementUnit measurementUnit(const char* x_str)
{
	if(!strcmp(x_str, "no unit"))     return UNT_NONE;
	if(!strcmp(x_str, "celsius"))     return UNT_CELSIUS;
	if(!strcmp(x_str, "kelvin"))      return UNT_KELVIN;
	if(!strcmp(x_str, "seconds"))     return UNT_SECONDS;
	if(!strcmp(x_str, "meters"))      return UNT_METERS;

	return UNT_NONE;
}

// String to explain measurement units
const char* explainMeasurementUnit(enum MeasurementUnit x)
{
	switch(x)
	{
		case UNT_NONE:        return "no unit";
		case UNT_CELSIUS:     return "celsius";
		case UNT_KELVIN:      return "kelvin";
		case UNT_SECONDS:     return "seconds";
		case UNT_METERS:      return "meters";
		default:              return "unknown";
	}
}

