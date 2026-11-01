typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[3]; ulong fl; int pad2[2]; struct M m; char pad3[0xAC - 0x60]; int nAC; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F3900
 * Purpose: Copy each visible part's XF world matrix (+0x40) and stamp (+0xA4) into the part
 * Info: proposed name UpdateSoPartMatrices (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3900(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) {
            struct O *po = pe->po;
            pe->m = po->m;
            pe->nAC = po->nA4;
        }
    }
}
