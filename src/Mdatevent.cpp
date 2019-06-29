#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>      // int8_t

namespace mevent {

Mdatevent::Mdatevent() {
	Mdatevent::EventID=IDClass::neutron;
	Mdatevent::EventTrigID=TrigIDClass::CmpReg;
	Mdatevent::EventDataID=DataIDClass::ADC1;
	Mdatevent::EventData=0b0;
	Mdatevent::EventTimestamp=0b0;
}

Mdatevent::Mdatevent(IDClass myid, TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata, TimestampClass mytimestamp){
	Mdatevent::EventID=myid;
	Mdatevent::EventTrigID=mytrigid;
	Mdatevent::EventDataID=mydataid;
	Mdatevent::EventData=mydata;
	Mdatevent::EventTimestamp=mytimestamp;
	std::cout << "Constructor was called\n";
}

Mdatevent::~Mdatevent() {
}

IDClass Mdatevent::getEventID(void){
	return Mdatevent::EventID;
	}


void Mdatevent::printEvent(void){
	printf("This could be an event\n");
}

int Mdatevent::pluseins(int a){
	int b{0};
	b=a+1;
	return b;
}

} /* namespace mevent */
