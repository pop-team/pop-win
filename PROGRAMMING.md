
Examples of programmation
-------------------------

TODO: Write a manual

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

	cd pop-win
	make
	popcrun objects.map ./main_example resources.json


### main.cpp
This file contains a more complete use of the POPWin objects. It also instantiates a POPSensor objects and lets the user send commands via the keyboard. This example can be used to test the system.

To compile and run this example use:

	cd pop-win
	make
	popcrun objects.map ./main resources.json

