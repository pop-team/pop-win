#include <iostream>
#include <unistd.h>

#include "myclass.ph"

using namespace std;

 int main( int argc , char** argv)
{ 
 printf("\nSTART program to access sensor!\n");
	
 char filepath1[20]="desfile1";
 POPSensor<double> tempSensor(filepath1);

 char filepath2[20]="desfile2";
 POPSensor<int> lightActuator(filepath2);

 int func = 0;
 while (func!=3)
 {
	printf("Input the function: 1) Get temperature data, 2) Set light, 3) Quit\n");
	scanf("%d",&func);
	switch(func)
	{
		case 1:
			double rtData;
 			rtData = tempSensor.gather();
 			printf("Get Temperature: %f \n" , rtData);
 			sleep(1);
			break;
		case 2:
			printf("Input the LED Index: \n");
 			int index;
 			scanf("%d",&index);
 			lightActuator.broadcast(index);
			sleep(1);
			break;
		case 3:
			break;
		default:
			break;	
	}
 }
 return 0;
} 

