typedef int qword __attribute__((mode(TI)));
struct S;
typedef struct { short d; short pad; int (*pfn)(); } VT;
struct S { int x0, x4, x8; VT *vt; char pad[0x150 - 0x10]; int k; int x154; int x158; char pad2[0x16C - 0x15C]; struct S *pt; float r; char pad3[0x1A0 - 0x174]; int x1A0; };
void func_001F36E0(struct S *p);
void func_00224258(struct S *p, int a);
qword func_001A62E8(struct S *p);
/** @sly2-annot addr=0x2240B0
 * Purpose: CID100 (Bentley gadget object) update: SO update; state 1 with link -> stick (0x224258); if the
 *   owner +0x16C is farther than +0x170 (3000) -> vt75 -> state 3 (auto-fire)
 * Info: proposed name UpdateCid100 (T2); class CID100 slot16; TU tu_223F70
 * Notes: runtime5a #18 CLARIFIED: distance owner(+0x16C) > +0x170 (3000) -> vt75 -> state 3 (auto-fire) | T2:
 *   placeholder token Cid100
 * Confidence: low (draft) [T2 0.55] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_002240B0(struct S *p)
{
    func_001F36E0(p);
    if (p->k == 1 && p->x158 != -1)
        func_00224258(p, p->x1A0);
    if (p->pt) {
        qword a = func_001A62E8(p);
        qword b = func_001A62E8(p->pt);
        qword d, t, l;
        float g;
        int f;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(t) : "0"(d));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
        f = p->r < g;
        if (f)
            p->vt[75].pfn((struct S *)((char *)p + p->vt[75].d), 0);
    }
}
