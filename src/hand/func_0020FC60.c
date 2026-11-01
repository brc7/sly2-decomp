typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct O { char pad[0x60]; struct O *next; };
struct G { char pad[0x20]; qword pos; float r; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
struct S { char pad[0x3D1C]; struct G *first; };
void *func_001AE318(struct MSG *pmsg);
void func_00133E30(struct S *p);
void func_00133E80(struct S *p);

/** @sly2-annot addr=0x20FC60
 * Purpose: Same for a sphere (center, radius)
 * Info: proposed name IntersectSwBoundingSphere (T1); TU sw
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FC60(struct S *p, qword pos, float r, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    struct MSG msg;
    struct O **ap;
    struct G *g;
    int c;
    msg.cb = 0x400;
    msg.nKind = 0x10;
    ap = func_001AE318(&msg);
    c = 0;
    func_00133E30(p);
    func_00133E80(p);
    for (g = p->first; g; g = g->next) {
        float rr;
        qword d, e, tmp;
        float f;
        struct O *o;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(g->pos));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(e), "=&j"(tmp) : "0"(d));
        rr = r + g->r;
        rr *= rr;
        __asm__("mtc1 %1, %0" : "=f"(f) : "r"(e));
        if (rr < f)
            continue;
        for (o = g->first; o; o = o->next) {
            if (pfn && !pfn(ctx, o))
                continue;
            if (c < 0x100)
                ap[c++] = o;
        }
    }
    *pap = ap;
    *pc = c;
}
