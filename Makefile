POPCC=popcc -cpp11 -g -no-async-allocation
INC=-IlineComm -I/usr/include/jsoncpp
LIBS=-ljsoncpp -lmysqlcppconn -lcurl

all: POPSensor.obj SensorProxy.obj main_sql objects.map

clean:
	rm -f *.o *.obj main main_demo main_sql objects.map

popwin_messages.o: lineComm/popwin_messages.c
	g++ -c lineComm/popwin_messages.c -o popwin_messages.o

%.stub.o: %.ph
	${POPCC} ${INC} -parclass-nobroker -c $< -o $@

%.phstub.o: %.ph
	${POPCC} ${INC} -c $< -o $@

%.obj.o: %.cc
	${POPCC} ${INC} -c $< -o $@

%.o: %.cc
	${POPCC} ${INC} -c $< -o $@


POPSensor.obj: POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o
	${POPCC} ${INC} -object -o POPSensor.obj POPSensor.obj.o POPSensor.phstub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o ${LIBS}

SensorProxy.obj: SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o POPSensorData.o
	${POPCC} ${INC} -object -o SensorProxy.obj SensorProxy.obj.o SensorProxy.phstub.o POPSensor.stub.o popwin_messages.o POPSensorData.o ${LIBS}

main_plot: main_plot.o POPSensorData.o popwin_messages.o
	${POPCC} ${INC} -o main_plot main_plot.o popwin_messages.o POPSensorData.o ${LIBS}
	
main_demo: main_demo.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o
	${POPCC} ${INC} -o main_demo main_demo.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o ${LIBS}
	
main_sql: main_sql.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o 
	${POPCC} ${INC} -o main_sql main_sql.o POPSensor.stub.o SensorProxy.stub.o popwin_messages.o POPSensorData.o ${LIBS}

objects.map: POPSensor.obj SensorProxy.obj
	./POPSensor.obj -listlong > objects.map
	./SensorProxy.obj -listlong >> objects.map
	
demo:
	popcrun objects.map ./main_demo localhost INOUT.json 160.98.61.190 IN.json
	
sql:
	popcrun objects.map ./main_sql localhost INOUT.json

documentation:
	ln -s POPSensor.ph POPSensor.h
	ln -s SensorProxy.ph SensorProxy.h
	doxygen
	rm POPSensor.h
	rm SensorProxy.h
