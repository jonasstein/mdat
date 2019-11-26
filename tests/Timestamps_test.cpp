#include <gtest/gtest.h>
#include "../src/Timestamps.h"

TEST(Timestamps, milliseconds)
{
	TimestampClass timestamp_ns { 999999848 };
	ASSERT_EQ(integermilliseconds(timestamp_ns), 1000);
}
TEST(Timestamps, milliseconds0)
{
	ASSERT_EQ(integermilliseconds(0), 0);
}

TEST(Timestamps, milliseconds1000)
{
	ASSERT_EQ(integermilliseconds(1000000001), 1000);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}
