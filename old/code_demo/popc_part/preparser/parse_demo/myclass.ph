#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>   
#include <json/json.h>


template <typename dataType> parclass POPSensor
{  
 classuid(1001); 
 public: 
 POPSensor(char path[20])@{od.SensorDes(path);}; 
 sync seq dataType gather(); 
 async seq void broadcast(dataType outdata);
 private: 
 dataType indata; 
};

