#ifndef SRC_GLOBALTYPES_H_
#define SRC_GLOBALTYPES_H_

#include <cstdint>               // int8_t
using TimestampClass = uint64_t; // 3x16 = 48 bit offset from the header

TimestampClass integermilliseconds(TimestampClass time_ns) {
	return ( time_ns / 1000000.0);
}


#endif /* SRC_GLOBALTYPES_H_ */
