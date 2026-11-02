struct E { int bytes; unsigned int source; int argumentBytes; const char *arguments; int unknown10; };
struct DMA { unsigned int source; unsigned int destination; int bytes; int attributes; };
extern struct E *D_004E5D80;
extern struct E *D_004E5E38;
extern int D_002E2058;
extern char D_002A6A48[];
void func_00236A60(int n);
int func_00238F00(void);
int func_002420F0(int n);
int func_0014B9C0(void);
void func_0014BA00(void);
int func_00237E70(void);
void func_0014BBB0(char *p, char *s, int n);
int func_00239340(const char *p);
int func_002392F0(void);
int func_002425F0(int n);
int func_0014BD30(int n);
int func_00238BD0(void);
void *func_00238C58(int cb);
void func_00234CA0(int n);
int func_00234DE0(struct DMA *p, int n);
int func_00239140(void *h, int argumentBytes, const char *arguments);
int func_00238D40(void *h);
static inline int CopyLimit(int value)
{
    int copy;
    __asm__("daddu %0,%1,$zero" : "=r"(copy) : "r"(value));
    return copy;
}
/** @sly2-annot addr=0x1A7630
 * Purpose: IOP bring-up: sceSifInitRpc, reboot IOP with ioprp300.img unless D_002E2058, sceCdInit/SetCdMmode,
 *   sceFsReset, alloc IOP heap and sceSifLoadModuleBuffer for each embedded IRX in table
 *   D_004E5D80..D_004E5E38
 * Info: proposed name StartupSif (T1); TU main
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/main-loop-startup.md#1-maincpp-0x1a7518-0x1a7c18,
 *   docs/engine/main-loop-startup.md#8-key-functions, docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void func_001A7630(void)
{
    union { char buf[0x100]; struct DMA dma; } u;
    struct E *pe;
    int cbMax;
    void *h;
    func_00236A60(0);
    func_00238F00();
    func_002420F0(0);
    if (!func_0014B9C0())
        while (1)
            ;
    func_0014BA00();
    func_00237E70();
    if (!D_002E2058) {
        func_0014BBB0(u.buf, D_002A6A48, 1);
        while (!func_00239340(u.buf))
            ;
        while (!func_002392F0())
            ;
        func_00236A60(0);
        func_00238F00();
        func_002420F0(0);
        if (!func_0014B9C0())
            while (1)
                ;
        func_0014BA00();
        func_00237E70();
    }
    func_002425F0(0);
    (void)func_0014BD30(1);
    func_00238BD0();
    cbMax = 0;
    {
        struct E *p;
        for (p = D_004E5D80; p < D_004E5E38; p++)
            if (p->bytes >= cbMax)
                cbMax = p->bytes;
    }
    h = func_00238C58(cbMax);
    for (pe = D_004E5D80; pe < D_004E5E38; pe++) {
        int cb = pe->bytes;
        int off = 0;
        while (cb) {
            register int limit __asm__("$2") = 0xF0000;
            int c;
            const int zeroOperation = 0, limitLow = 0xFFC0;
            register unsigned int destination __asm__("$5");
            register int operation __asm__("$4");
            __asm__("ori %0,%0,%1" : "+r"(limit) : "i"(limitLow));
            __asm__("addu %0,%1,%2" : "=r"(destination) : "r"(h), "r"(off));
            c = CopyLimit(limit);
            __asm__("slt %0,%1,%0" : "+r"(limit) : "r"(cb));
            __asm__("movn %0,%1,%2" : "+r"(c) : "r"(cb), "r"(limit));
            u.dma.source = pe->source + off;
            __asm__("daddu %0,%z3,$zero\n\tsw %2,%1"
                    : "=r"(operation), "=m"(u.dma.destination)
                    : "r"(destination), "rJ"(zeroOperation));
            u.dma.bytes = c;
            u.dma.attributes = 0;
            func_00234CA0(operation);
            while (!func_00234DE0(&u.dma, 1))
                ;
            cb -= c;
            off += c;
        }
        while (func_00239140(h, pe->argumentBytes, pe->arguments) < 0)
            ;
    }
    func_00238D40(h);
}

typedef char offset_0_bytes[(unsigned)&((struct E*)0)->bytes==0?1:-1];
typedef char offset_0_source[(unsigned)&((struct E*)0)->source==4?1:-1];
typedef char offset_0_argumentBytes[(unsigned)&((struct E*)0)->argumentBytes==8?1:-1];
typedef char offset_0_arguments[(unsigned)&((struct E*)0)->arguments==12?1:-1];
typedef char offset_0_unknown10[(unsigned)&((struct E*)0)->unknown10==16?1:-1];
typedef char offset_1_source[(unsigned)&((struct DMA*)0)->source==0?1:-1];
typedef char offset_1_destination[(unsigned)&((struct DMA*)0)->destination==4?1:-1];
typedef char offset_1_bytes[(unsigned)&((struct DMA*)0)->bytes==8?1:-1];
typedef char offset_1_attributes[(unsigned)&((struct DMA*)0)->attributes==12?1:-1];
typedef char size_0[sizeof(struct E)==20?1:-1];
typedef char size_1[sizeof(struct DMA)==16?1:-1];
typedef char size_2[sizeof(int)==4?1:-1];
typedef char size_3[sizeof(unsigned int)==4?1:-1];
typedef char size_4[sizeof(void*)==4?1:-1];
typedef char size_5[sizeof(char)==1?1:-1];
typedef char alignment_0[__alignof__(struct E)==4?1:-1];
typedef char alignment_1[__alignof__(struct DMA)==4?1:-1];
