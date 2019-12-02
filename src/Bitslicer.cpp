#include "Bitslicer.h"
#include <cmath>
#include <cstdint> // int8_t

namespace bitslicer {

uint16_t byteswap(uint16_t word) {
  return (((word & 0xff) << 8) | ((word & 0xff00) >> 8));
}

uint64_t byteswap64(uint64_t value) {
  uint64_t partA = value & 0xffff000000000000;
  uint64_t partB = value & 0x0000ffff00000000;
  uint64_t partC = value & 0x00000000ffff0000;
  uint64_t partD = value & 0x000000000000ffff;
  return ((partA >> 48) + (partB >> 16) + (partC << 16) + (partD << 48));
}

uint64_t LowMidHigh(uint16_t LowWord, uint16_t MidWord, uint16_t HighWord) {
  uint64_t High64 = static_cast<uint64_t>(HighWord);
  High64 = High64 << 32;

  uint64_t Mid64 = static_cast<uint64_t>(MidWord);
  Mid64 = Mid64 << 16;

  uint64_t Low64 = static_cast<uint64_t>(LowWord);

  return (High64 + Mid64 + Low64);
}

uint64_t getintbybitpattern(uint64_t pattern, uint64_t cutpattern) {
  uint8_t firstbitset{0};
  firstbitset = log2(cutpattern & ~(cutpattern - 1));
  return ((pattern & cutpattern) >> firstbitset);
}

} // namespace bitslicer
