POPWin
======
- **Author:** Laurent Winkler
- **Date:**   Decembre 2014

This document contains a general description of the POPWin project, specifications and examples of programmation. 

1. For a more technical documentation (installation, troubleshooting, ...) the developer documentation can be found here [here](INSTALL.md). 
2. The full programming guide for POPWin can be found [here](PROGRAMMING.md). 
3. Specifications of resource description and messaging can be found [here](SPECIFICATIONS.md). 

Contacts
--------
- EIFR
	- Pierre Kuonen <pierre.kuonen@hefr.ch>
	- Laurent Winkler <laurent.winkler@hefr.ch>
	- Lu Yao <yao.lu@edu.hefr.ch>
- UNI GE
	- Pierre Leone <Pierre.Leone@unige.ch>
	- Cristina Munoz Illescas <Cristina.Munoz@unige.ch>

System overview
---------------
POPWin provides a set of tools (based on POP-C++) to build communication between a remote machine and a network of sensors. 

The complete system is devided into two parts. The **high-level** part handles the communication between different parallel objects that can run on different machines using the POP-C++ framework. The **low-level** part contains the sensor that communicate using radio waves. Inbetween the two parts a GatewaySensor must be used. It will forward the messages between the SensorProxy object (running on a PC) and the rest of the sensor network. Communication between the SensorProxy and GatewaySensor is done via USB.


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
- **High level**: the POP model is used
- **Low level**: the publish/subscribe model is used

A more complete scheme of the network can be found [here](POPWINarchi.pdf) !




