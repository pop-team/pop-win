#include <stdio.h>
#include <unistd.h>


#include "sensor_combox.h"

int main(int argc, char **argv)
{
       sensorcom toto;
       toto.connection();
       char* JSONData=  "{\"function\":0}\n";
       toto.init_sensor(JSONData);
       JSONData=  "{\"function\":2}\n";
       toto.get_data(JSONData);
       JSONData=  "{\"function\":3, \"led\":1}\n";
       toto.set_data(JSONData);
       sleep(2);
       JSONData=  "{\"function\":3, \"led\":1}\n";
       toto.set_data(JSONData);
       JSONData=  "{\"function\":1}\n";
       toto.destroy_object(JSONData);
       return 1;
}
