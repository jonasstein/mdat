#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Bitslicer.h"

TEST(Bitsclicer, PatternA)
{
	uint64_t bitpattern = 0b100101010001000100001000100001001010101001111010;
	uint64_t cutpattern = 0b011100000000000000000000000000000000000000000000;
	uint64_t testobject2 {bitslicer::getintbybitpattern(bitpattern, cutpattern)};
    ASSERT_EQ(testobject2, 0b001);
}

TEST(Bitsclicer, PatternB)
{
	uint64_t bitpattern = 0b100101010001000100001000100001001010101001111010;
	uint64_t cutpattern = 0b000011110000000000000000000000000000000000000000;
	uint64_t testobject2 {bitslicer::getintbybitpattern(bitpattern, cutpattern)};
    ASSERT_EQ(testobject2, 0b0101);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

