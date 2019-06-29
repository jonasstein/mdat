#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>      // int8_t

namespace mevent {

Mdatevent::Mdatevent() :
		EventID(IDClass::neutron), EventTrigID(TrigIDClass::CmpReg), EventDataID(DataIDClass::ADC1), EventData(0b0), EventTimestamp(0b0) {
}

Mdatevent::Mdatevent(IDClass myid, TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata, TimestampClass mytimestamp) :
		EventID(myid), EventTrigID(mytrigid), EventDataID(mydataid), EventData(mydata), EventTimestamp(mytimestamp) {
//	std::cout << "Constructor was called\n";
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
