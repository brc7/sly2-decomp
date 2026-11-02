// func_001A87F0: for each of `count` vertices of mesh A's point table, pick the best-ranked contact plane from the vertex's
// surface list (heads[i], NODEs chained through ->next), then emit contacts through func_001A7C18:
//   - kind 1: the vertex itself (position = points.data[indices[i]]) with the best plane's normal and the vertex's own plane distance;
//   - kind 2: every other list vertex of the same chain that lies closer to the best plane than the vertex does.
// A surface is ranked by the smallest signed distance (dot(normal.xyz, p.xyz) - normal.w) from its plane to the vertex (orig) and to
// the far end of every edge in the chain (min); the surface with the largest `min` above D_002A6AD0 wins, and bestOrig is its `orig`.
// SURFACE.flags bit 8 marks surfaces already ranked (set while scanning, cleared again before the contacts are emitted).
//
// How the match was found (w2lane8 v4 + wave-3 lane 4; all natural C++, VU asm only):
//  - SCALAR/VECTOR have default constructors that call func_002564B8 / func_002564C0 (the engine's 16-byte vector initialisers); PlaneDist's two SCALAR
//    locals therefore become the ctor calls + the dead dot/one stores at sp+0x10.. of every inlined expansion.
//  - `float d` and `NODE *m` live at function scope and are shared by the edge loop and the emit loop: cse1 canonicalises the `if (n)` loop guard to the
//    lowest-numbered pseudo of its equivalence class, so loops over `n` (declared after `head`) test `head` and loops over `m` test `m`.
//  - `pt` is read before `nn` is constructed, so it is live across the ctor call (callee-saved GPR) which also decides the s0/s1 split between n and m.
//  - `plane` is read before the `flags & 8` test, which lets reorg fill the bnez delay slot from before.
//  - func_001A7C18 takes its float in the middle of the argument list (EABI: float args go to $f12 either way); the position only matters for the order gcc
//    evaluates the arguments in.
//  - PlaneDist takes the plane as a VU_VECTOR4 by value: the by-value class parameter adds a few TImode copies that survive cse1 (and vanish at
//    register allocation), which makes gcse's expression table 77 buckets instead of 73. PRE numbers its reaching registers in hash-bucket order, so
//    &normal (sp+0x50) now gets its pseudo before i+1, and reload gives them the spill slots 0xD0 / 0xD4 as in the original.
typedef int qword __attribute__((mode(TI)));
struct SCALAR;
struct VECTOR;
extern "C" SCALAR *func_002564B8(SCALAR *);
extern "C" VECTOR *func_002564C0(VECTOR *);
struct SCALAR { qword q; SCALAR() { (void)func_002564B8(this); } };
struct VECTOR { qword q; VECTOR() { (void)func_002564C0(this); } };
struct VU_VECTOR4 { qword q; VU_VECTOR4(qword a) { q = a; } };
struct W;
struct B;
struct XP;
struct POINTS { int count; qword *data; };
struct A { char pad[0x90]; POINTS points; };
struct SURFACE { qword plane; char pad10[8]; int flags; };
struct EDGE { unsigned short ends[2]; };
struct NODE { char pad[0x10]; qword point; char pad20[0x50 - 0x20]; SURFACE *surface; EDGE *edge; int reverse; NODE *next; };
extern "C" {
extern float D_002A6AD0;
void func_001A7C18(W *, A *, B *, qword, float, qword, int, int, int, XP **);
}

/* signed distance of pos from plane: dot(plane.xyz, pos.xyz) - plane.w, via VU0 macro mode */
static inline float PlaneDist(VU_VECTOR4 plane, qword pos)
{
    SCALAR dot, one;
    qword r, unit;
    float f;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddaz.x $ACC,%1,%0z\n\tvmsubw.x %0,%1,%2w"
            : "=j"(r), "=&j"(unit) : "j"(plane.q), "j"(pos));
    dot.q = r;
    one.q = unit;
    /* gcc supplies the qmfc2 for the "r" qword operand; the extra unused "j"(r) is allocation-only: it keeps r's preferred class VU (sqc2 store, not sq). */
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(dot.q), "j"(r));
    return f;
}

/** @sly2-annot addr=0x1A87F0
 * Purpose: Rank embedded vertex surfaces (largest min distance above D_002A6AD0) and emit vertex contacts
 *   through PxpAdd
 * Info: proposed name AddSoEmbedXps (T2); TU mark
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
extern "C" void func_001A87F0(W *w, A *a, B *b, int count, int *indices, NODE **heads, XP **out)
{
    float d;
    NODE *m;
    for (int i = 0; i < count; i++) {
        POINTS *positions = &a->points;
        qword *data = positions->data;
        int index = indices[i];
        NODE *head = heads[i];
        SURFACE *best = 0;
        float bestMin = D_002A6AD0;
        float bestOrig;
        qword bestNormal;
        qword position = data[index];
        NODE *n;
        for (n = head; n; n = n->next) {
            SURFACE *s = n->surface;
            qword plane = s->plane;
            if (!(s->flags & 8)) {
                s->flags |= 8;
                float orig = PlaneDist(plane, position);
                float min = orig;
                for (m = head; m; m = m->next) {
                    d = PlaneDist(plane, data[m->edge->ends[1 - m->reverse]]);
                    if (d < min) min = d;
                }
                if (bestMin < min) { bestNormal = plane; best = s; bestMin = min; bestOrig = orig; }
            }
        }
        for (n = head; n; n = n->next) n->surface->flags &= ~8;
        VECTOR normal;
        normal.q = bestNormal;
        func_001A7C18(w, a, b, position, bestOrig, normal.q, 1, (int)&a->points.data[index], (int)best, out);
        for (m = head; m; m = m->next) {
            d = PlaneDist(bestNormal, m->point);
            if (d < bestOrig) {
                qword pt = m->point;
                VECTOR nn;
                nn.q = bestNormal;
                func_001A7C18(w, a, b, pt, d, nn.q, 2, (int)m->edge, (int)best, out);
            }
        }
    }
}
