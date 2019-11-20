#include <gtest/gtest.h>
//#include <cmath>
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Lmfile.h"

const std::string testFileA = "39events.mdat";
const std::string testFileB = "5linesheader.mdat";

TEST(lmfile, getfilesize)
{
	mfile::Lmfile limo(testFileA, 0);
    ASSERT_EQ(limo.getfilesize(), 308);
}

TEST(lmfile, set_read_verbosity)
{
	mfile::Lmfile lmA(testFileA, 0);
    ASSERT_EQ(lmA.getverbosity(), 0);

    mfile::Lmfile lmB (testFileA, 1);
    ASSERT_EQ(lmB.getverbosity(), 1);

    lmB.setverbosity(3);
    ASSERT_EQ(lmB.getverbosity(), 3);
}


TEST(lmfile, jumpbehindfileheader_2Lines)
{
	mfile::Lmfile limo(testFileA, 0);
    limo.jumpbehindfileheader();
    ASSERT_EQ(limo.getposition(), 50);
}

TEST(lmfile, jumpbehindfileheader_5Lines)
{
	mfile::Lmfile limo(testFileB, 0);
    limo.jumpbehindfileheader();
    ASSERT_EQ(limo.getposition(), 129);
}


TEST(lmfile, getbytestillEOF)
{
	mfile::Lmfile limo(testFileA, 0);

    mfile::filesize_t fs = limo.getfilesize();

    ASSERT_EQ(limo.getbytestillEOF(), fs);
    limo.jumpbehindfileheader();
    ASSERT_EQ(limo.getbytestillEOF(), fs-50);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}
