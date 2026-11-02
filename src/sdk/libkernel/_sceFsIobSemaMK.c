// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 3/28, SemaParam store order (all 6 orders tried, void* option too)
struct SemaParam { int currentCount; int maxCount; int initCount; int numWaitThreads; unsigned int attr; unsigned int option; };
extern int D_002C4A88;
extern int D_002C4A8C;
extern char D_002AAA10[];
extern char D_002AAA20[];
int func_00234A20(struct SemaParam *);
void _sceFsIobSemaMK(void)
{
    struct SemaParam p;
    if (D_002C4A88 == -1) {
        p.option = (unsigned int)D_002AAA10;
        p.initCount = 1;
        p.maxCount = 1;
        D_002C4A88 = func_00234A20(&p);
        p.option = (unsigned int)D_002AAA20;
        D_002C4A8C = func_00234A20(&p);
    }
}
