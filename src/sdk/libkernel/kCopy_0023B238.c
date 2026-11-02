// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder
int kCopy_0023B238(unsigned int *dst, unsigned int *src, unsigned int n)
{
    unsigned int i;
    n >>= 2;
    for (i = 0; i < n; i++)
        *dst++ = *src++;
    return 0;
}
