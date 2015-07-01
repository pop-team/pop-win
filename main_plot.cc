/**
 *
 *
 * @author Laurent Winkler based on work by Valentin Bourqui
 * @date   May 2015
 * @brief  A small executable used to plot an existing CSV file
 *
 *
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "POPSensorData.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "usage: " << argv[0] << " data.csv" << popcendl;
		exit(0);
	}

	try
	{
		POPSensorData data;
		ifstream ifs;
		ifs.open(argv[1]);
		data.ReadFromFile(ifs);
		ifs.close();
		cout << "Read " << data.GetSize() << " records" << popcendl;

		stringstream ss;
		ss << "mkdir -p " << argv[1] << "_plots";
		system(ss.str().c_str());

		// Create one plot per sensor id with all measurement
		for(auto id : data.GroupAllIds())
		{
			string target = string(argv[1]) + "_plots/sensor"  + to_string(id) + ".html";
			cout << "Create " << target << popcendl;
			data.PrintToPlot(target, id);
		}

		// Create one plot per measurement with all sensors
		for(auto mt : data.GroupAllMeasurementTypes())
		{
			string target = string(argv[1]) + "_plots/"  + explainMeasurementType(mt) + ".html";
			cout << "Create " << target << popcendl;
			data.PrintToPlot(target, mt);
		}
	}
	catch(std::exception &e)
	{
		cerr<<"Exception caught in " << argv[0] << ": " << e.what() << popcendl;
		return 1;
	}
	catch(...)
	{
		cerr<<"Exception caught in popwin main loop" << argv[0] << popcendl;
		return 1;
	}


	return 0;
}
