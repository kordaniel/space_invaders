#include "Size.h"

Size::Size(int32_t width, int32_t height):
    m_width(width),
    m_height(height)
{
    //
}

int32_t Size::GetWidth(void)     const { return m_width; }
int32_t Size::GetHeight(void)    const { return m_height; }
int32_t Size::GetTotalSize(void) const { return GetWidth() * GetHeight(); }
