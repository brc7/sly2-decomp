// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder
int kCopy_00239898(unsigned char *dst, unsigned char *src, unsigned int n)
{
    unsigned int i;
    for (i = 0; i < n; i++)
        *dst++ = *src++;
    return 0;
}
