POPCC=popcc --std=c++11

all: POPSensor.obj SensorProxy.obj main objects.map

clean:
	rm -f POPSensor.obj SensorProxy.obj main objects.map

POPSensor.obj: POPSensor.ph SensorProxy.ph SensorProxy.cc
	${POPCC} -parclass-nobroker -c SensorProxy.ph
	${POPCC} -object POPSensor.ph POPSensor.cc -o POPSensor.obj SensorProxy.stub.o

SensorProxy.obj: POPSensor.ph POPSensor.cc SensorProxy.ph SensorProxy.cc
	${POPCC} -parclass-nobroker -c POPSensor.ph
	${POPCC} -object SensorProxy.ph SensorProxy.cc -o SensorProxy.obj

main: POPSensor.ph SensorProxy.ph SensorProxy.cc main.cc
	${POPCC} -parclass-nobroker -c POPSensor.ph
	${POPCC} -parclass-nobroker -c SensorProxy.ph SensorProxy.cc
	${POPCC} -o main main.cc POPSensor.ph POPSensor.o SensorProxy.ph SensorProxy.o

objects.map: POPSensor.obj SensorProxy.obj
	./POPSensor.obj -listlong > objects.map
	./SensorProxy.obj -listlong >> objects.map

run:
	popcrun objects.map ./main
