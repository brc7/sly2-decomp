struct LO { char pad[0x40]; int x40; char pad2[0x80 - 0x44]; char x80[0x4C]; int xCC; };
struct G { char pad[0x3D5C]; int x3D5C; };
int CploFindSwObjectsByClass(struct G *p, int a, int b, int c, int d, struct LO **ap);
void func_001681F8(void *p);
static inline int FPtr(int v) { return !(v & 1); }
static inline int Flags(struct LO *plo)
{
    int v = plo->x40;
    if (FPtr(v)) v = *(int *)v;
    return v;
}
/** @sly2-annot addr=0x1CD9E0
 * Purpose: On illumination change: FindSwObjects(0x105,0x30) and SubmitGlobsetRelight on each lit object's
 *   globset
 * Info: proposed name SubmitSwRelightAll (T1); TU render
 * Notes: superseded: RenderSwObjectsFound (r5_1AE130)
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130+rendering
 * See: docs/engine/rendering.md#1-frame-structure,
 *   docs/reference/functions/rendering.md#render-0x1cd5f0-0x1cdcd0
 * @end-sly2-annot */
void func_001CD9E0(struct G *p)
{
    struct LO *ap[0x400];
    int n, i;
    n = CploFindSwObjectsByClass(p, 0x105, 0x30, 0, 0x400, ap);
    for (i = 0; i < n; i++) {
        struct LO *plo = ap[i];
        if ((Flags(plo) & 8) && plo->xCC != p->x3D5C) func_001681F8(plo->x80);
    }
}
