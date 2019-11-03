#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Bitslicer.h"

TEST(CategoryTest, Bitsclicer)
{
	uint64_t testobject2 {bitslicer::getintbybitpattern(0b100101010001000100001000100001001010101001111010,
			0b011100000000000000000000000000000000000000000000)};

    ASSERT_EQ(testobject2, 0b001);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
