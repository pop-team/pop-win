#include <iostream>
#include "myclass.ph"


template <typename dataType> POPSensor<dataType>::POPSensor(char path[20]){} 

template <typename dataType> dataType POPSensor<dataType>::gather() 
{
 dataType curdata = (dataType)indata;
 return curdata; 
} 

template <typename dataType> void POPSensor<dataType>::broadcast(dataType outdata) 
{
}

@pack(POPSensor) ; 
