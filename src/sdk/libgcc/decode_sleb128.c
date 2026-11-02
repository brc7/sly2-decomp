// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
unsigned char *decode_sleb128(unsigned char *buf, int *r)
{
    unsigned int shift = 0;
    unsigned int result = 0;
    unsigned char byte;
    while (1) {
        byte = *buf++;
        result |= (byte & 0x7f) << shift;
        shift += 7;
        if ((byte & 0x80) == 0) break;
    }
    if (shift < (sizeof(*r) * 8) && (byte & 0x40) != 0)
        result |= (-1) << shift;
    *r = result;
    return buf;
}
