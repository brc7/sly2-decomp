// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    unsigned int d4madr, d4tadr, d4qwc, d4chcr;
    unsigned int d3madr, d3qwc, d3chcr;
    unsigned int ipubp, ipuctrl;
} sceIpuDmaEnv;
void setD3_CHCR(unsigned int v);
void setD4_CHCR(unsigned int v);
#define R(a) (*(volatile unsigned int *)(a))
void sceIpuStopDMA(sceIpuDmaEnv *env)
{
    setD4_CHCR(1);
    env->d4madr = R(0x1000B410);
    env->d4tadr = R(0x1000B430);
    env->d4qwc = R(0x1000B420);
    env->d4chcr = R(0x1000B400);
    while (R(0x10002010) & 0xF0)
        ;
    setD3_CHCR(0);
    env->d3madr = R(0x1000B010);
    env->d3qwc = R(0x1000B020);
    env->d3chcr = R(0x1000B000);
    env->ipubp = R(0x10002020);
    env->ipuctrl = R(0x10002010);
}
