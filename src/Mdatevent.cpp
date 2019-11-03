#include "Mdatevent.h"

#include "Bitslicer.h"
#include <cstdint>  // int8_t
#include <iostream> // std::cout

namespace mevent {

Mdatevent::Mdatevent()
    : EventID(IDClass::neutron), EventTimestamp(0b0), EventModID(0),
      EventSlotID(0), EventAmplitude(0), EventPosition(0),
      EventTrigID(TrigIDClass::CmpReg), EventDataID(DataIDClass::ADC1),
      EventData(0b0) {}

Mdatevent::Mdatevent(IDClass myid, TimestampClass mytimestamp,
                     ModIDClass EventModID, SlotIDClass EventSlotID,
                     AmplitudeClass EventAmplitude, PositionClass EventPosition,
                     TrigIDClass mytrigid, DataIDClass mydataid,
                     DataClass mydata)
    : EventID(myid), EventTimestamp(mytimestamp), EventModID(0), EventSlotID(0),
      EventAmplitude(0), EventPosition(0), EventTrigID(mytrigid),
      EventDataID(mydataid), EventData(mydata) {
}

Mdatevent::~Mdatevent() {}

IDClass getEventID(uint64_t rawinteger) {
  mevent::IDClass myreturn;

  switch (rawinteger) {
  case 0: {
    myreturn = mevent::IDClass::neutron;
    break;
  }
  default:
    myreturn = mevent::IDClass::trigger;
  }
  return myreturn;
}

TrigIDClass getTrigID(uint64_t rawinteger) {
  mevent::TrigIDClass myreturn;

  switch (rawinteger) {
  case 1: {
    myreturn = mevent::TrigIDClass::Timer1;
    break;
  }
  case 2: {
    myreturn = mevent::TrigIDClass::Timer2;
    break;
  }
  case 3: {
    myreturn = mevent::TrigIDClass::Timer3;
    break;
  }
  case 4: {
    myreturn = mevent::TrigIDClass::Timer4;
    break;
  }
  case 5: {
    myreturn = mevent::TrigIDClass::RearTTL1;
    break;
  }
  case 6: {
    myreturn = mevent::TrigIDClass::RearTTL2;
    break;
  }
  default: {
    myreturn = mevent::TrigIDClass::CmpReg;
    break;
  }
  }

  return myreturn;
}

DataIDClass getDataID(uint64_t rawinteger) {
  mevent::DataIDClass myreturn;

  switch (rawinteger) {
  case 0: {
    myreturn = mevent::DataIDClass::Monitor1;
    break;
  }
  case 1: {
    myreturn = mevent::DataIDClass::Monitor2;
    break;
  }
  case 2: {
    myreturn = mevent::DataIDClass::Monitor3;
    break;
  }
  case 3: {
    myreturn = mevent::DataIDClass::Monitor4;
    break;
  }
  case 4: {
    myreturn = mevent::DataIDClass::RearTTL1;
    break;
  }
  case 5: {
    myreturn = mevent::DataIDClass::RearTTL2;
    break;
  }
  case 6: {
    myreturn = mevent::DataIDClass::ADC1;
    break;
  }
  default: {
    myreturn = mevent::DataIDClass::ADC2;
    break;
  }
  }

  return myreturn;
}

Mdatevent Mdatevent::neutronevent(TimestampClass mytimestamp,
                                  ModIDClass EventModID,
                                  SlotIDClass EventSlotID,
                                  AmplitudeClass EventAmplitude,
                                  PositionClass EventPosition) {

  mevent::Mdatevent mynewevent{mevent::IDClass::neutron,
                               mytimestamp,
                               EventModID,
                               EventSlotID,
                               EventAmplitude,
                               EventPosition,
                               mevent::TrigIDClass::Timer1,
                               mevent::DataIDClass::Monitor1,
                               0};
  return mynewevent;
}

Mdatevent Mdatevent::triggerevent(TimestampClass mytimestamp,
                                  TrigIDClass mytrigid, DataIDClass mydataid,
                                  DataClass mydata) {

  mevent::Mdatevent mynewevent{mevent::IDClass::trigger,
                               mytimestamp,
                               0,
                               0,
                               0,
                               0,
                               mytrigid,
                               mydataid,
                               mydata};
  return mynewevent;
}

Mdatevent Mdatevent::importrawevent(char rawinput[6]) {

  uint8_t LoWordA = rawinput[0];
  uint8_t LoWordB = rawinput[1];
  uint8_t MidWordA = rawinput[2];
  uint8_t MidWordB = rawinput[3];
  uint8_t HiWordA = rawinput[4];
  uint8_t HiWordB = rawinput[5];

  uint64_t fullevent = HiWordA << 40 + HiWordB << 32 + MidWordA << 24 + MidWordB
                               << 16 + LoWordA << 8 + LoWordB;

  IDClass myID        = getEventID(bitslicer::getintbybitpattern(fullevent, 0b100000000000000000000000000000000000000000000000));
  TrigIDClass mytrigid = getTrigID(bitslicer::getintbybitpattern(fullevent, 0b011100000000000000000000000000000000000000000000));
  DataIDClass mydataid = getDataID(bitslicer::getintbybitpattern(fullevent, 0b000011110000000000000000000000000000000000000000));
  u_int64_t mydata               = bitslicer::getintbybitpattern(fullevent, 0b000000001111111111111111111110000000000000000000);
  u_int64_t mytimestamp          = bitslicer::getintbybitpattern(fullevent, 0b000000000000000000000000000001111111111111111111);

  mevent::Mdatevent mynewevent{};
  mynewevent.triggerevent(mytimestamp, mytrigid, mydataid, mydata);
  return mynewevent;
}

void Mdatevent::printeventverbose(void) {

  if (this->EventID == IDClass::neutron) {
    std::cout << "EventID: " << static_cast<int>(EventID) << " (neutron event) \n";
    std::cout << "EventTimestamp: " << static_cast<int>(EventTimestamp) << "\n";

    std::cout << "EventModID: " << static_cast<int>(EventModID) << "\n";
    std::cout << "EventSlotID: " << static_cast<int>(EventSlotID) << "\n";
    std::cout << "EventAmplitude: " << static_cast<int>(EventAmplitude) << "\n";
    std::cout << "EventPosition: " << static_cast<int>(EventPosition) << "\n";
  }

  if (this->EventID == IDClass::trigger) {
	  std::cout << "EventID: " << static_cast<int>(EventID) << " (trigger event) \n";
	  std::cout << "EventTimestamp: " << static_cast<int>(EventTimestamp) << "\n";

	  std::cout << "EventTrigID: " << static_cast<int>(EventTrigID) << "\n";
	  std::cout << "EventDataID: " << static_cast<int>(EventDataID) << "\n";
	  std::cout << "EventData: " << static_cast<int>(EventData) << "\n";
  }
}

void Mdatevent::printevent(void) {
  if (this->EventID == IDClass::neutron) {
	std::cout << static_cast<int>(EventTimestamp) << ", ";
	std::cout << "8, "; // 0..7 are trigger events 8 are neutron events
	std::cout << static_cast<int>(EventModID)<< ", ";
	std::cout << static_cast<int>(EventSlotID)<< ", ";
	std::cout << static_cast<int>(EventAmplitude)<< ", ";
	std::cout << static_cast<int>(EventPosition)<< "\n";
  }

  if (this->EventID == IDClass::trigger) {
	  std::cout << static_cast<int>(EventTimestamp)<< ", ";;
	  std::cout << static_cast<int>(EventTrigID)<< ", ";;
	  std::cout << static_cast<int>(EventDataID)<< ", ";;
	  std::cout << static_cast<int>(EventData)<< "\n";;
  }
}

} /* namespace mevent */


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
