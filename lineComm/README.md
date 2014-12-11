* **Author** Laurent Winkler
* **Date**   December 2014



commands to build:

	FLAGS="CONTIKI=/home/lwinkler/prog/contiki"
	make $FLAGS TARGET=z1 savetarget
	make $FLAGS sensor_node
	make $FLAGS sensor_node.upload
