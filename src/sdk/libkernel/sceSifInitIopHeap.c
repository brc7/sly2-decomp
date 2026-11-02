// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p[0x24]; int server; } RpcCD;
extern int D_002C4A94;
extern RpcCD D_0044BE80;
int func_002370A8(RpcCD *, unsigned int, int);
int sceSifInitIopHeap(void)
{
    int i;
    for (;;) {
        if (func_002370A8(&D_0044BE80, 0x80000003, 0) < 0)
            return -1;
        if (D_0044BE80.server != 0)
            break;
        i = 0x100000;
        while (i--)
            ;
    }
    D_002C4A94 = 0;
    __asm__ __volatile__(""); /* emulates Sony scheduler: keep the store before the return-value set */
    return 0;
}
