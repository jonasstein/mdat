#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Mdatevent.h"

//const std::string testFileName = "samples/180sec_2chan_5kHz_pm_4kHz_FM_4Hz_Trigger1Hz.mdat";

TEST(CategoryTest, SpecificTest)
{
    ASSERT_EQ(1, 1);
}


TEST(CategoryTest, ZeroTest)
{
    ASSERT_EQ(0, 1);
}


TEST(CategoryTest, Plustest)
{
	mevent::Mdatevent testobject2 {mevent::IDClass::trigger,
					  mevent::TrigIDClass::CmpReg,
					  mevent::DataIDClass::Monitor1,
					  0b0,
					  0b0} ;

    ASSERT_EQ(testobject2.pluseins(1), 2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

