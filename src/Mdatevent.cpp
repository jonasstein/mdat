#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>       // int8_t

namespace mevent {

/*
Mdatevent::Mdatevent(EventID_t myEventID, EventTrigID_t myEventTrigID, EventDataID_t myEventDataID) {
	EventID = myEventID;
	EventTrigID = myEventTrigID;
	EventDataID = myEventDataID;
}
*/

Mdatevent::Mdatevent() {
	//EventID = mevent::IDClass::0;
	//EventTrigID = mevent::TrigIDClass::CmpReg; //myEventTrigID;
	//EventDataID = mevent::DataIDClass::ADC1; //myEventDataID;
}


Mdatevent::~Mdatevent() {
}

/*
Mdatevent Mdatevent::NewNeutron(int modID, int slotID, int amplitude, int position, int timestamp){
	Mdatevent NewEvent { };

	NewEvent.ID = IDClass::neutron;
	return NewEvent;
}
*/



EventID_t getEventID(Mdatevent thisevent){
	return thisevent.ID;
	}


} /* namespace mevent */
