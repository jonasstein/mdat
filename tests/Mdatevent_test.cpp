#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Mdatevent.h"
#include "../src/Bitslicer.h"


//const std::string testFileName = "samples/180sec_2chan_5kHz_pm_4kHz_FM_4Hz_Trigger1Hz.mdat";

TEST(CategoryTest, SpecificTest)
{
	mevent::Mdatevent newevent;

	uint16_t Low = bitslicer::byteswap(0x5429);
	uint16_t Mid = bitslicer::byteswap(0x003C);
	uint16_t High = bitslicer::byteswap(0xF000);

	uint64_t sorted = 0b100000000000000000000000000000000000000000000000;
			//bitslicer::LowMidHigh(Low,Mid,High);

    newevent.importevent(sorted);

    ASSERT_EQ(newevent.printevent(), "test");
}


TEST(Mevent, creator)
{
	mevent::Mdatevent newevent;

    ASSERT_EQ(0, 0);
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

