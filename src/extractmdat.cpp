//============================================================================
// Name        : extractmdat.cpp
// Author      : Jonas Stein
// Version     :
// Copyright   : Your copyright notice
// Description : Extract events with timestamps from binary mdat files
//============================================================================

#include <iostream>
#include <string>
#include "QMRConfig.h"
#include "Mdatevent.h"


int main() {
	std::cout << "Version: " << QMR_VERSION_STRING << std::endl;

	Mdatevent ev{};
//	IDClass TestID;
/*
	TestID = ev.getEventID();
	if (IDClass::trigger == TestID) {
		std::cout<< "This was a trigger\n";
	}
	else
		{
		std::cout<< "This was a neutron\n";
		};
*/
	return 0;
}
