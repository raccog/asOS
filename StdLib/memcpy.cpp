#include "StdLib/StdLib.h"

uint8_t *memcpy(uint8_t *dest, const uint8_t *src, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        *(dest + i) = *(src + i);
    }

    return dest;
}
