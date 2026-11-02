// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef struct { u_long NLOOP:15, EOP:1, pad16:16, id:14, PRE:1, PRIM:11, FLG:2, NREG:4; u_long REGS; } sceGifTag;
int func_00236960(const char *fmt, ...);
extern char D_002AB2F0[];
#define D2_CHCR ((volatile u_int *)0x1000A000)
#define D2_MADR ((volatile u_int *)0x1000A010)
#define D2_QWC  ((volatile u_int *)0x1000A020)
int sceGsPutDrawEnv(sceGifTag *giftag)
{
    u_int cnt = 0;
    if (*D2_CHCR & 0x100) {
        do {
            if (cnt++ > 0x1000000) {
                func_00236960(D_002AB2F0) /* "sceGsPutDrawEnv: DMA Ch.2 does not terminate" */;
                return -1;
            }
        } while (*D2_CHCR & 0x100);
    }
    *D2_QWC = giftag->NLOOP + 1;
    if (((u_int)giftag & 0x70000000) == 0x70000000)
        *D2_MADR = ((u_int)giftag & 0x0FFFFFFF) | 0x80000000;
    else
        *D2_MADR = (u_int)giftag & 0x0FFFFFFF;
    *D2_CHCR = 0x101;
    return 0;
}
