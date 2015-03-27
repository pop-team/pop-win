#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>     /* atof */

#include <json/json.h>
#include <iostream>

#include "sensor_combox.h"

using namespace std;



int main(int argc, char **argv)
{

       sensorcom toto;
       toto.connection();
       char* JSONData=  "{\"function\":0}\n";
       toto.init_sensor(JSONData);

       string setdata;
       setdata= "{\"function\":3, \"led\":";
       int out = 1;
       char num[5];
       sprintf(num,"%d",out);
       setdata = setdata+num+"}\n";
	     
       printf("Set data: %s\n",setdata.c_str());
       JSONData = const_cast<char*> (setdata.c_str());
       toto.set_data(JSONData);
       sleep(1);

       JSONData=  "{\"function\":1}\n";
       toto.destroy_object(JSONData);
       return 1;
}
