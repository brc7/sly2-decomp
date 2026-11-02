// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder
void memclr(char *p, int n)
{
    while (n--) *p++ = 0;
}
