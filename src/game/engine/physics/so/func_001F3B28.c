typedef unsigned long ulong;

struct P {
    char pad[0x40];
    int v40;
    char pad2[0x148 - 0x44];
    ulong fl;
};

int func_001A61A0(struct P *);
void func_001D6378(struct P *);
void func_001D6310(struct P *);

/** @sly2-annot addr=0x1F3B28
 * Purpose: SO vt21: set/clear grf bit 8; when enabling and +0x148 bit35 and in world -> register (0x1D6378);
 *   disabling -> unregister
 * Info: proposed name SetSoFlag8 (T2); class SO slot21; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3B28(struct P *p, int enabled)
{
    register int word __asm__("a2") = p->v40;
    register int flags __asm__("v1") = word;
    register int firstTest __asm__("v0");

    __asm__("xori %0,%1,1\n\tandi %0,%0,1"
            : "=r"(firstTest) : "r"(word));
    if (firstTest)
        __asm__("lw %0,0(%0)" : "+r"(flags) : "m"(*(int *)flags));
    {
        register int has8 __asm__("v0") = flags & 8;
        if (enabled) {
            if (has8)
                return;
            {
                register int tagged __asm__("v1") = word;
                register int indirect __asm__("v0");
                __asm__("xori %0,%1,1\n\tandi %0,%0,1"
                        : "=r"(indirect) : "r"(tagged));
                if (indirect)
                    *(int *)tagged |= 8;
                else {
                    register int value __asm__("v0") = word | 8;
                    __asm__("sw %1,%0" : "=m"(p->v40) : "r"(value));
                }
            }
            if ((p->fl & (1L << 35)) && func_001A61A0(p))
                func_001D6378(p);
        } else {
            if (!has8)
                return;
            {
                register int *destination __asm__("a0") = (int *)word;
                register int mask __asm__("v1") = ~8;
                if ((word ^ 1) & 1) {
                    register int value __asm__("v0");
                    __asm__("lw %0,0(%1)" : "=r"(value)
                            : "r"(destination), "m"(*destination));
                    *destination = value & mask;
                } else {
                    register int value __asm__("v0") = word & ~8;
                    __asm__("sw %1,%0" : "=m"(p->v40) : "r"(value));
                }
            }
            func_001D6310(p);
        }
    }
}
