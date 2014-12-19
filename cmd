POPCC="popcc --std=c++11"

$POPCC -parclass-nobroker -c POPSensor.ph POPSensor.o
$POPCC -parclass-nobroker -c SensorProxy.ph SensorProxy.cc
$POPCC -object POPSensor.ph POPSensor.cc -o POPSensor.obj SensorProxy.stub.o
$POPCC -object SensorProxy.ph SensorProxy.cc -o SensorProxy.obj

$POPCC -parclass-nobroker -c POPSensor.ph POPSensor.o
$POPCC -parclass-nobroker -c SensorProxy.ph SensorProxy.cc
$POPCC -o main main.cc POPSensor.ph POPSensor.o SensorProxy.ph SensorProxy.o

./POPSensor.obj -listlong > objects.map
./SensorProxy.obj -listlong >> objects.map
