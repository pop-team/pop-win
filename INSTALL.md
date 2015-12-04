Technical documentation
=======================
- **Authors:** Laurent Winkler, Marco Lourenço
- **Date:**    November 2015

This documentation describes the more advanced aspects of the development of POPWin and is meant for developers only. 

Contacts
--------
- EIFR
	- Pierre Kuonen <pierre.kuonen@hefr.ch>
	- Laurent Winkler <laurent.winkler@hefr.ch>
	- Lu Yao <yao.lu@edu.hefr.ch>
	- Marco Lourenço <marco.lourenco@hefr.ch>
- UNI GE
	- Pierre Leone <Pierre.Leone@unige.ch>
	- Cristina Munoz Illescas <Cristina.Munoz@unige.ch>


Installation of Contiki
-----------------------
Contiki OS is the operation system that runs on the Z1 sensor. There are different ways of installing:

- With a VM ware virtual machine: http://www.contiki-os.org/start.html
- Or by installing a cross-compilation toolchain for Ubuntu https://github.com/contiki-os/contiki/wiki/Setup-contiki-toolchain-in-ubuntu-13.04

In this doc we have chosen the second option.

### Z1 and XM100 sensors

In this demo we use a Zolertia Z1 sensor (also called a mote)

	http://www.contiki-os.org/hardware.html

or AdvanticSys sensors: 

	http://www.advanticsys.com/shop/asxm1000-p-24.html

### Set the access rights on the USB port

To be able to write to the USB port you will need to have access rights to the /dev/ttyUSB0. 
This can be done with a chmod 777 command or better by adding your user (in this case lwinkler) to the group that has access to the port:

	ll /dev/ttyUSB0 

should give you:

	crw-rw---- 1 root dialout 188, 0 Dez  5 17:02 /dev/ttyUSB0

You can give permission to your user (in this case lwinkler) to read-write to the USB port:

	sudo adduser lwinkler dialout

Then logout of your Ubuntu session and login again. You should now have read-write access on the port.


### Installing the toolchain

As explained on:

	https://github.com/contiki-os/contiki/wiki/Setup-contiki-toolchain-in-ubuntu-13.04

Dependencies to install (on Ubuntu):

    sudo apt-get install build-essential binutils-msp430 gcc-msp430 msp430-libc binutils-avr gcc-avr gdb-avr avr-libc avrdude openjdk-7-jdk openjdk-7-jre ant libncurses5-dev doxygen git

Get the sources and compiling the example natively

	git clone git://github.com/contiki-os/contiki.git contiki
	cd contiki
	git checkout release-3-0
	
Contiki 3.0 has a bug with the serialdump utility, so we have to patch it manually :

	cd tools/sky/
	rm serialdump-linux
	
Open the serialdump.c file and at line 167, replace

	#ifdef O_SYNC
	
with

	#ifndef O_SYNC
	
Now you can recompile the file

	make
	cd ../../examples/hello-world

Run the commands below to compile the example for the sensor and upload it. Use savetarget if you want to remember the target platform to avoid typing it all the time. You may have to reboot your computer if you have a "can't open port" error after the first connection of the Z1 sensor.

	make TARGET=z1 hello-world
	make TARGET=z1 savetarget
	make hello-world.upload

To contact the sensor and visualize the hello-world message type:

	make login

The application is executed on a reset of the sensor. If you click on the small reset button on the sensor you should see the message appear.

**Important** : For the AdvanticSys XM1000 mote, you also need to install specific files as explained in section (**Installation of contiki files for xm1000**)


Demo
----
The demo setup shows the communication between a POP-C++ application running on PC and a network of remote sensor.

Agents
- POP-C++ main application running on PC
- POP-C++ SensorProxy (or proxy node) running on PC
- Gateway mote running on Zolertia (or xm1000) sensor
- Other motes that form the network of sensors


### Installing the demo
This section indicates how to prepare the demonstration. The PC is running Ubuntu 14.04.

#### Compilation of POP-C++
Downloading the 3.0 release version of POP-C++.

In case of problem with the installation, please refer to the README file contained in POP-C++.

    sudo apt-get install git-core g++ flex bison cxxtest zlib1g-dev cmake
    cd ~
    git clone https://github.com/pop-team/pop-cpp.git
    cd pop-cpp
    git checkout -b release3.0 tags/3.0
    cmake .
    make
    sudo make install

Type 'y' and enter and do not forget to add POPC_LOCATION to your path as specified at the end of the installation. Then restart another shell.

