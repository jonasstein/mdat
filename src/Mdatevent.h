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
enum class TrigIDClass : uint8_t{ Source1 = 0, Source2, Source3, Source4,
	Timer1, Timer2, Timer3, Timer4,
	RearTTL1, RearTTL2,
	CmpReg}; // "Compare Register" is used most often to trigger.

enum class DataIDClass : uint8_t { Monitor1 = 0, Monitor2, Monitor3, Monitor4,
	RearTTL1, RearTTL2,
	ADC1, ADC2}; // Data source

using DataClass = uint32_t;      // 21 bit needed
using TimestampClass = uint64_t; // 48 bit offset from the header, 19 bit time in the event

struct eventLMH {
	uint8_t Lo : 8;
	uint8_t Mid : 8;
	uint8_t Hi : 8;};

class Mdatevent {
private:
	IDClass EventID;
	TrigIDClass EventTrigID;
	DataIDClass EventDataID;
	DataClass EventData;
	TimestampClass EventTimestamp;

	struct event { IDClass ID : 1;
			 	  TrigIDClass TrigID : 3;
				  DataIDClass DataID : 4;
				  uint32_t Data : 21;
				  uint32_t Timestamp : 19;};


public:
	Mdatevent();
	Mdatevent(IDClass myid, TrigIDClass mytrigid, DataIDClass mydataid, DataClass mydata, TimestampClass mytimestamp);
	virtual ~Mdatevent();
	IDClass getEventID(void);
	void printfullEvent();
	//event getevent(triggerevent ev);
	int pluseins(int a);
};

} /* namespace mevent */

#endif /* SRC_MDATEVENT_H_ */
