// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { void (*func)(void *, void *); void *arg; void *gp; int pad; } CmdData;
extern CmdData D_0044E708[];
extern CmdData *D_002C5A50;
extern int D_002C5A54;
static inline void *SetGP(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(old) : "r"(gp) : "memory");
    return old;
}
static inline void *GetGP(void)
{
    void *gp;
    __asm__ volatile("move %0, $gp" : "=r"(gp));
    return gp;
}
void *sceSifExecNotifyHandler(int sys, void *data)
{
    CmdData *p;
    int n;
    void *gp;
    if (sys) {
        n = 0x20;
        p = D_0044E708;
    } else {
        p = D_002C5A50;
        n = D_002C5A54;
    }
    gp = GetGP();
    if (n > 0) {
        CmdData *q = p;
        int i = n;
        do {
            if (q->func) {
                SetGP(q->gp);
                q->func(data, q->arg);
            }
            i--;
            q++;
        } while (i != 0);
    }
    return SetGP(gp);
}
