

To compile and install the files on the mote:

	make TARGET=z1 clean
	make TARGET=z1 gatewayMote
	make TARGET=z1 gatewayMote.upload

To view what is printed by the mote if it is connected via USB:

	cd PATH_TO_CONTIKI/tools/sky
	./serialdump-linux -b115200 /dev/ttyUSB0