#### Installation of Contiki
Contiki must be installed as mentionned above. The version to install is 3.0.

```
	cd ~
	git clone git://github.com/contiki-os/contiki.git
	cd contiki
	git checkout release-3-0
```

Do not forget to install the mentionned patch for **cpu/msp430/Makefile.msp430** if you choose to run the 2.6 version.

#### Download and compile the sources of POPWin

When prompted for MySQL admin password during mysql-server-5.6 install, choose one and remember it !

```
	sudo apt-get install libjsoncpp-dev libmysqlcppconn-dev mysql-server-5.6
	cd ~
	git clone https://github.com/pop-team/pop-win.git
	cd pop-win
	make clean && make
```

#### Install the MySQL workbench and setup MySQL tables

- Go to http://dev.mysql.com/downloads/workbench/ and download workbench
- Or use direct link for workbench 6.3.5 for Ubuntu 14.04 x64 : http://bit.ly/1MZ72tL
- Then execute (in the right directory) :

```	
	sudo dpkg -i mysql-workbench-community-6.3.5-1ubu1404-amd64.deb
	sudo apt-get -f install
```

- Launch workbench

```
	mysql-workbench &
```

- Connect to DB with PWD created when mysql-server-5.6 was installed
- Create schema with query :

```
	CREATE SCHEMA `popwin_schema` ;
```

- Reconnect to DB/refresh if needed for schema to appear, make it default (right-click on name) then execute :

```
	CREATE TABLE `POPSensorData` (`idPOPSensorData` int(11) NOT NULL AUTO_INCREMENT,
	`type` varchar(45) DEFAULT NULL, `genre` varchar(45) DEFAULT NULL,
  	`location` varchar(45) DEFAULT NULL, `sensorID` int(11) NOT NULL,
  	`value` varchar(45) NOT NULL, `unit` varchar(45) DEFAULT NULL,
  	PRIMARY KEY (`idPOPSensorData`)) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

#### Install the application on the gateway mote
- Connect the mote to your PC. In our setup we use a Zolertia z1 mote
- Go to the **gatewayMote/** directory inside popwin
- Modify the Makefile to point to your contiki directory

Makefile:

```
	CONTIKI=$(HOME)/contiki/
	include $(CONTIKI)/Makefile.include
```

Make sure XM1000_FLASH is commented in **gatewayMote.c** if you want to flash for Z1 :

```
	//#define XM1000_FLASH
```

- Compile and upload to Z1 sensor (use TARGET=xm1000 for xm1000 sensors).

```
	make TARGET=z1 clean
	make TARGET=z1 gatewayMote
	make TARGET=z1 gatewayMote.upload
```

#### Start the demo on PC
In your root POPWin directory type:


```
	make sql
```

This should give you the following output:

```
	=======================================================================================
 	POPWin demo: Launching the demo                                                       
	=======================================================================================
	Creating POPSensor 1
	Creating POPSensor with id=0
	Creating sensor proxy with id=1000 on port /dev/ttyUSB0
	Created 1 sensor proxy objects
	Gateway 1000 subscribes to led type:int direction: IN
	Gateway 1000 can publish led commands, direction: OUT
	Gateway 1000 subscribes to temperature type:float direction: IN
	Instantiating driver
	Connecting to db

	Press key to continue

```

Wait for some values to be inserted in the DB and then press a keyboard key to show values from DB. Feel free to change main_sql.cc to test other SQL queries (~line 53).


Installation of POP-C++ with support for templates
--------------------------------------------------
Support for templates in POP-C++ is only experimental. Not included in this release.

Installation of contiki files for xm1000
----------------------------------------
To compile the code for xm1000 mote, you will need to add files to your Contiki directory. This is because Contiki doesn't provide them for xm1000, only for Z1 platform.

In directory popwin/gatewayMote: decompress the files in archive popwin_patch_contiki3.tar.gz. Then copy the content of tools/ and platform/ directories in your Contiki directory.

Please note that these files are based on the v2.7 ones and modified to work for Contiki 3.0. Getting the ones from the website (see below) won't work for Contiki 3.0.

You can find the latest version of the code on page (november 2015, currently for 2.7 only):

	http://www.advanticsys.com/shop/asxm1000-p-24.html
	http://www.advanticsys.com/wiki/index.php?title=XM1000

Examples of programmation
-------------------------

Troubleshooting
---------------

### Problem
When connecting to sensor with

	make login
	
or

	./serialdump-linux -b115200 /dev/ttyUSB0
	
you get the following error :

	connecting to /dev/ttyUSB0 (115200)
	open: Invalid argument
	
### Solution
You didn't patch and recompile the file in contiki/tools/sky/serialdump.c (see instructions in **Installing the toolchain**).

### Problem
The sensors outputs garbage characters with serialdump or nothing at all
### Solution
- You are running an old version of Contiki (2.6 or 2.7), our install was updated to use Contiki 3.0.
- The reset (using the sensor button) is not handled properly. You need to unplug the device of USB port and plug it back in after at least 5 seconds.
- Another solution to this problem (with the Zolertia z1) is to hit the reset button repeatidly until an output can be seen

- And also check that the baudrate of your port is set to 115200. You can get the current value with:

	stty -F /dev/ttyUSB0

The SensorProxy class of POPWin will try to reset the value to 115200 but this does apparently not work all the time.

### Problem
Cannot compile application with Contiki 2.6:

Message:
	
	... relocation truncated to fit: R_MSP430_16_BYTE against `no symbol'

