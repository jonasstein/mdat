#include "Mdatevent.h"
#include "Bitslicer.h"
#include "Globaltypes.h"
#include <cstdint>  // int8_t
#include <iostream> // std::cout
#include <sstream>
#include <string>

namespace mevent {

Mdatevent::Mdatevent()
    : EventID(IDClass::neutron), BufferTimestamp_ns(0b0),
      FullEventTimestamp_ns(0b0), EventModID(0), EventSlotID(0),
      EventAmplitude(0), EventPosition(0), EventTrigID(TrigIDClass::CmpReg),
      EventDataID(DataIDClass::ADC1), EventData(0b0) {}

Mdatevent::Mdatevent(uint64_t sortedevent,
                     TimestampClass myBufferTimestamp_ns) {

    this->EventID = getEventID(bitslicer::getintbybitpattern(
        sortedevent, 0b100000000000000000000000000000000000000000000000));

    if (IDClass::trigger == this->EventID) {

        this->EventTrigID = getTrigID(bitslicer::getintbybitpattern(
            sortedevent, 0b011100000000000000000000000000000000000000000000));
        this->EventDataID = getDataID(bitslicer::getintbybitpattern(
            sortedevent, 0b000011110000000000000000000000000000000000000000));
        this->EventData = bitslicer::getintbybitpattern(
            sortedevent, 0b000000001111111111111111111110000000000000000000);

        this->EventModID = 0;
        this->EventSlotID = 0;
        this->EventAmplitude = 0;
        this->EventPosition = 0;
    } else {
        this->EventTrigID = TrigIDClass::Timer1;
        this->EventDataID = DataIDClass::ADC2;
        this->EventData = 0;

        this->EventModID = bitslicer::getintbybitpattern(
            sortedevent, 0b011100000000000000000000000000000000000000000000);
        this->EventSlotID = bitslicer::getintbybitpattern(
            sortedevent, 0b000011111000000000000000000000000000000000000000);
        this->EventAmplitude = bitslicer::getintbybitpattern(
            sortedevent, 0b000000000111111111100000000000000000000000000000);
        this->EventPosition = bitslicer::getintbybitpattern(
            sortedevent, 0b000000000000000000011111111110000000000000000000);
    }

    TimestampClass time19bit_ns =
        100 *
        bitslicer::getintbybitpattern(
            sortedevent, 0b000000000000000000000000000001111111111111111111);
    this->BufferTimestamp_ns = myBufferTimestamp_ns;
    this->FullEventTimestamp_ns = myBufferTimestamp_ns + time19bit_ns;
}

Mdatevent::~Mdatevent() {}

IDClass getEventID(uint64_t rawinteger) {
    switch (rawinteger) {
    case 0: {
        return (mevent::IDClass::neutron);
    }
    default:
        return (mevent::IDClass::trigger);
    }
}

TrigIDClass getTrigID(uint64_t rawinteger) {
    switch (rawinteger) {
    case 1: {
        return (mevent::TrigIDClass::Timer1);
    }
    case 2: {
        return (mevent::TrigIDClass::Timer2);
    }
    case 3: {
        return (mevent::TrigIDClass::Timer3);
    }
    case 4: {
        return (mevent::TrigIDClass::Timer4);
    }
    case 5: {
        return (mevent::TrigIDClass::RearTTL1);
    }
    case 6: {
        return (mevent::TrigIDClass::RearTTL2);
    }
    default: {
        return (mevent::TrigIDClass::CmpReg);
    }
    }
}

DataIDClass getDataID(uint64_t rawinteger) {
    switch (rawinteger) {
    case 0: {
        return (mevent::DataIDClass::Monitor1);
    }
    case 1: {
        return (mevent::DataIDClass::Monitor2);
    }
    case 2: {
        return (mevent::DataIDClass::Monitor3);
    }
    case 3: {
        return (mevent::DataIDClass::Monitor4);
    }
    case 4: {
        return (mevent::DataIDClass::RearTTL1);
    }
    case 5: {
        return (mevent::DataIDClass::RearTTL2);
    }
    case 6: {
        return (mevent::DataIDClass::ADC1);
    }
    default: {
        return (mevent::DataIDClass::ADC2);
    }
    }
}

std::string Mdatevent::printeventverbose(void) {
    std::stringstream buffer;

    if (this->EventID == IDClass::neutron) {
        buffer << "EventID: " << static_cast<uint64_t>(EventID)
               << " (neutron event) \n";
        buffer << "FullEventTimestamp: "
               << static_cast<uint64_t>(FullEventTimestamp_ns) << " ns\n";

        buffer << "EventModID: " << static_cast<uint64_t>(EventModID) << "\n";
        buffer << "EventSlotID: " << static_cast<uint64_t>(EventSlotID) << "\n";
        buffer << "EventAmplitude: " << static_cast<uint64_t>(EventAmplitude)
               << "\n";
        buffer << "EventPosition: " << static_cast<uint64_t>(EventPosition)
               << "\n";
    }

    if (this->EventID == IDClass::trigger) {
        buffer << "EventID: " << static_cast<uint64_t>(EventID)
               << " (trigger event) \n";
        buffer << "FullEventTimestamp: "
               << static_cast<uint64_t>(FullEventTimestamp_ns) << " ns\n";

        buffer << "EventTrigID: " << static_cast<uint64_t>(EventTrigID) << "\n";
        buffer << "EventDataID: " << static_cast<uint64_t>(EventDataID) << "\n";
        buffer << "EventData: " << static_cast<uint64_t>(EventData) << "\n";
    }
    return (buffer.str());
}

std::string Mdatevent::printevent(void) {
    std::stringstream buffer;

    if (this->EventID == IDClass::neutron) {
        buffer << static_cast<uint64_t>(FullEventTimestamp_ns) << " ";
        buffer << "8 "; // 0..7 are trigger events 8 are neutron events
        buffer << static_cast<uint64_t>(EventModID) << " ";
        buffer << static_cast<uint64_t>(EventSlotID);
    }
    if (this->EventID == IDClass::trigger) {
        buffer << static_cast<uint64_t>(FullEventTimestamp_ns) << " ";
        buffer << static_cast<uint64_t>(EventTrigID) << " ";
        buffer << static_cast<uint64_t>(EventDataID) << " ";
        buffer << static_cast<uint64_t>(EventData);
    }
    return (buffer.str());
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
