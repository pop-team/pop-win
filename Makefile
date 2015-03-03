POPCC=popcc --std=c++11 -g

all: POPSensor.obj SensorProxy.obj main objects.map

clean:
	rm -f *.o *.obj main objects.map

%.stub.o: %.ph
	${POPCC} -parclass-nobroker -c $< -o $@

%.phstub.o: %.ph
	${POPCC} -c $< -o $@

%.obj.o: %.cc
	${POPCC} -c $< -o $@

POPSensor.obj: POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o
	${POPCC} -object -o POPSensor.obj POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o

SensorProxy.obj: SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o
	${POPCC} -object -o SensorProxy.obj SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o

main: main.obj.o POPSensor.stub.o SensorProxy.stub.o 
	${POPCC} -o main main.obj.o POPSensor.stub.o SensorProxy.stub.o

objects.map: POPSensor.obj SensorProxy.obj
	./POPSensor.obj -listlong > objects.map
	./SensorProxy.obj -listlong >> objects.map

run:
	popcrun objects.map ./main
