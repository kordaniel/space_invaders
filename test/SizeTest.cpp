#include "gtest/gtest.h"
#include "Size.hpp"
 
TEST(SizeTests, Width)
{
    double width = 8;
    double height = 6;
    Size sizeobj(width, height);
    double result = sizeobj.GetWidth();
    EXPECT_EQ(width, result);
}

TEST(SizeTests, Height)
{
    double width = 8;
    double height = 6;
    Size sizeobj(width, height);
    double result = sizeobj.GetHeight();
    EXPECT_EQ(height, result);
}

TEST(SizeTests, TotalSize)
{
    double width = 8;
    double height = 6;
    Size sizeobj(width, height);
    double result = sizeobj.GetTotalSize();
    EXPECT_EQ(width * height, result);
}
