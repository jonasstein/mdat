#include "Bitslicer.h"
#include <cmath>
#include <cstdint>      // int8_t

namespace bitslicer{

uint16_t byteswap(uint16_t word){
	return ((word & 0xff) << 8) | ((word & 0xff00) >> 8);
}

uint64_t LowMidHigh(uint16_t LowWord, uint16_t MidWord, uint16_t HighWord){
	return ((static_cast<uint64_t> (HighWord)<< 32) + (static_cast<uint64_t> (MidWord)<<16) + (static_cast<uint64_t> (LowWord)));
	}


uint64_t getintbybitpattern(uint64_t pattern, uint64_t cutpattern){
/*
 * typical values:
 *
 * pattern    = 0b100101010001000100001000100001001010101001111010;
 * cutpattern = 0b011100000000000000000000000000000000000000000000;
 *
 */

	uint8_t firstbitset {0};
	firstbitset = log2(cutpattern & ~(cutpattern-1));

return ((pattern & cutpattern) >> firstbitset);
}

}
