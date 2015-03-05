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
	int ret = sscanf(buffer, "%02x %02x %02x %02x",
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

// -------------------------------------------------------------------------------- //
// Notification message

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

// Read message from buffer
int unbufferizeNotifyMessage(struct NotifyMessage* msg, char* data, const char* buffer, size_t size)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int un    = -1;
	int dataSize = 0;

	int ret = sscanf(buffer, "%02x %02x %02x %04x %02x %04x",
		&mtype,
		&dt,
		&mt,
		&id,
		&un,
		&dataSize
	);
	// printf("data %s --> %02x %02x %02x %04x %02x %04d %s\n", buffer, mtype, dt,mt,id,un,dataSize,data);
	if(ret == 6 && mtype == MSG_NOTIFY)
	{
		msg->measurementType = (enum MeasurementType) mt;
		msg->dataType        = (enum DataType) dt;
		msg->id              = id;
		msg->unit            = (enum MeasurementUnit) un;
		msg->dataSize        = (size_t) dataSize;
		strcpy(data, buffer + 21 + 1);
		return 1;
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //
// Publication message

// Print message to buffer
int bufferizePublishMessage(const struct PublishMessage* msg, char* buffer, size_t size)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(buffer, size, "%02x %02x %02x %04x %04x %s\n",
		MSG_PUBLISH,
		msg->dataType,
		msg->publicationType,
		msg->id,
		// msg->unit,
		(int)msg->dataSize,
		msg->data
	);
	return ret > 0 && ret < size;
}

// Read message from buffer
int unbufferizePublishMessage(struct PublishMessage* msg, char* data, const char* buffer, size_t size)
{
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	// int un    = -1;
	int dataSize = 0;

	int ret = sscanf(buffer, "%02x %02x %02x %04x %04x",
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
		msg->publicationType = (enum PublicationType) mt;
		msg->dataType        = (enum DataType) dt;
		msg->id              = id;
		// msg->unit            = (enum PublicationUnit) un;
		msg->dataSize        = (size_t) dataSize;
		strcpy(data, buffer + 18 + 1);
		return 1;
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
