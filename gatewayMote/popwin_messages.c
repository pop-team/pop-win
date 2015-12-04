/*
 * Copyright (c) 2014, EIA-FR.
 * All rights reserved.
 * @author Laurent Winkler <lwinkler888@gmail.com>.
 * @author Marco Lourenço <kriegalex@gmail.com>.
 */

//-----------------------------------------------------------------
/*! \file */
/// This file contains all messages and format that are common to both the PC and the remote sensor

#include <stdio.h>
#include <string.h>
#include "popwin_messages.h"

// TYPES OF MESSAGES //

// -------------------------------------------------------------------------------- //
// Subscription message

/*!
 * \brief Print message to buffer
 *
 * \param x_msg         input SubscribeMessage msg
 * \param xp_buffer     output buffer
 * \param x_bufferSize  size_t size of the buffer for safety
 */
int bufferizeSubscribeMessage(const struct SubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x",
			MSG_SUBSCRIBE,
			x_msg->dataType,
			x_msg->measurementType,
			x_msg->id
	);
	return ret > 0 && ret < x_bufferSize;
}

/// \brief Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
int unbufferizeSubscribeMessage(struct SubscribeMessage* xp_msg, const char* x_buffer)
{
	memset(xp_msg, 0, sizeof(xp_msg));
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
// UnSubscription message

/// \brief Print message to buffer
///
/// @param x_msg     Incoming UnSubscribeMessage message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeUnSubscribeMessage(const struct UnSubscribeMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x",
			MSG_UNSUBSCRIBE,
			x_msg->dataType,
			x_msg->measurementType,
			x_msg->id
	);
	return ret > 0 && ret < x_bufferSize;
}

/// \brief Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
int unbufferizeUnSubscribeMessage(struct UnSubscribeMessage* xp_msg, const char* x_buffer)
{
	memset(xp_msg, 0, sizeof(xp_msg));
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
	if(ret == 4 && mtype == MSG_UNSUBSCRIBE)
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

/// \brief Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeNotifyMessage(const struct NotifyMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %02x %04x %02x %04x %04x %s %s",
			MSG_NOTIFY,
			x_msg->dataType,
			x_msg->measurementType,
			x_msg->id,
			x_msg->unit,
			(int)x_msg->dataSize,
			(int)x_msg->locationSize,
			x_msg->data,
			x_msg->location
	);

	return ret > 0 && ret < x_bufferSize;
}

/// \brief Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizeNotifyMessage(struct NotifyMessage* xp_msg, const char* x_buffer, size_t x_maxDataSize)
{
	memset(xp_msg, 0, sizeof(xp_msg));
	int mtype = -1;
	int id    = -1;
	int mt    = -1;
	int dt    = -1;
	int un    = -1;
	int dataSize = 0;
	int locationSize = 0;

	int ret = sscanf(x_buffer, "%02x %02x %02x %04x %02x %04x %04x",
			&mtype,
			&dt,
			&mt,
			&id,
			&un,
			&dataSize,
			&locationSize
	);

	// printf("data %s --> %02x %02x %02x %04x %02x %04d (data) (%d)\n", x_buffer, mtype, dt,mt,id,un,dataSize,/*data,*/ ret);
	if(ret == 7 && mtype == MSG_NOTIFY)
	{
		//printf("'%s', unbuffer %d, mtype %d\n",x_buffer, ret,mtype);
		xp_msg->measurementType = (enum MeasurementType) mt;
		xp_msg->dataType        = (enum DataType) dt;
		xp_msg->id              = id;
		xp_msg->unit            = (enum MeasurementUnit) un;
		xp_msg->dataSize        = (size_t) dataSize;
		xp_msg->locationSize    = (size_t) locationSize;
		if(dataSize + locationSize + 2 > x_maxDataSize)
		{
			printf("ERROR: Buffer has insufficient size %d > %d\n", dataSize + locationSize + 2, (int)x_maxDataSize);
			return 0;
		}
		int offsetData = 2+2+2+4+2+4+4+7*1;
		//printf("sizeof: %d",sizeof(xp_msg->data));
		int s = snprintf(xp_msg->data, dataSize+1, "%s", x_buffer + offsetData);
		int offsetLocation = offsetData+dataSize+1;
		int sizeLocation = snprintf(xp_msg->location, locationSize+1, "%s", x_buffer + offsetLocation);
		//printf("sizeLocation: %d",sizeLocation);
		return 1;
	}
	else
	{
		return 0;
	}
}

