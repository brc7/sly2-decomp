// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int WaitDma(void *, int);
int sceDmaSync(void *ch, int mode, int timeout)
{
    if (mode != 1) {
        WaitDma(ch, mode);
        return 0;
    }
    return (*(unsigned int *)ch >> 8) & 1;
}
