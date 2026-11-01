struct S { char pad[0x40]; int f; char pad2[0x13C]; int a, b, c; };
static inline int FIsPtr(int f) { return !(f & 1); }
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x228B48
 * Purpose: InitSo; defaults (1, -10, ...)
 * Info: proposed name InitWater (T1); class WATER slot7; TU water
 * Notes: names_v2 C proposed UpdateSoXfWorldHierarchy__FP2SO (0.200)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/engine/physics-collision.md#6-source-files, docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
void func_00228B48(struct S *p)
{
    int f;
    func_001F2108(p);
    f = p->f;
    p->a = 1;
    p->b = -10;
    p->c = 0x2000;
    if (FIsPtr(f)) *(int *)f |= 2;
    else p->f = f | 2;
}
