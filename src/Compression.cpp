#include "Compression.h"


///
/// Decompresses the bytes from the array [current => last] and stores
/// the decoded bytes in the array target. Does no error or index checking.
/// @param: current is the pointer to the first byte of the array.
/// @param: last is the pointer to the last byte of the array.
/// @param: target is the pointer to where this function will write.
/// @return: returns the count of bytes written to the target array.
size_t Compression::DecompressRLE(const uint8_t* current, const uint8_t* const last,
                                  uint8_t* const target)
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
            target[targetIdx++] = idxValue;
        }
    }
    return targetIdx;
}
