#include "common.h"

void memset(void *dest, int val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}