### Solution: Patch for contiki

	diff --git a/cpu/msp430/Makefile.msp430 b/cpu/msp430/Makefile.msp430
	index 8e9ffd2..651bd3f 100644
	--- a/cpu/msp430/Makefile.msp430
	+++ b/cpu/msp430/Makefile.msp430
	@@ -129,7 +129,7 @@ ifndef CC_MCU
	endif

	ifndef CFLAGSNO
	-CFLAGSNO = -Wall -mmcu=$(CC_MCU) -g $(CFLAGSWERROR)
	+CFLAGSNO = -Wall -mmcu=$(CC_MCU) $(CFLAGSWERROR)
	endif
	CFLAGS  += -Os -fno-strict-aliasing
	LDFLAGS += -mmcu=$(CC_MCU) -Wl,-Map=contiki-$(TARGET).map


### Problem
Image generated and uploaded on sensor is corrupt

**Caution 1: ** As stated in the doc, if the upload phase is almost instantaneous, this probably means that the upload failed. In this case check if your sensor is connected and if you have access to the /dev/ttyUSB0 port.

**Caution 2: ** With the latest versions of contiki (November 2014) it seems that sometimes the upload fails randomly. If you get an output similar to this you may have to re-upload:
If the application is already in communication, the port will be busy and the upload will fail with a very criptic message: 

	Traceback (most recent call last):
	File "../tools/z1/z1-bsl-nopic", line 1940, in <module>
	main(0);
	File "../tools/z1/z1-bsl-nopic", line 1855, in main
	for f in toinit: f()
	File "../tools/z1/z1-bsl-nopic", line 1120, in actionMassErase
	0xa506)             #Required setting for mass erase!
	File "../tools/z1/z1-bsl-nopic", line 762, in bslTxRx
	self.bslSync(wait)                          #synchronize BSL
	File "../tools/z1/z1-bsl-nopic", line 700, in bslSync
	c = self.serialport.read(1)             #read answer
	File "/usr/lib/python2.7/dist-packages/serial/serialposix.py", line 460, in read
	raise SerialException('device reports readiness to read but returned no data (device disconnected?)')
	serial.serialutil.SerialException: device reports readiness to read but returned no data (device disconnected?)
	+++++ Programming /dev/ttyUSB0
	MSP430 Bootstrap Loader Version: 1.39-goodfet-8
	Invoking BSL...
	Transmit default password ...
	MSP430 Bootstrap Loader Version: 1.39-goodfet-8
	Invoking BSL...
	Transmit default password ...
	Traceback (most recent call last):
	File "../tools/z1/z1-bsl-nopic", line 1940, in <module>
	main(0);
	File "../tools/z1/z1-bsl-nopic", line 1865, in main
	speed=speed,
	File "../tools/z1/z1-bsl-nopic", line 1151, in actionStartBSL
	self.txPasswd(self.passwd)                  #transmit password
	File "../tools/z1/z1-bsl-nopic", line 1079, in txPasswd
	wait=wait)           #if wait is 1, try to sync forever
	File "../tools/z1/z1-bsl-nopic", line 762, in bslTxRx
	self.bslSync(wait)                          #synchronize BSL
	File "../tools/z1/z1-bsl-nopic", line 700, in bslSync
	c = self.serialport.read(1)             #read answer
	File "/usr/lib/python2.7/dist-packages/serial/serialposix.py", line 460, in read
	raise SerialException('device reports readiness to read but returned no data (device disconnected?)')
	serial.serialutil.SerialException: device reports readiness to read but returned no data (device disconnected?)
	+++++ Resetting /dev/ttyUSB0
	MSP430 Bootstrap Loader Version: 1.39-goodfet-8
	Use -h for help
	Use --fromweb to upgrade a GoodFET.


