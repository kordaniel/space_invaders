#ifndef SIZE_H
#define SIZE_H

#include "global.h"

class Size
{
    public:
        int32_t width;
        int32_t height;
        Size(int32_t, int32_t);
        Size(const Size& other) = delete;
        int32_t getTotalSize(void) const;
};

#endif
