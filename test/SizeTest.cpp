#include "gtest/gtest.h"
#include "Size.hpp"
 
TEST(SizeTests, Width)
{
    int width = 8;
    int height = 6;
    Size sizeobj(width, height);
    int result = sizeobj.GetWidth();
    EXPECT_EQ(width, result);
}

TEST(SizeTests, Height)
{
    int width = 8;
    int height = 6;
    Size sizeobj(width, height);
    int result = sizeobj.GetHeight();
    EXPECT_EQ(height, result);
}

TEST(SizeTests, TotalSize)
{
    int width = 8;
    int height = 6;
    Size sizeobj(width, height);
    int result = sizeobj.GetTotalSize();
    EXPECT_EQ(width * height, result);
}
