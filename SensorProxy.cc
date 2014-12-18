/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date Dec 2014
 * @brief Proxy class that handles communication with one sensor but resides on the same machine as the gateway
 *
 *
 */


#include "SensorProxy.ph"
#include "Gateway.ph"
#include <unistd.h>



SensorProxy::SensorProxy(POPString x_url)
{
	
}

SensorProxy::~SensorProxy()
{
	
}

/// Subscribe to a sensor
void SensorProxy::SubscribeMe(/*Gateway* xp_gateway*/)
{
}


@pack(SensorProxy);
