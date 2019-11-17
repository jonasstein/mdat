#include <gtest/gtest.h>
//#include <cmath>
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Lmfile.h"

const std::string testFileA = "39events.mdat";

TEST(lmfile, set_read_verbosity)
{
//	uint64_t sorted = 0b100000000000000000000000000000000000000000000000;
//	mevent::TimestampClass myBuffertime_ns {0x0 * 100};
//	mevent::set
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileA, 0);
    ASSERT_EQ(limo->getfilesize(), 308);
    delete limo;

}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

