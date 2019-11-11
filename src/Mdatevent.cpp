#include "Mdatevent.h"

#include "Bitslicer.h"
#include <cstdint>  // int8_t
#include <iostream> // std::cout
#include <sstream>
#include <string>

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
      EventDataID(mydataid), EventData(mydata) {}

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

void Mdatevent::importevent(uint64_t sortedevent) {
	this->EventID = getEventID(bitslicer::getintbybitpattern(
      sortedevent, 0b100000000000000000000000000000000000000000000000));
	this->EventTrigID = getTrigID(bitslicer::getintbybitpattern(
      sortedevent, 0b011100000000000000000000000000000000000000000000));
	this->EventDataID = getDataID(bitslicer::getintbybitpattern(
      sortedevent, 0b000011110000000000000000000000000000000000000000));
	this->EventData = bitslicer::getintbybitpattern(
      sortedevent, 0b000000001111111111111111111110000000000000000000);
	this->EventTimestamp = bitslicer::getintbybitpattern(
      sortedevent, 0b000000000000000000000000000001111111111111111111);
}

std::string Mdatevent::printeventverbose(void) {
  std::stringstream buffer;

  if (this->EventID == IDClass::neutron) {
    buffer << "EventID: " << static_cast<int>(EventID) << " (neutron event) \n";
    buffer << "EventTimestamp: " << static_cast<int>(EventTimestamp) << "\n";

    buffer << "EventModID: " << static_cast<int>(EventModID) << "\n";
    buffer << "EventSlotID: " << static_cast<int>(EventSlotID) << "\n";
    buffer << "EventAmplitude: " << static_cast<int>(EventAmplitude) << "\n";
    buffer << "EventPosition: " << static_cast<int>(EventPosition) << "\n";
  }

  if (this->EventID == IDClass::trigger) {
    buffer << "EventID: " << static_cast<int>(EventID) << " (trigger event) \n";
    buffer << "EventTimestamp: " << static_cast<int>(EventTimestamp) << "\n";

    buffer << "EventTrigID: " << static_cast<int>(EventTrigID) << "\n";
    buffer << "EventDataID: " << static_cast<int>(EventDataID) << "\n";
    buffer << "EventData: " << static_cast<int>(EventData) << "\n";
  }
  return buffer.str();
}

std::string Mdatevent::printevent(void) {
  std::stringstream buffer;

  if (this->EventID == IDClass::neutron) {
    buffer << static_cast<int>(EventTimestamp) << ", ";
    buffer << "8, "; // 0..7 are trigger events 8 are neutron events
    buffer << static_cast<int>(EventModID) << ", ";
    buffer << static_cast<int>(EventSlotID) << ", ";
    buffer << static_cast<int>(EventAmplitude) << ", ";
    buffer << static_cast<int>(EventPosition) << "\n";
  }
  if (this->EventID == IDClass::trigger) {
    buffer << static_cast<int>(EventTimestamp) << ", ";
    ;
    buffer << static_cast<int>(EventTrigID) << ", ";
    ;
    buffer << static_cast<int>(EventDataID) << ", ";
    ;
    buffer << static_cast<int>(EventData) << "\n";
    ;
  }
  return buffer.str();
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
