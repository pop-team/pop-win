#include <iostream>
#include "myclass_3.ph"


  POPSensor_0 ::POPSensor_0( char path[ 20]){} 
  POPSensor_1 ::POPSensor_1( char path[ 20]){} 


  double POPSensor_0 ::gather() 
{ 
 double curdata=( double) indata; 
 return curdata; 
} 
  int POPSensor_1 ::gather() 
{ 
 int curdata=( int) indata; 
 return curdata; 
} 


  void POPSensor_0 ::broadcast( double outdata) 
{ 
} 
  void POPSensor_1 ::broadcast( int outdata) 
{ 
} 


@pack(POPSensor_0,POPSensor_1) ; 
