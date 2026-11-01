typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct MSG { int cb; int nKind; };
struct O { char pad[0x60]; struct O *next; };
struct G { float mn[4]; float mx[3]; char pad[0x34 - 0x1C]; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
struct S { char pad[0x3D1C]; struct G *first; };
void *func_001AE318(struct MSG *pmsg);
void func_00133E30(struct S *p);
void func_00133E80(struct S *p);

/** @sly2-annot addr=0x20FAA0
 * Purpose: Collect up to 256 SOs whose groups overlap the AABB(min/max of two points), optional filter;
 *   result in slot-allocated array
 * Info: proposed name IntersectSwBoundingBox (T1); TU sw
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/game-progression.md#8-key-functions, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_0020FAA0(struct S *p, qword a, qword b, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    U mn, mx;
    struct MSG msg;
    struct O **ap;
    struct G *g;
    int c;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(mn.q) : "j"(a), "j"(b)); __asm__("vmax.xyz %0, %1, %2" : "=j"(mx.q) : "j"(a), "j"(b));
    {
        struct MSG *pm = &msg;
        pm->cb = 0x400;
        pm->nKind = 0x10;
    }
    c = 0;
    ap = func_001AE318(&msg);
    func_00133E30(p);
    func_00133E80(p);
    for (g = p->first; g; g = g->next) {
        struct O *o;
        if (g->mn[0] > mx.f[0] || g->mx[0] < mn.f[0] ||
            g->mn[1] > mx.f[1] || g->mx[1] < mn.f[1] ||
            g->mn[2] > mx.f[2] || g->mx[2] < mn.f[2])
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
