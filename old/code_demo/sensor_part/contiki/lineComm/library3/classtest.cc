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

       char indata[100];
       JSONData=  "{\"function\":2}\n";
       toto.get_data(JSONData,indata);

       printf("Full Information: %s\n",indata);

       string tmp=indata;
       int bindex,eindex;
       bindex = tmp.find("ture\":\"",0);
       string temp = tmp.substr((bindex+8),20);
       eindex = temp.find("\"}}",0);
       temp = temp.substr(0,eindex);

       double temperature = atof(temp.c_str());
       
       printf("Get Information: %f\n",temperature);
       printf("End Location: %d\n",eindex);


       string setdata;
       setdata= "{\"function\":3, \"led\":";
       char num[5];
       scanf("%s", num);
       setdata = setdata+num+"}\n";
	     
      printf("Set data: %s\n",setdata.c_str());
      JSONData = const_cast<char*> (setdata.c_str());
	       toto.set_data(JSONData);
	       sleep(1);


       JSONData=  "{\"function\":1}\n";
       toto.destroy_object(JSONData);
       return 1;
}
