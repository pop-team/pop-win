#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <json/json.h>


 parclass POPSensor_0 
{ 
 classuid(1001); 
 public: 
 POPSensor_0( char path[ 20])@{od.SensorDes( path);}; 
 sync seq double gather(); 
 async seq void broadcast( double outdata); 
 private: 
 double indata; 
};
 parclass POPSensor_1 
{ 
 classuid(1002); 
 public: 
 POPSensor_1( char path[ 20])@{od.SensorDes( path);}; 
 sync seq int gather(); 
 async seq void broadcast( int outdata); 
 private: 
 int indata; 
};


