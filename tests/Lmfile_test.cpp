#include <gtest/gtest.h>
//#include <cmath>
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Lmfile.h"

//const std::string testFileName = "samples/180sec_2chan_5kHz_pm_4kHz_FM_4Hz_Trigger1Hz.mdat";

TEST(Bitoperations, byteswap)
{
    ASSERT_EQ(mfile::byteswap(0x00AA), 0xAA00);
    ASSERT_EQ(mfile::byteswap(0xABCD), 0xCDAB);
    ASSERT_EQ(mfile::byteswap(0xFFFF), 0xFFFF);
    ASSERT_EQ(mfile::byteswap(0x0000), 0x0000);
    ASSERT_EQ(mfile::byteswap(0x00FF), 0xFF00);
    ASSERT_EQ(mfile::byteswap(0x0F0F), 0x0F0F);
}


TEST(Bitoperations, LowMidHigh)
{
    ASSERT_EQ(mfile::LowMidHigh(0xAAAA, 0xBBBB, 0xCCCC), 0x0000CCCCBBBBAAAA);
    ASSERT_EQ(mfile::LowMidHigh(0x00AA, 0x00BB, 0x00CC), 0x000000CC00BB00AA);
}


/*TEST(CategoryTest, Plustest)
{
	mevent::Mdatevent testobject2 {mevent::IDClass::trigger,
					  mevent::TrigIDClass::CmpReg,
					  mevent::DataIDClass::Monitor1,
					  0b0,
					  0b0} ;

    ASSERT_EQ(testobject2.pluseins(1), 2);
}
*/

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

