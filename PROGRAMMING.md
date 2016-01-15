Programming manual
==================

Examples of programmation
-------------------------

### main_sql.cpp
This file contains the a simple example on how to uses the different classes of the POPWin project. The main code of this examples is:

    // Create a POPSensor object on the local machine with resource description
	POPSensor popSensor("localhost", "resource.json");

	// wait ...
	
	// Send a command to all sensors of WSN to toggle green LED
    popSensor.Broadcast(MSR_LED, UNT_NONE, LED_GREEN_TOGGLE);
    
	// Gather all data from WSN database
	POPSensorData data = popSensor.executeQuery("SELECT * FROM popwin_schema.POPSensorData");
	data.printAll();

	// Clear data
	popSensor.clear();

In case you don't want everything from the database, the complete list of columns is : idPOPSensorData, type, genre, location, sensorID, value, unit. This could give for example :

    popSensor.executeQuery("SELECT genre,value,unit FROM popwin_schema.POPSensorData");

As shown here the POPSensor object can be used in a very simple and efficient manner. For the programmer this class can be used transparently.

To compile and run this example use:

```
	cd pop-win
	make
	popcrun objects.map ./main_sql resources.json
```


### main_demo.cpp
This file contains a more complete use of the POPWin objects. It also instantiates a remote POPSensor objects.

The best way to run the demo is by having two virtual machines (with SSH keys already exchanged) and connect one Wireless Sensor Network to each VM. Then configure the IP addresses accordingly in the makefile and in the two json files (one per POPSensor, so one per VM). The two VMs must have the same file structure for simplicity of use. For example, install popwin in the home folder on both VMs.

To compile and run this example use:

```
	cd pop-win
	make
	popcrun objects.map ./main_demo resources.json
```

### main_plot.cpp
This file contains a small utility that can be used to plot data from a csv file. The csv must have been generated previously. Here is an example of command:

```
	popcrun objects.map ./main_plot out/data_20150527_08:29:46.csv
```

Main parallel classes
---------------------
This chapter describes in more details the main classes used in this project.

### POPSensor parallel class
POPSensor is the primary class to use in a POPWin application. It allows the user to access the remote sensors. Its main methods are:

- **Constructor**: specifies the IP of the machine connected to the target WSN and the name of the JSON file that describes resources and start acquiring data from the remote sensors.
- **ExecuteQuery**: returns the data that was gathered by the SQL request in the form of a POPSensorData object.
- **Broadcast**  : broadcast one message to all sensors in the WSN

[Full definition](POPSensor.ph). 

### SensorProxy parallel class
One POPSensor object can connect to one or many SensorProxy class. This second class handles the communication with the gateway sensor. The POPSensor object will create one SensorProxy for each gateway sensor connected on USB. This is transparent to the programmer so this class should not be used directly.

[Full definition](SensorProxy.ph). 

### POPSensorData class
A POPSensorData object is returned by the ExecuteQuery() method. It contains all the data collected by the sensors including:

- Sensor id
- Time stamp
- Measurement type
- Measurement unit

Additionally this class provides the methods:

- **printAll**       : Prints the data to the standard output
- **clear**       : Clears the data
- **getRow**     : Returns the number of rows
- **insert**      : Insert a new data row into the structure

[Full definition](POPSensorData.h). 

