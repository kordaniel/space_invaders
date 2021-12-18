#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "global.h"

#include <vector>

///
/// Class to handle compressed data.
/// Contains only static methods.
///
/// Provides RLE-compression and RLE-decompression where RLE is defined as
/// bytes of uint8_t type. The leftmost bit represents the value 0/1
/// and the 7 rightmost bits is the repeat count for the value, starting
/// from 0.
///
/// Also provides functions to (de)serialize Sprite-objects mapping the data
/// into bitmaps (bitarrays) using one bit for every value.
class Compression
{
public:
    Compression(const Compression& other) = delete;
    void operator=(const Compression&) = delete;

    static size_t DecompressRLE(const uint8_t* current, const uint8_t* const last, uint8_t* const targetBuff);
    static void CompressRLE(const uint8_t* current, const uint8_t* const last, std::vector<uint8_t>& targetBuff);

    template<typename Tint>
    static void CompressSpriteData(const uint8_t* arr, Tint width, Tint height, Tint spritesCount, std::vector<uint8_t>& targetBuff);
    static void DecompressSpriteData(const std::vector<uint8_t>& compressed, std::vector<uint8_t>& decompressed);

private:
    Compression(void); // Should be used as a static class => private constructor.
};

#endif
