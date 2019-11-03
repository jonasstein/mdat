#include "Mdatevent.h"

#include <iostream>     // std::cout
#include <cstdint>      // int8_t
#include "Bitslicer.h"


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

Mdatevent Mdatevent::neutronevent(TimestampClass mytimestamp,
		ModIDClass EventModID, SlotIDClass EventSlotID, AmplitudeClass EventAmplitude, PositionClass EventPosition) {

	mevent::Mdatevent mynewevent{mevent::IDClass::neutron, mytimestamp,
		EventModID, EventSlotID, EventAmplitude, EventPosition,
		mevent::TrigIDClass::Source1,mevent::DataIDClass::Monitor1, 0};
	return mynewevent;
}


Mdatevent Mdatevent::triggerevent(TimestampClass mytimestamp,
		TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata) {

	mevent::Mdatevent mynewevent{mevent::IDClass::trigger, mytimestamp,
			0, 0, 0, 0,
			mytrigid, mydataid, mydata};
		return mynewevent;
}


Mdatevent Mdatevent::importrawevent(char rawinput [6]) {


	uint8_t LoWordA  = rawinput[0];
	uint8_t LoWordB  = rawinput[1];
	uint8_t MidWordA = rawinput[2];
	uint8_t MidWordB = rawinput[3];
	uint8_t HiWordA  = rawinput[4];
	uint8_t HiWordB  = rawinput[5];

	uint64_t fullevent = HiWordA  << 40 +
			             HiWordB  << 32 +
						 MidWordA << 24 +
						 MidWordB << 16 +
						 LoWordA  <<  8 +
						 LoWordB;

     u_int64_t myID        = (fullevent & 0b100000000000000000000000000000000000000000000000) >> 47;
	 u_int64_t mytrigid    = (fullevent & 0b011100000000000000000000000000000000000000000000) >> 44;
	 u_int64_t mydataid    = (fullevent & 0b000011110000000000000000000000000000000000000000) >> 40;
	 u_int64_t mydata      = (fullevent & 0b000000001111111111111111111110000000000000000000) >> 19;
	 u_int64_t mytimestamp = (fullevent & 0b000000000000000000000000000001111111111111111111);



	mevent::Mdatevent mynewevent{};
	mynewevent.triggerevent(mytimestamp, mevent::TrigIDClass::Timer1 , mevent::DataIDClass::ADC1 , mydata);
	return mynewevent;
}




void Mdatevent::printeventverbose(void){


	if (this->EventID == IDClass::neutron){
	printf("EventID %d (neutron event), ", this->EventID);
    printf("EventTimestamp %d, ", this->EventTimestamp);

	printf("EventModID %d, ", this->EventModID);
	printf("EventSlotID %d, ", this->EventSlotID);
	printf("EventAmplitude %d, ", this->EventAmplitude);
	printf("EventPosition %d\n", this->EventPosition);
	}

	if (this->EventID == IDClass::trigger){
	printf("EventID %d (trigger event), ", this->EventID);
	printf("EventTimestamp %d, ", this->EventTimestamp);

	printf("EventTrigID %d, ", this->EventTrigID);
	printf("EventDataID %d, ", this->EventDataID);
	printf("EventData %d\n", this->EventData);
	}
}



void Mdatevent::printevent(void){
	if (this->EventID == IDClass::neutron){
		printf("%d, ", this->EventTimestamp);
		printf("8, ");  // 0..7 are trigger events 8 are neutron events
		printf("%d, ", this->EventModID);
		printf("%d, ", this->EventSlotID);
		printf("%d, ", this->EventAmplitude);
		printf("%d\n", this->EventPosition);
		}

		if (this->EventID == IDClass::trigger){
		printf("%d, ", this->EventTimestamp);
		printf("%d, ", this->EventTrigID);
		printf("%d, ", this->EventDataID);
		printf("%d\n", this->EventData);
		}
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
