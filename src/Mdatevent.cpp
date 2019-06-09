#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>       // int8_t

//namespace mevent {


Mdatevent::Mdatevent() {
	Mdatevent::EventID=IDClass::neutron;
	Mdatevent::EventTrigID=TrigIDClass::CmpReg;
	Mdatevent::EventDataID=DataIDClass::ADC1;
}


Mdatevent::~Mdatevent() {
}


IDClass Mdatevent::getEventID(void){
	return Mdatevent::EventID;
	}


//} /* namespace mevent */
