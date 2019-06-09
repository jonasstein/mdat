#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>       // int8_t

//namespace mevent {

enum class IDClass { neutron = 0, trigger = 1 };
enum class TrigIDClass { Source1 = 0, Source2, Source3, Source4,
	Timer1, Timer2, Timer3, Timer4,
	RearTTL1, RearTTL2,
	CmpReg}; // "Compare Register" is used most often to trigger.

enum class DataIDClass { Monitor1 = 0, Monitor2, Monitor3, Monitor4,
	RearTTL1, RearTTL2,
	ADC1, ADC2}; // Data source


typedef uint8_t EventID_t;
typedef uint8_t EventTrigID_t;
typedef uint8_t EventDataID_t;


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



EventID_t getEventID(void){
	return EventID;
	}


//} /* namespace mevent */
