#include <gtest/gtest.h>
//#include <cmath>
//#include <cstdint>
//#include <string>     // std::string, std::stoull

#include "../src/Lmfile.h"

const std::string testFileA = "39events.mdat";
const std::string testFileB = "5linesheader.mdat";

TEST(lmfile, getfilesize)
{
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileA, 0);
    ASSERT_EQ(limo->getfilesize(), 308);
    delete limo;
}

TEST(lmfile, set_read_verbosity)
{
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileA, 0);
    ASSERT_EQ(limo->getverbosity(), 0);
    delete limo;

    limo = new mfile::Lmfile(testFileA, 1);
    ASSERT_EQ(limo->getverbosity(), 1);

    limo->setverbosity(3);
    ASSERT_EQ(limo->getverbosity(), 3);
    delete limo;
}


TEST(lmfile, jumpbehindfileheader_2Lines)
{
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileA, 0);
    limo->jumpbehindfileheader();
    ASSERT_EQ(limo->getposition(), 50);
    delete limo;
}

TEST(lmfile, jumpbehindfileheader_5Lines)
{
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileB, 0);
    limo->jumpbehindfileheader();
    ASSERT_EQ(limo->getposition(), 129);
    delete limo;
}


TEST(lmfile, getbytestillEOF)
{
	mfile::Lmfile *limo;
    limo = new mfile::Lmfile(testFileA, 0);

    mfile::filesize_t fs = limo->getfilesize();

    ASSERT_EQ(limo->getbytestillEOF(), fs);
    limo->jumpbehindfileheader();
    ASSERT_EQ(limo->getbytestillEOF(), fs-50);
    delete limo;
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}

