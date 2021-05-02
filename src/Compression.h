#ifndef COMPRESSION_H
#define COMPRESSION_H


#include "global.h"

///
/// Class to handle compressed data.
/// Contains only static methods.
/// Currently provides only RLE-decompression where RLE is defined as
/// bytes of uint8_t type. The leftmost bit represents the value 0/1
/// and the 7 rightmost bits is the repeat count for the value.
class Compression
{
public:
    Compression(const Compression& other) = delete;
    static size_t DecompressRLE(const uint8_t* current, const uint8_t* const last, uint8_t* const target);

private:
    Compression(void); // Should be used as a static class => private constructor.
};

#endif
