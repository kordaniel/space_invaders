#include "Size.h"

Size::Size(int32_t width, int32_t height):
    width(width),
    height(height)
{
    //
}

int32_t Size::getTotalSize(void) const
{
    return width * height;
}
