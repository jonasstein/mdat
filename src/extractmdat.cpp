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
#include "Lmfile.h"


int main() {
	std::cout << "Version: " << QMR_VERSION_STRING << std::endl;

	mevent::Mdatevent testobject{};
	//testobject = mevent::Mdatevent			;
	//(mevent::IDClass::trigger,		  mevent::TrigIDClass::CmpReg,					  mevent::DataIDClass::Monitor1,					  0b0,					  0b0);

	//testobject2.printEvent();


	mevent::IDClass TestID{mevent::IDClass::trigger};
	std::cout<< mevent::to_underlying(TestID);

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
