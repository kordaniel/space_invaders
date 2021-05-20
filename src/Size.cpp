#include "Size.h"

Size::Size(int32_t width, int32_t height):
    m_width(width),
    m_height(height)
{
    //
}

int32_t Size::getTotalSize(void) const
{
    return m_width * m_height;
}
