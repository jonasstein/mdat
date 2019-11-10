#ifndef SRC_BITSLICER_H_
#define SRC_BITSLICER_H_

#include <cstdint>

namespace bitslicer{
uint16_t byteswap(uint16_t word);
uint64_t LowMidHigh(uint16_t LowWord, uint16_t MidWord, uint16_t HighWord);
uint64_t getintbybitpattern(uint64_t pattern, uint64_t cutpattern);
}



#endif /* SRC_BITSLICER_H_ */
