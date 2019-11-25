#include <gtest/gtest.h>
#include "../src/Histogram.h"
#include "../src/Timestamps.h"
#include <cstdint> // int8_t
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

TEST(Histogram, add_Event)
{

uint64_t NumberOfBins = 10;
	histo::Histogram myhisto(NumberOfBins, 10000);
	TimestampClass myevent = 1000;
	myhisto.put(myevent);
	myevent = 10000000;
	myhisto.put(myevent);
	myevent = 11000;
	myhisto.put(myevent);
	myhisto.put(myevent);
	myhisto.put(myevent);

std::vector<TimestampClass> testvector;
testvector=myhisto.getbins();
ASSERT_EQ(testvector.size(), NumberOfBins);

testvector=myhisto.getfrequency();
ASSERT_EQ(testvector.size(), NumberOfBins);

std::string s = myhisto.frequencystring();
ASSERT_EQ(s, "2,3,0,0,0,0,0,0,0,0");
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}

