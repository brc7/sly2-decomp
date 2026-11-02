// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x828]; int x828; unsigned int x82C; char p1[0x850 - 0x830]; int load_iq; int load_niq;
} MpegD;
extern char D_002AC728[];
extern char D_002AC750[];
int func_00252148(MpegD *d, int n);
void _waitIpuIdle_254488(MpegD *d);
void func_0024FA38(MpegD *d, const char *msg);
void _quantMatrixExtension(MpegD *d)
{
    if ((d->load_iq = func_00252148(d, 1)) != 0) {
        _waitIpuIdle_254488(d);
        *(volatile unsigned int *)0x10002000 = 0x50000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle_254488(d);
    }
    if ((d->load_niq = func_00252148(d, 1)) != 0) {
        _waitIpuIdle_254488(d);
        *(volatile unsigned int *)0x10002000 = 0x58000000;
        d->x82C = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle_254488(d);
    }
    if (func_00252148(d, 1)) func_0024FA38(d, D_002AC728);
    if (func_00252148(d, 1)) func_0024FA38(d, D_002AC750);
}
