// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
unsigned int func_0023AE70(int cid, int mode, void *pkt, int pktsize, void *src, void *dest, int size);
unsigned int sceSifSendCmd(int cid, void *pkt, int pktsize, void *src, void *dest, int size)
{
    int mode = 0;
    return func_0023AE70(cid, mode, pkt, pktsize, src, dest, size);
}
