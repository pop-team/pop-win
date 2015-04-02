Technical documentation
=======================

- **Author:** Laurent Winkler
- **Date:**   Decembre 2014

Contacts
--------
- EIFR
	- Pierre Kuonen <pierre.kuonen@hefr.ch>
	- Lu Yao <yao.lu@edu.hefr.ch>
	- Laurent Winkler <laurent.winkler@hefr.ch>
- UNI GE
	- Pierre Leone <Pierre.Leone@unige.ch>
	- Cristina Munoz Illescas <Cristina.Munoz@unige.ch>

Installation of Contiki
-----------------------
Contiki OS is the operation system that runs on the Z1 sensor. There is different way for installation:

- With a VM ware virtual machine: http://www.contiki-os.org/start.html
- Or by installing a cross-compilation toolchain for Ubuntu https://github.com/contiki-os/contiki/wiki/Setup-contiki-toolchain-in-ubuntu-13.04

In this doc we have chosen the second option.

### Z1 sensor

In this demo we use a Zolertia Z1 sensor (also called a mote)

	http://www.contiki-os.org/hardware.html

### Set the access rights on the USB port

To be able to write to the USB port you will need to have access rights to the /dev/ttyUSB0. 
This can be done with a chmod 777 command or better by adding your user (in this case lwinkler) to the group that has access to the port:

	ll /dev/ttyUSB0 

gives you:

	crw-rw---- 1 root dialout 188, 0 Dez  5 17:02 /dev/ttyUSB0

	sudo adduser lwinkler dialout

Then logout of your Ubuntu session and login again. You should now have read-write access on the port.


### Installing the toolchain

As explained on:

	https://github.com/contiki-os/contiki/wiki/Setup-contiki-toolchain-in-ubuntu-13.04

Dependencies to install (on Ubuntu):

	sudo apt-get install build-essential binutils-msp430 gcc-msp430 msp430-libc binutils-avr gcc-avr gdb-avr avr-libc avrdude openjdk-7-jdk openjdk-7-jre ant libncurses5-dev doxygen git

Get the sources and compiling the example natively

	git clone git://github.com/contiki-os/contiki.git contiki
	cd contiki/examples/hello-world
	make TARGET=native hello-world

This should give you the following output:

	Contiki-2.6-1935-gb6b5c6e started with IPV6, RPL
	Rime started with address 1.2.3.4.5.6.7.8
	MAC nullmac RDC nullrdc NETWORK sicslowpan
	Tentative link-local IPv6 address fe80:0000:0000:0000:0302:0304:0506:0708
	Hello, world

To compile the example for the sensor and upload it

	make TARGET=z1 hello-world
	make TARGET=z1 savetarget
	make hello-world.upload

To contact the sensor and visualize the hello-world message type:

	make login

The application is executed on a reset of the sensor. If you click on the small reset button on the sensor you should see the message appear.


Demo
----
The demo setup shows the communication between a POP-C++ application running on PC and a network of remote sensor.

Agents
- POP-C++ main application running on PC
- POP-C++ SensorProxy (or proxy node) running on PC
- Gateway mote running on Zolertia (or xm1000) sensor
- Other motes that form the network of sensors


### Installing the demo
This section indicates how to prepare the demonstration. The PC is running Ubuntu 14.10.

#### Compilation of POP-C++
Downloading the develop version of POP-C++. As soon as the release is made the 3.0 version can also be used.

In case of problem with the installation, please refer to the README file contained in POP-C++.

```
	sudo apt-get install git-core g++ flex bison cxxtest
	git clone git@github.com:pop-team/pop-cpp.git -b develop
	cd pop-cpp
	cmake .
	make
	sudo make install

```
Type 'y' and enter and do not forget to add POPC_LOCATION to your path as specified at the end of the installation. Then restart another shell.

#### Installation of Contiki
Contiki must be installed as mentionned above. The version to install is 2.6.

```
	git clone git://github.com/contiki-os/contiki.git
	git checkout -b 2.6 2.6-rc0
```

Do not forget to install the mentionned patch for **cpu/msp430/Makefile.msp430**

#### Download and compile the sources of POPWin

```
	git clone git@github.com:pop-team/pop-win.git
	cd pop-win
	make clean && make
```

#### Install the application on the gateway mote
- Connect the mote to your PC. In our setup we use a Zolertia z1 mote
- Go to the **gatewayMote/** directory inside popwin
- Modify the Makefile to point to your contiki directory

Makefile:

```
	CONTIKI=/home/lwinkler/prog/contiki/
	include $(CONTIKI)/Makefile.include
```
- Compile and upload

```
	make TARGET=z1 clean
	make TARGET=z1 gatewayMote
	make TARGET=z1 gatewayMote.upload
```

#### Start the demo on PC
In your POPWin directory type:


```
	make run
```

This should give you the following output:

```
	=======================================================================================
	POPWin demo: type a command to interact with your sensors                             
	=======================================================================================
	q: Quit
	a: Ask the sensor to read temperature
	b: Make leds blink on sensor
	c + <nb>: Send a custom command to sensor. 'c0' should list the available commands.
	g: Generate test data
	e: Clear stored data
	p: Print stored data
	---------------------------------------------------------------------------------------

	Creating sensor proxy with id=1000 on port /dev/ttyUSB0
	2015-04-02 16:21:16  9908 WARN  POP-C++ exception in paroc_broker::DoInvoke Cannot open device: /dev/ttyUSB1 (broker_serve.cc:214 DoInvoke)
	2015-04-02 16:21:16  9928 WARN  POP-C++ exception in paroc_broker::DoInvoke Cannot open device: /dev/ttyUSB2 (broker_serve.cc:214 DoInvoke)
	2015-04-02 16:21:16  9948 WARN  POP-C++ exception in paroc_broker::DoInvoke Cannot open device: /dev/ttyUSB3 (broker_serve.cc:214 DoInvoke)
	Created 1 sensor proxy objects
	Enter selection:

```

By typing commands in the application you will be able to interact with your sensors. Enjoy!


Installation of POP-C++ with support for templates
--------------------------------------------------
TODO: Add this version of POPC to github

Installation of contiki files for xm1000
----------------------------------------
To compile the code for xm1000 mote, you will need to add the following files to your Contiki directory:

In directory popwin/gatewayMote: decompress the files in archive contiki-xm1000-v2012-08-29.rar. Then copy the content of tools/ and platform/ directories in your Contiki directory.

Remember that you have to copy and paste the appropriate files (2 files) of the xm1000 (available in the Advantic website) in the file system of InstantContiki2.6 (contiki->platform and contiki->tools).

Remember also to call to the target xm1000 instead to call the target sky.

This is like this because the xm1000 sensor have a better micro-controller than usual Tmotes Sky and we have to use the appropriate drivers for this micro-controller.

	http://www.advanticsys.com/wiki/index.php?title=XM1000

Troubleshooting
---------------
### Problem
The sensors outputs garbage characters with serialdump or nothing at all
### Solution
- The reset (using the sensor button) is not handled properly. You need to unplug the device of USB port and plug it back in after at least 5 seconds.

- And also check that the baudrate of your port is set to 115200. You can get the current value with:

	stty -F /dev/ttyUSB0

The SensorProxy class of POPWin will try to reset the value to 115200 but this does apparently not work each time.

- It seems that the upload of a program on the mote does not succeed each time (this problem appeared with contiki 2.6). The upload may have to be repeated many times ... annoying.

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

