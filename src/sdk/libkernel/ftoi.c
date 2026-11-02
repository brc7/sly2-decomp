// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
int ftoi(unsigned long d)
{
    long e;
    unsigned long m;
    m = d;
    e = (m << 1) >> 53; e -= 0x433;
    if (e < -53) return 0;
    if (e >= 13) return 9999;
    m = (m << 12) >> 12; m |= 1UL << 52;
    if (e < 0) {
        e = -e;
        m >>= (int)(e - 2);
        if ((m & 3) == 3) m = (m >> 2) + 1;
        else m >>= 2;
    } else {
        m <<= (int)e;
    }
    return (long)(m << 32) >> 32;
}

