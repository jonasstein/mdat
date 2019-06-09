#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>       // int8_t

//namespace mevent {

//typedef uint8_t EventID_t;
//typedef uint8_t EventTrigID_t;
//typedef uint8_t EventDataID_t;

using EventID_t = uint8_t;
using EventTrigID_t = uint8_t;
using EventDataID_t = uint8_t;


Mdatevent::Mdatevent() {
	//EventID = mevent::IDClass::0;
	//EventTrigID = mevent::TrigIDClass::CmpReg; //myEventTrigID;
	//EventDataID = mevent::DataIDClass::ADC1; //myEventDataID;
}


Mdatevent::~Mdatevent() {
}


EventID_t getEventID(void){
	return EventID;
	}


//} /* namespace mevent */
