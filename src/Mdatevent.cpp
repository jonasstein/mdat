#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>      // int8_t

namespace mevent {

Mdatevent::Mdatevent() :
		EventID(IDClass::neutron), EventTimestamp(0b0),
		EventModID(0), EventSlotID(0), EventAmplitude(0), EventPosition(0),
		EventTrigID(TrigIDClass::CmpReg), EventDataID(DataIDClass::ADC1), EventData(0b0) {
}

Mdatevent::Mdatevent(IDClass myid, TimestampClass mytimestamp,
		ModIDClass EventModID, SlotIDClass EventSlotID, AmplitudeClass EventAmplitude, PositionClass EventPosition,
		TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata) :
		EventID(myid), EventTimestamp(mytimestamp),
		EventModID(0), EventSlotID(0), EventAmplitude(0), EventPosition(0),
		EventTrigID(mytrigid), EventDataID(mydataid), EventData(mydata) {
//	std::cout << "Constructor was called\n";
}



Mdatevent::~Mdatevent() {
}

IDClass Mdatevent::getEventID(void){
	return Mdatevent::EventID;
	}


void Mdatevent::printfullEvent(void){
	printf("This could be an event\n");
}


Mdatevent Mdatevent::neutronevent(IDClass myid, TimestampClass mytimestamp,
		ModIDClass EventModID, SlotIDClass EventSlotID, AmplitudeClass EventAmplitude, PositionClass EventPosition) {

	mevent::Mdatevent mynewevent{mevent::IDClass::neutron, mytimestamp,
		EventModID, EventSlotID, EventAmplitude, EventPosition,
		mevent::TrigIDClass::Source1,mevent::DataIDClass::Monitor1, 0};
	return mynewevent;
}


Mdatevent Mdatevent::triggernevent(IDClass myid, TimestampClass mytimestamp,
		TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata) {

	mevent::Mdatevent mynewevent{mevent::IDClass::neutron, mytimestamp,
			0, 0, 0, 0,
			mytrigid, mydataid, mydata};
		return mynewevent;
}


} /* namespace mevent */


// new neutronfromraw
// new neutronfromdata
// new triggerfromraw
// new triggerfromdata
// isneutron
// istrigger


/*
 * Neutron events
 * ID         = event[0] = 0   1 Bit
 * ModID      = event[1..3]    3 Bit
 * SlotID     = event[4..8]    5 Bit
 * Amplitude  = event[9..18]  10 Bit
 * Position  = event[19..28] 10 Bit
 * Timestamp  = event[29..47] 19 Bit
 *
 *
 * Trigger events
 * ID         = event[0] = 1   1 Bit
 * TrigID     = event[1..3]    3 Bit
 * DataID     = event[4..7]    4 Bit
 * Data       = event[8..28]  21 Bit
 * Timestamp  = event[29..47] 19 Bit
 */
