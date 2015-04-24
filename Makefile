POPCC=popcc -cpp11 -g
INC=-IlineComm -I/usr/include/jsoncpp
LIBS=-ljsoncpp

all: POPSensor.obj SensorProxy.obj main objects.map

clean:
	rm -f *.o *.obj main objects.map

popwin_messages.o: lineComm/popwin_messages.c
	g++ -c lineComm/popwin_messages.c -o popwin_messages.o

%.stub.o: %.ph
	${POPCC} ${INC} -parclass-nobroker -c $< -o $@

%.phstub.o: %.ph
	${POPCC} ${INC} -c $< -o $@

%.obj.o: %.cc
	${POPCC} ${INC} -c $< -o $@

POPSensor.obj: POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o
	${POPCC} ${INC} -object -o POPSensor.obj POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o ${LIBS}

SensorProxy.obj: SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o
	${POPCC} ${INC} -object -o SensorProxy.obj SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o ${LIBS}

main: main.obj.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o
	${POPCC} ${INC} -o main main.obj.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o ${LIBS}

objects.map: POPSensor.obj SensorProxy.obj
	./POPSensor.obj -listlong > objects.map
	./SensorProxy.obj -listlong >> objects.map

run:
	popcrun objects.map ./main resources.json
