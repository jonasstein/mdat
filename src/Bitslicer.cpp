#include "Bitslicer.h"
#include <cstdint>
#include <math.h>



namespace bitslicer{

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
