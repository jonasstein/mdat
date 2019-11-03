#ifndef SRC_MDATEVENT_H_
#define SRC_MDATEVENT_H_

#include <cstdint>       // int8_t
#include <type_traits>


namespace mevent {
template <typename E>
constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}


enum class IDClass : bool { neutron = 0, trigger = 1 };
enum class TrigIDClass : uint8_t{Timer1 = 0, Timer2, Timer3, Timer4,
	RearTTL1, RearTTL2,
	CmpReg}; // "Compare Register" is used most often to trigger.

enum class DataIDClass : uint8_t { Monitor1 = 0, Monitor2, Monitor3, Monitor4,
	RearTTL1, RearTTL2,
	ADC1, ADC2}; // Data source

using DataClass = uint32_t;      // 21 bit needed
using TimestampClass = uint64_t; // 48 bit offset from the header, 19 bit time in the event

using ModIDClass = uint8_t; // 3 Bit used
using SlotIDClass = uint8_t; // 5 Bit used
using AmplitudeClass = uint16_t; // 10 Bit used
using PositionClass = uint16_t; // 10 Bit used


struct eventLMH {
	uint8_t Lo : 8;
	uint8_t Mid : 8;
	uint8_t Hi : 8;};

class Mdatevent {
private:
	//properties of neutron and trigger events
	IDClass EventID;
	TimestampClass EventTimestamp;

	//properties of neutron events
	ModIDClass EventModID;
	SlotIDClass EventSlotID;
	AmplitudeClass EventAmplitude;
	PositionClass EventPosition;

	//properties of trigger events
	TrigIDClass EventTrigID;
	DataIDClass EventDataID;
	DataClass EventData;



public:
	Mdatevent();
	Mdatevent(IDClass myid, TimestampClass mytimestamp,
			ModIDClass EventModID, SlotIDClass EventSlotID, AmplitudeClass EventAmplitude, PositionClass EventPosition,
			TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata);

	Mdatevent neutronevent(TimestampClass mytimestamp,
			ModIDClass EventModID, SlotIDClass EventSlotID, AmplitudeClass EventAmplitude, PositionClass EventPosition);
	Mdatevent triggerevent(TimestampClass mytimestamp,
			TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata);
	Mdatevent importrawevent(char rawinput [6]);

	virtual ~Mdatevent();

	IDClass getEventID(uint64_t rawinteger);
	TrigIDClass getTrigID(uint64_t rawinteger);
	DataIDClass getDataID(uint64_t rawinteger);

	void printeventverbose();
	void printevent();



};

} /* namespace mevent */

#endif /* SRC_MDATEVENT_H_ */
