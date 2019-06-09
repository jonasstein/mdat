#ifndef SRC_MDATEVENT_H_
#define SRC_MDATEVENT_H_

#include <cstdint>       // int8_t


//namespace mevent {

enum class IDClass { neutron = 0, trigger = 1 };
enum class TrigIDClass { Source1 = 0, Source2, Source3, Source4,
	Timer1, Timer2, Timer3, Timer4,
	RearTTL1, RearTTL2,
	CmpReg}; // "Compare Register" is used most often to trigger.

enum class DataIDClass { Monitor1 = 0, Monitor2, Monitor3, Monitor4,
	RearTTL1, RearTTL2,
	ADC1, ADC2}; // Data source



class Mdatevent {
private:
	IDClass EventID;
	TrigIDClass EventTrigID;
	DataIDClass EventDataID;

public:
	Mdatevent();
	virtual ~Mdatevent();

	IDClass getEventID(void);
};

//} /* namespace mevent */

#endif /* SRC_MDATEVENT_H_ */
