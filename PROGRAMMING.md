Programming manual
==================

Examples of programmation
-------------------------

### main_example.cpp
This file contains the a simple example on how to uses the different classes of the POPWin project. The main code of this examples is:

    // Create a POPSensor object on the local machine with resource description
	POPSensor popSensor("localhost", "resource.json");

	// Start listening to the remote sensors
	popSensor.StartListening();

	// wait ...

	// Stop listening
	popSensor.StopListening();

	// Print the gathered data
	POPSensorData data(popSensor.Gather());
	data.Print();

	// Clear data
	popSensor.Clear();

As shown here the POPSensor object can be used in a very simple and efficient manner. For the programmer this class can be used transparently.

To compile and run this example use:

```
	cd pop-win
	make
	popcrun objects.map ./main_example resources.json
```


### main.cpp
This file contains a more complete use of the POPWin objects. It also instantiates a POPSensor objects and lets the user send commands via the keyboard. This example can be used to test the system.

To compile and run this example use:

```
	cd pop-win
	make
	popcrun objects.map ./main resources.json
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

- **Constructor**: specifies the name of the JSON file that describes resources and start acquiring data from the remote sensors.
- **Gather**     : returns the data that was gathered in the form of a POPSensorData object.
- **Broadcast**  : broadcast one message to all sensors
- **Reduce**      : Compute and returns statistics on the data (size, min, max, aver, sum, stdev)

[Full definition](POPSensor.ph). 

### SensorProxy parallel class
One POPSensor object can connect to one or many POPSensor class. This second class handles the communication with the gateway sensor. The POPSensor object will create one POPSensor for each gateway sensor connected on USB. This is transparent to the programmer so this class should not be used directly.

[Full definition](SensorProxy.ph). 

### POPSensorData class
A POPSensorData object is returned by the Gather() method. It contains all the data collected by the sensors including:

- Sensor id
- Time stamp
- Measurement type
- Measurement unit

Additionally this class provides the mothods:

- **Print**       : Prints the data to the standard output
- **PrintToFile** : Outputs the data to a file in .csv format
- **Clear**       : Clears the data
- **GetSize**     : Returns the number of records
- **Insert**      : Insert new data into the structure
- **Reduce**      : Compute and returns statistics on the data (size, min, max, aver, sum, stdev)

[Full definition](POPSensorData.h). 

