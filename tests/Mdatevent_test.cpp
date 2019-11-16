#include <gtest/gtest.h>
//#include <cmath> 
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Mdatevent.h"
#include "../src/Bitslicer.h"


//const std::string testFileName = "samples/180sec_2chan_5kHz_pm_4kHz_FM_4Hz_Trigger1Hz.mdat";

TEST(importraw, trigger_0xffff)
{
	uint64_t sorted = 0b100000000000000000000000000000000000000000000000 + 0xffff;
	mevent::TimestampClass myBuffertime_ns {0x000000};
	mevent::Mdatevent newevent(sorted, myBuffertime_ns);
	ASSERT_EQ(newevent.printevent(), "6553500, 7, 0, 0\n");
}

TEST(importraw, neutron_0xffff)
{
	uint64_t sorted = 0b000000000000000000000000000000000000000000000000 + 0xffff;
	mevent::TimestampClass myBuffertime_ns {0x000000};
	mevent::Mdatevent newevent(sorted, myBuffertime_ns);
    ASSERT_EQ(newevent.printevent(), "6553500, 8, 0, 0\n");
}

TEST(importraw, trigger_addoffset)
{
	uint64_t sorted = 0b100000000000000000000000000000000000000000000000 + 0x1000;
	mevent::TimestampClass myBuffertime_ns {0xefff * 100};
	mevent::Mdatevent newevent(sorted, myBuffertime_ns);
    ASSERT_EQ(newevent.printevent(), "6553500, 7, 0, 0\n");
}


TEST(importraw, trigger_realraw)
{
	uint16_t Low = 0x407E;
	uint16_t Mid = 0x0008;
	uint16_t High  = 0xF000;


	uint64_t sorted = bitslicer::LowMidHigh(Low,Mid,High);
	mevent::TimestampClass myBuffertime_ns {0x000000};
	mevent::Mdatevent newevent(sorted, myBuffertime_ns);
    ASSERT_EQ(newevent.printevent(), "1651000, 7, 0, 1\n");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

