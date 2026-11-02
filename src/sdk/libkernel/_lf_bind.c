// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int pad[9]; void *serve; } ClientData;
typedef struct { char c[4]; } B4;
extern int D_002C4A98;
extern ClientData D_0044C240;
extern B4 D_0044C040;
extern B4 D_0044C268;
int sceSifBindRpc(ClientData *, unsigned int, unsigned int);
int sceSifCallRpc(ClientData *, int, unsigned int, void *, int, void *, int, void *, void *);
int _lf_bind(void)
{
    int i;
    if (D_002C4A98 < 0) {
    again:
        if (sceSifBindRpc(&D_0044C240, 0x80000006, 0) < 0)
            return -1;
        if (D_0044C240.serve != 0) {
            D_002C4A98 = 0;
            if (sceSifCallRpc(&D_0044C240, 0xFF, 0, 0, 0, &D_0044C040, 4, 0, 0) < 0)
                return 0xFFFEFFFF;
            D_0044C268 = D_0044C040;
            return 0;
        }
        i = 0x100000;
        do __asm__ volatile(""); while (i-- != 0);
        goto again;
    }
    return 0;
}
