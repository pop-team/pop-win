POPCC=popcc --std=c++11 -g -IlineComm

all: POPSensor.obj SensorProxy.obj main objects.map

clean:
	rm -f *.o *.obj main objects.map

popwin_messages.o: lineComm/popwin_messages.c
	g++ -c lineComm/popwin_messages.c -o popwin_messages.o

%.stub.o: %.ph
	${POPCC} -parclass-nobroker -c $< -o $@

%.phstub.o: %.ph
	${POPCC} -c $< -o $@

%.obj.o: %.cc
	${POPCC} -c $< -o $@

POPSensor.obj: POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o
	${POPCC} -object -o POPSensor.obj POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o

SensorProxy.obj: SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o
	${POPCC} -object -o SensorProxy.obj SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o

main: main.obj.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o
	${POPCC} -o main main.obj.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o

objects.map: POPSensor.obj SensorProxy.obj
	./POPSensor.obj -listlong > objects.map
	./SensorProxy.obj -listlong >> objects.map

run:
	popcrun objects.map ./main
