typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct O { char pad[0x70]; qword pos; };
struct E { struct O *po; int pad; float r; int pad2; ulong fl; char pad3[0x74 - 0x18]; void *p74; char pad4[0xB0 - 0x78]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
int func_00118FB0(void *, int, qword);
/** @sly2-annot addr=0x1F72F8
 * Purpose: SO vt73: true if pos is inside any visible sphere part or part BSP (0x118FB0)
 * Info: proposed name FPosInSo (T2); class SO slot73; TU so
 * Notes: names_v2 v1 proposed ExtendSoBounds__FP2SOP6VECTORf (0.058)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F72F8(struct P *p, qword pos)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->fl & (1L << 50)) continue;
        if (pe->fl & (1L << 49)) {
            qword d, one;
            float r2, d2;
            int t;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(pe->po->pos));
            r2 = pe->r * pe->r;
            __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "0"(d));
            __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(d));
            __asm__("mtc1 %1, %0" : "=f"(d2) : "r"(t));
            { int f = d2 < r2; if (f) return 1; }
        }
        if (pe->p74 && func_00118FB0(pe->p74, 0x7FFFFFFF, pos)) return 1;
    }
    return 0;
}
