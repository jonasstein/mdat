//============================================================================
// Name        : extractmdat.cpp
// Author      : Jonas Stein
// Version     :
// Copyright   : Your copyright notice
// Description : Extract events with timestamps from binary mdat files
//============================================================================

#include <iostream>
#include "QMRConfig.h"
#include "Mdatevent.h"


int main() {
	std::cout << "!!!Hello World!!! Version:" << QMR_VERSION_MAJOR << std::endl;

	mevent::Mdatevent testevent { };
	testevent.mevent::Mdatevent::MakeNeutron(1, 1, 1, 1, 1);
	testevent.mevent::Mdatevent::PrintEvent();
	return 0;
}
