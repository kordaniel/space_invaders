#include "Compression.h"

#include "Io.h"

///
/// Decompresses the bytes from the array [current => last] and stores
/// the decoded bytes in the array targetBuff. Does no error or index checking.
/// @param: current is the pointer to the first byte of the array.
/// @param: last is the pointer to the last byte of the array.
/// @param: targetBuff is the pointer to where this function will write.
/// @return: returns the count of bytes written to the targetBuff array.
size_t Compression::DecompressRLE(const uint8_t* current, const uint8_t* const last,
                                  uint8_t* const targetBuff)
// static classmethod
{
    assertpair(current <= last, (size_t) current, (size_t) last);
    uint8_t rleEncodedData, idxValue;
    size_t targetIdx = 0;
    for (; current != last; ++current) {
        rleEncodedData = *current;
        idxValue  = (rleEncodedData >> 7); // Extract the current value: 0 or 1.
        rleEncodedData &= (uint8_t) 0x7F;  // Extract the repeat count of the value.
        for (uint8_t i = 0; i < rleEncodedData; ++i) {
            targetBuff[targetIdx++] = idxValue;
        }
    }
    return targetIdx;
}

///
/// Is the reverse operation of the DecompressRLE function.
/// @param: current the pointer to the first byte of the array to be compressed.
/// @param: last is the pointer to the last byte of the array.
/// @param: targetBuff is the vector used to store the compressed bytes.
void Compression::CompressRLE(const uint8_t* current, const uint8_t* const last,
                              std::vector<uint8_t>& targetBuff)
// static classmethod
{
    const uint8_t* lastElem = last - 1;
    uint8_t buff = *current; ++current; // TODO: *current; ++current; <=??=> *current++ ? Should be, make sure
    uint8_t count = 1;

    for (; current != last; ++current) {
        assert(*current < 2);
        if (*current > 1) {
            io::print_to_stderr_varargs("[RLE-compress]: Got an array with the val: ", (int) *current);
        }

        if (count == 127 || *current != buff) {
            // 127 => waste 1/128 of the space to keep things simple (no real need for 0)
            buff <<= 7;
            buff  |= count;
            targetBuff.push_back(buff);
            buff = *current;
            count = 1;
        } else {
            ++count;
        }

        if (current == lastElem) {
            buff <<= 7;
            buff |= count;
            targetBuff.push_back(buff);
        }
    }
}

///
/// Compresses the data required to build one Sprite. The structure of the compressed bytestreams is:
/// 3 fields of type Tint (currently using uint16_t) for all three: width, height and spritescount
/// The rest of the bytes is simply a "bitmap", that is every 0/1 value of the arr is mapped sequentially
/// to the bits of the bytes.
/// @param: arr the array holding the uncompressed values
/// @param: width, height and spritesCount the member variables of class Sprite
/// @param: targetBuff the vector used as the output buffer
/// @return: void
template<typename Tint>
void Compression::CompressSpriteData(const uint8_t* arr, Tint width, Tint height, Tint spritesCount, std::vector<uint8_t>& targetBuff)
// static classmethod
{
    auto pushBytesOfValue = [&targetBuff](Tint value) -> void {
        const size_t bytes = sizeof(value);
        for (size_t i = 0; i < bytes; ++i) {
            targetBuff.push_back((uint8_t) value >> (i * 8));
        }
    };
   pushBytesOfValue(width);
   pushBytesOfValue(height);
   pushBytesOfValue(spritesCount);

    const size_t length = spritesCount * width * height;
    size_t count = 0;
    uint8_t buff = 0;

    while (true) {
        buff |= *arr << (count++ % 8);
        ++arr;

        if (count == length) {
            targetBuff.push_back(buff);
            break;
        } else if (count % 8 == 0) {
            targetBuff.push_back(buff);
            buff = 0;
        }
    }
}
template void Compression::CompressSpriteData(const uint8_t* arr, uint16_t width, uint16_t height, uint16_t spritesCount, std::vector<uint8_t>& targetBuff);

///
/// Decompresses the compressed Bitmaps. First 6 bytes are the uncompressed bits for the Sprite-class member
/// variables. The product of these 3 variables is the number of bytes extracted.
/// @param: compressed vector holding the bitmap bytes
/// @param: decompressed the vector used as the output buffer
/// @return: void
void Compression::DecompressSpriteData(const std::vector<uint8_t>& compressed, std::vector<uint8_t>& decompressed)
// static classmethod
{
    auto constructUint16_tFromUint8_t = [&compressed](size_t i) -> uint16_t {
        return (uint16_t) compressed[i] | (uint16_t) compressed[i+1] << 8;
    };
    uint16_t width        = constructUint16_tFromUint8_t(0);
    uint16_t height       = constructUint16_tFromUint8_t(2);
    uint16_t spritesCount = constructUint16_tFromUint8_t(4);
    const size_t length   = spritesCount * width * height;

    for (size_t i = 0; i < 6; ++i) {
        decompressed.push_back(compressed[i]);
    }

    size_t offset = 6;
    size_t i = 0;
    while (i < length) {
        decompressed.push_back((compressed[offset] >> (i % 8)) & 1);
        ++i;
        if (i % 8 == 0) {
            ++offset;
        }
    }
}
