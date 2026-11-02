// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
/* libgcc frame.c */
unsigned char *decode_uleb128(unsigned char *buf, unsigned int *r)
{
    unsigned int shift = 0, byte, result;
    byte = *buf++;
    result = byte & 0x7f;
    while (byte & 0x80) {
        byte = *buf++;
        shift += 7;
        result |= (byte & 0x7f) << shift;
    }
    *r = result;
    return buf;
}
