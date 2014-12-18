popcc -parclass-nobroker -c Gateway.ph Gateway.o
popcc -parclass-nobroker -c SensorProxy.ph SensorProxy.cc
popcc -object Gateway.ph Gateway.cc -o Gateway.obj SensorProxy.stub.o
popcc -object SensorProxy.ph SensorProxy.cc -o SensorProxy.obj

popcc -parclass-nobroker -c Gateway.ph Gateway.o
popcc -parclass-nobroker -c SensorProxy.ph SensorProxy.cc
popcc -o main main.cc Gateway.ph Gateway.o SensorProxy.ph SensorProxy.o

./Gateway.obj -listlong > objects.map
./SensorProxy.obj -listlong >> objects.map
