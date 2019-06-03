#ifndef SRC_MDATEVENT_H_
#define SRC_MDATEVENT_H_

#include <cstdint>       // int8_t


namespace mevent {

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


class Mdatevent {
private:
	EventID_t EventID;
	EventTrigID_t EventTrigID;
	EventDataID_t EventDataID;

public:
	//Mdatevent(EventID_t myEventID, EventTrigID_t myEventTrigID, EventDataID_t myEventDataID);
	Mdatevent();
	virtual ~Mdatevent();

	IDClass ID = IDClass::neutron;
	TrigIDClass TrigID = TrigIDClass::Source1;

	//Mdatevent NewNeutronEvent(int modID, int slotID, int amplitude, int position, int timestamp);
	Mdatevent NewTriggerEvent(EventID_t EventID, EventTrigID_t EventTrigID, EventDataID_t EventDataID);
	EventID_t getEventID(Mdatevent);
};

} /* namespace mevent */

#endif /* SRC_MDATEVENT_H_ */
