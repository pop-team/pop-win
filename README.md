POPWin
======
- **Authors:** Laurent Winkler, Marco Lourenço
- **Date:**    November 2015

This document contains a general description of the POPWin project, specifications and examples of programmation. 

Contacts
--------
- EIFR
	- Pierre Kuonen <pierre.kuonen@hefr.ch>
	- Laurent Winkler <laurent.winkler@hefr.ch>
	- Marco Lourenço <marco.lourenco@hefr.ch>
	- Lu Yao <yao.lu@edu.hefr.ch>
- UNI GE
	- Pierre Leone <Pierre.Leone@unige.ch>
	- Cristina Munoz Illescas <Cristina.Munoz@unige.ch>

System overview
---------------
POPWin provides a set of tools (based on POP-C++) to build communication between a remote machine and a network of sensors. 

The complete system is devided into two parts:
- The **high-level** part handles the communication between different parallel objects that can run on different machines using the POP-C++ framework.
- The **low-level** part contains the sensor that communicate using radio waves. 
- In between the two parts a **Gateway sensor** must be used. It will forward the messages between the SensorProxy object (running on a PC) and the rest of the sensor network. Communication between the SensorProxy and gateway sensor is done via USB.

### Schema of the POPWin network

	                                                                        ************
	main -------------- POPSensor -- SensorProxy1 === GatewaySensor ***   *Sensor network*
	                            \                                           ************  
	                            |                                           
	                            |
	                            |                                                         
	                            \                                           ************  
	                             --- SensorProxy2 === GatewaySensor ***   *Sensor network*
	                                                                        ************
	                                     ...

	--- TCPIP communication
	=== USB communication
	*** Radio communication

As explained in the documentation of the POPWin project the communication uses different messaging models:
- **High level**: the POP communication model (by TCP/IP)
- **Low level**: the publish/subscribe model is used (using the libraries of Contiki OS)

A more complete scheme of the network: [POPWin architecture](POPWINarchi.pdf "POPWin architecture")

Resource description and messaging system
-----------------------------------------
To configure easily where to find sensors (described as resources) the application uses a **resource file** in JSON format. This file indicates how to connect to sensors and to which data the user wants to subscribe.

To set up communication between the different sensors of the low-level part and the nodes of the high-level parts of the system **a messaging interface** was developed. This interface allows to easily transcribe message from the publish-subscribe communication model used in the low-level system to the POP communication model used on the higher level.

Specifications of resource description and messaging can be found [here](SPECIFICATIONS.md). 

Programming
-----------
The goal of the POPWin project is to provide an easy programming interface to the developer. This interface allows to acquire data from different sensor types in a transparent way.

The full programming guide for POPWin can be found [here](PROGRAMMING.md). 

Quick start
-----------
To start the main example of POPWin:

1. Install POP-C++ on your computer (https://github.com/pop-team/pop-cpp)
2. Connect the gateway sensor to your computer via USB (Zolertia z1 in recommended)
3. Compile the files in directory **gatewayMote/** and upload them to your gateway sensor
4. Compile and launch POPWin main application

        cd pop-win
        make
        popcrun objects.map ./main_sql resources.json

For details on installation and troubleshooting please look at the [development documentation](INSTALL.md). 