### Problem
With contiki 2.7+ while compiling for xm1000

	.../dev/sht11.c: In function ‘sht11_init’:
	.../dev/sht11.c:218:4: warning: #warning SHT11: DISABLING I2C BUS [-Wcpp]
	.../dev/sht11.c:221:5: error: ‘SHT11_PxREN’ undeclared (first use in this function)
	.../dev/sht11.c:221:5: note: each undeclared identifier is reported only once for each function it appears in
	...file.include:214: recipe for target 'obj_xm1000/sht11.o' failed

### Solution
Add this line in contiki/platform/xm1000/dev/sht11-arch.h (http://sourceforge.net/p/contiki/mailman/message/31712247/)

	#define SHT11_PxREN  P1REN

### Problem: Incoming communication problem with xm1000
A big problem encountered with the XM1000 sensors happens with the communication PC->sensor. It seems that the XM1000 receives a message from the PC but is unable to read it. This is annoying and I was not able to fix it after contacting both AdvanticSys and Contiki. AdvanticSys was useless for support and on the Contiki mailing list someone replied and suggest a workaround (using the Zolertia Z1 as gateway).

I ended using the workaround.

The thread on Contiki mailing list:

	http://ehc.ac/p/contiki/mailman/message/34129191/
	
The e-mail sent to AdvanticSys:

	Dear support team

	I am having trouble with the XM1000 remote sensor and contiki 2.7. It seems that the incoming communication (PC->sensor) are not handled by the sensor. 

	It seems that the following program gets stuck at PROCESS_WAIT_EVENT_UNTIL and does not handle any events (ie nothing gets printed)

	The code is compiled with the toolchain and contiki 2.7. (The same code works with Zolertia z1 sensors)

	Can you help me ? 

	Best regards
	Laurent Winkler



	#include "contiki.h"
	#include "dev/leds.h"
	#include <stdio.h>
	#include "dev/serial-line.h"

	/*---------------------------------------------------------------------------*/
	/* We first declare our processes. */
	PROCESS(gateway_communication_process, "Communication to/from the gateway");


	/* The AUTOSTART_PROCESSES() definition specifices what processes to
	   start when this module is loaded. We put our processes there. */
	AUTOSTART_PROCESSES(&gateway_communication_process);

	PROCESS_THREAD(gateway_communication_process, ev, data)
	{
		PROCESS_EXITHANDLER(goto exit);
		PROCESS_BEGIN();

		/* Initialize stuff here. */
		printf("++++++++++++++++++++++++++++++\n");
		printf("+   INIT/START SERIAL COM    +\n");
		printf("++++++++++++++++++++++++++++++\n");

		while(1) {
			PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message && data != NULL);
			printf("RECEIVED DATA\n");
			if(data == NULL)
			{
				printf("Received empty data\n");
				continue;
			}
		}               
	exit:
		printf("Exiting process\n");
		leds_off(LEDS_ALL);
		PROCESS_END();
	}

I was not able to fix this problem so I used a workaround instead: I used the Zolertia Z1 as a gateway sensor. It works pretty much ok.
	

Limitations
-----------
- **Broadcast of messages**: Broadcast of message is working only in HEIA_FR code (not DRW) and mainly for test to light some LEDs.
- **Full implementation of routing** : So far the routing of message is implemented twice: the first implementation is a full implementation made by UNIGE (slow), the second is a faster implementation using the libraries of Contiki (fast). The second implementation was used for tests.

```
	// note: we can choose here which version of the code we want to run:
	\#if DRW_FLASH
	// Processes to run with routing algo of UNIGE
	AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &communication_process, &drw, &sensor_events); // Processes to run with algo of UNIGE
	\#else
	// Processes to use the routing of messages given by the multihop example
	AUTOSTART_PROCESSES(&gateway_communication_process, &button_pressed, &multihop_announce, &multihop_sense);
	\#endif
```

- **SQL queries**: SQL queries of type SELECT or TRUNCATE work as expected. Types like UPDATE will work but if you update the value for a LED, the LED won't change color or go on/off. Instead you simply changed a value in the DB.
