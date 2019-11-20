#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Bitslicer.h"

TEST(Bitsclicer, byteswap)
{
    ASSERT_EQ(bitslicer::byteswap(0x00AA), 0xAA00);
    ASSERT_EQ(bitslicer::byteswap(0xABCD), 0xCDAB);
    ASSERT_EQ(bitslicer::byteswap(0xFFFF), 0xFFFF);
    ASSERT_EQ(bitslicer::byteswap(0x0000), 0x0000);
    ASSERT_EQ(bitslicer::byteswap(0x00FF), 0xFF00);
    ASSERT_EQ(bitslicer::byteswap(0x0F0F), 0x0F0F);
}

TEST(Bitsclicer, byteswap64)
{
    ASSERT_EQ(bitslicer::byteswap64(0x0000111122223333), 0x3333222211110000);
    ASSERT_EQ(bitslicer::byteswap64(0xaaaa5555ffff0000), 0x0000ffff5555aaaa);
    ASSERT_EQ(bitslicer::byteswap64(0xffffffffeeeedddd), 0xddddeeeeffffffff);
}

TEST(Bitsclicer, LowMidHigh)
{
    ASSERT_EQ(bitslicer::LowMidHigh(0xAAAA, 0xBBBB, 0xCCCC), 0x0000CCCCBBBBAAAA);
    ASSERT_EQ(bitslicer::LowMidHigh(0x00AA, 0x00BB, 0x00CC), 0x000000CC00BB00AA);
}


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
    return(RUN_ALL_TESTS());
}

