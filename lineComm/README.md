* **Author** Laurent Winkler
* **Date**   December 2014



commands to build and upload on remote node (connected via USB)

	FLAGS="CONTIKI=/home/lwinkler/prog/contiki"
	make $FLAGS TARGET=z1 savetarget
	make $FLAGS sensor_node
	make $FLAGS sensor_node.upload


To check if the application is working on remote node

	$CONTIKI/tools/sky/serialdump-linux -b115200 /dev/ttyUSB0