// -------------------------------------------------------------------------------- //
// Publication message

/// \brief Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizePublishMessage(const struct PublishMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %04x",
			MSG_PUBLISH,
			x_msg->publicationType,
			x_msg->id
	);
	return ret > 0 && ret < x_bufferSize;
}

/// \brief Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizePublishMessage(struct PublishMessage* xp_msg, const char* x_buffer, size_t x_maxDataSize)
{
	//printf("Unbuffering from buffer size: %d\n",strlen(x_buffer));
	memset(xp_msg, 0, sizeof(xp_msg));
	int mtype = -1;
	int id    = -1;
	int mt    = -1;

	int ret = sscanf(x_buffer, "%02x %02x %04x",
			&mtype,
			&mt,
			&id
	);
	if(ret == 3 && mtype == MSG_PUBLISH)
	{
		xp_msg->publicationType = (enum MeasurementType) mt;
		xp_msg->id              = id;
		return 1;
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //
// UnPublication message

/// \brief Print message to buffer
///
/// @param x_msg     Incoming message
/// @param xp_buffer Output buffer
/// @param x_bufferSize Size of the buffer for safety
int bufferizeUnPublishMessage(const struct UnPublishMessage* x_msg, char* xp_buffer, size_t x_bufferSize)
{
	// note: sizeof(size_t) = 2 for contiki but we use 4 for compatibility
	int ret = snprintf(xp_buffer, x_bufferSize, "%02x %02x %04x",
			MSG_UNPUBLISH,
			x_msg->publicationType,
			x_msg->id
	);
	return ret > 0 && ret < x_bufferSize;
}

/// \brief Read message from buffer
///
/// @param xp_msg         Output: message structure
/// @param x_buffer       Input:  message is read from here
/// @param x_maxDataSize: Size of the data buffer for safety
int unbufferizeUnPublishMessage(struct UnPublishMessage* xp_msg, const char* x_buffer, size_t x_maxDataSize)
{
	//printf("Unbuffering from buffer size: %d\n",strlen(x_buffer));
	memset(xp_msg, 0, sizeof(xp_msg));
	int mtype = -1;
	int id    = -1;
	int mt    = -1;

	int ret = sscanf(x_buffer, "%02x %02x %04x",
			&mtype,
			&mt,
			&id
	);
	if(ret == 3 && mtype == MSG_UNPUBLISH)
	{
		xp_msg->publicationType = (enum MeasurementType) mt;
		xp_msg->id              = id;
		return 1;
	}
	else return 0;
}

// -------------------------------------------------------------------------------- //

/// Retrieve the type of the message (first 2 bytes)
enum MessageType getMessageType(const char* x_msg)
{
	int type = 0;
	if(sscanf(x_msg, "%02d", &type) != 1)
		return MSG_UNKNOWN;
	else
		return (enum MessageType) type;

}

// -------------------------------------------------------------------------------- //

/// Translate a measurement type from text to enum
enum MeasurementType translateMeasurementType(const char* x_str)
{
	if(!strcmp(x_str, "logging"))      return MSR_LOG;
	if(!strcmp(x_str, "command"))      return MSR_COMMAND;
	if(!strcmp(x_str, "temperature"))  return MSR_TEMPERATURE;
	if(!strcmp(x_str, "vibration"))    return MSR_VIBRATION;
	if(!strcmp(x_str, "test"))         return MSR_TEST;
	if(!strcmp(x_str, "acceleration")) return MSR_ACCELERATION;
	if(!strcmp(x_str, "light"))        return MSR_LIGHT;
	if(!strcmp(x_str, "humidity"))     return MSR_HUMIDITY;
	if(!strcmp(x_str, "infrared"))     return MSR_INFRARED;
	if(!strcmp(x_str, "event"))        return MSR_EVENT;
	if(!strcmp(x_str, "led"))          return MSR_LED;
	if(!strcmp(x_str, "set_gw"))       return MSR_SET_GW;
	if(!strcmp(x_str, "error"))        return MSR_ERROR;

	printf("ERROR: Unknown measurement type %s\n", x_str);
	return MSR_LOG;
}

/// Translate a measurement type enum to text
const char* explainMeasurementType(enum MeasurementType x)
{
	switch(x)
	{
	case MSR_LOG:         return "logging";
	case MSR_COMMAND:     return "command";
	case MSR_TEMPERATURE: return "temperature";
	case MSR_VIBRATION:   return "vibration";
	case MSR_TEST:        return "test";
	case MSR_ACCELERATION:return "acceleration";
	case MSR_LIGHT:       return "light";
	case MSR_HUMIDITY:    return "humidity";
	case MSR_INFRARED:    return "infrared";
	case MSR_EVENT:       return "event";
	case MSR_LED:         return "led";
	case MSR_SET_GW:      return "set_gw";
	case MSR_ERROR:       return "error";
	}
	printf("ERROR: Unknown measurement type %d\n", (int)x);
	return "unknown";
}

/// Translate a measurement unit from text to enum
enum MeasurementUnit translateMeasurementUnit(const char* x_str)
{
	if(!strcmp(x_str, "no unit"))     return UNT_NONE;
	if(!strcmp(x_str, "none"))     	  return UNT_NONE;
	if(!strcmp(x_str, "celsius"))     return UNT_CELSIUS;
	if(!strcmp(x_str, "kelvin"))      return UNT_KELVIN;
	if(!strcmp(x_str, "seconds"))     return UNT_SECONDS;
	if(!strcmp(x_str, "meters"))      return UNT_METERS;
	if(!strcmp(x_str, "lux"))         return UNT_LUX;
	if(!strcmp(x_str, "percent"))     return UNT_PERCENT;

	printf("ERROR: Unknown measurement unit %s\n", x_str);
	return UNT_NONE;
}

/// Translate a measurement unit enum to text
const char* explainMeasurementUnit(enum MeasurementUnit x)
{
	switch(x)
	{
	case UNT_NONE:        return "no unit";
	case UNT_CELSIUS:     return "celsius";
	case UNT_KELVIN:      return "kelvin";
	case UNT_SECONDS:     return "seconds";
	case UNT_METERS:      return "meters";
	case UNT_LUX:         return "lux";
	case UNT_PERCENT:     return "percent";
	}
	printf("ERROR: Unknown measurement unit %d\n", (int)x);
	return "unknown";
}

/// Translate a data type from text to enum
enum DataType translateDataType(const char* x_str)
{
	if(!strcmp(x_str, "unknown"))     return TYPE_UNKNOWN;
	if(!strcmp(x_str, "float"))     return TYPE_DOUBLE;
	if(!strcmp(x_str, "double"))     return TYPE_DOUBLE;
	if(!strcmp(x_str, "int"))      return TYPE_INT;
	if(!strcmp(x_str, "string"))      return TYPE_STRING;

	printf("ERROR: Unknown data type %s\n", x_str);

	return TYPE_UNKNOWN;
}

/// Translate a data type enum to text
const char* explainDataType(enum DataType x)
{
	switch(x)
	{
	case TYPE_UNKNOWN    : return "unknown";
	case TYPE_DOUBLE     : return "float";
	case TYPE_INT        : return "int";
	case TYPE_STRING     : return "string";
	}
	printf("ERROR: Unknown data type %d\n", (int)x);
	return "unknown";
}

/// Translate a publication type from text to enum
enum PublicationType translatePublicationType(const char* x_str)
{
	if(!strcmp(x_str, "led"))         return PUB_LED;
	if(!strcmp(x_str, "command"))     return PUB_COMMAND;
	if(!strcmp(x_str, "switch"))      return PUB_SWITCH;
	if(!strcmp(x_str, "unknown"))	  return PUB_UNKNOWN;

	printf("ERROR: Unknown publication type %s\n", x_str);

	return PUB_UNKNOWN;
}

/// Translate a measurement unit enum to text
const char* explainPublicationType(enum PublicationType x)
{
	switch(x)
	{
	case PUB_COMMAND    : return "command";
	case PUB_LED        : return "led";
	case PUB_SWITCH		: return "switch";
	case PUB_UNKNOWN	: return "unknown";
	}
	printf("ERROR: Unknown publication type %d\n", (int)x);
	return "unknown";
}

