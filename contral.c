#include "contral.h"

int clz_bin(uint32_t x)
{
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8; x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4; x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2; x <<= 2; }
    if (x <= 0x7FFFFFFF) { n += 1; x <<= 1; }
    return n;
}


static uint32_t offset[] = { 16, 8, 4, 2, 1 };
static uint32_t mask[] = { 0xFFFF, 0x00FF, 0x000F, 0x0003, 0x0001 };

static int clz_rec(uint32_t x, int i)
{
    /* shift upper half down, rest is filled up with 0s */
    uint16_t upper = (x >> offset[i]);
	// mask upper half away
    uint16_t lower = (x & mask[i]);

    if (i == 4) return !upper;

    return upper ? clz_recupper, i + 1) : offset[i] + clz_rec(lower, i + 1);
}

int clz(uint32_t x)
{
    if (!x) return 32;

    clz_rec(x, 0);
}
