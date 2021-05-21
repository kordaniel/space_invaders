#ifndef SIZE_H
#define SIZE_H

#include "global.h"

class Size
{
public:
    Size(int32_t, int32_t);
    Size(const Size& other) = delete;
    int32_t GetWidth(void) const;
    int32_t GetHeight(void) const;
    int32_t GetTotalSize(void) const;

protected:
    int32_t m_width;
    int32_t m_height;
};

#endif
