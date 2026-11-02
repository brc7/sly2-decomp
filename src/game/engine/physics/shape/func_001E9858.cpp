/* func_001E9858: rebuild every mesh entry's sample array in path-local coordinates.
 *
 * For each mesh node and each of its entries: copy the entry's vertices to a scratch array, allocate the
 * output sample array, then for every vertex
 *   world = vertex * owner->mat                         (point transform)
 *   FindShapeClosestPoint finds the closest hit/direction/distance on the path for `world`
 *   BuildOrthonormalMatrixZ builds a basis from (direction, axis); it is transposed (= inverted)
 *   sample = (world - hit) * inverse basis, with .w = distance.
 *
 * Notes on getting the exact code (plain C++ plus the VU0 / MMI asm that the original needs):
 *  - `axis`, `next` and `world` are ordinary locals.  Nothing is saved to a struct: the compiler spills them
 *    (stack slots 0x8C0, 0x8D0, 0x8E0) and reloads them around the calls, and that spill is what gives the
 *    `lq v0,0x20(v0) ; sq v0,0x8C0(sp)` pair at function entry.
 *  - TransposeInto is split into two asm statements (interleave, then recombine): the four early-clobber
 *    temporaries are then allocated before the inputs (v0,v1,a0,a1 / inputs a2,a3,t0), as in the original.
 *  - No register pins: with the plain loop variables the allocation priorities put the node pointer in s4,
 *    the `&work.phase` pointer in s7.
 */
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct M3 { U a, b, c; };
struct M4 { qword a, b, c, d; };
struct E { int n; char pad4[8]; U *vertices; char pad10[0x1C - 0x10]; U *samples; };
struct MESH { char pad[0x14]; };
struct OWNER { char pad[0x40]; M4 mat; };
struct STATE { int pad; OWNER *object; char pad8[0x28 - 8]; };
struct NODE { MESH *mesh; int n; E *entries; NODE *next; };
struct CONFIG { void *owner; char pad4[0xC]; U zero; float limit; char pad24[0x18]; MESH *meshes; STATE *states; void *ae; };
struct S { char pad[0x80]; CONFIG config; char padD0[0xDC - 0xD0]; NODE *head; char padE0[0x150 - 0xE0]; void *path; };
struct AL { int cb, align; };
union PHASE { AL request; U hit; };
struct WORK { PHASE phase; U direction; M3 basis, temporary, inverse; float distance; };
extern char D_002C44F0;

extern "C" {
void PostSoLoad(S *);
M3 *func_001A6318(S *);
void func_0016EF00(CONFIG *, int);
void func_001AE5A0(void *, const void *, unsigned);   /* memcpy */
void *func_001AE1D0(AL *);                            /* allocate (size, align) */
void FindShapeClosestPoint(void *, qword, void *, qword *, qword *, float *, float *);
M3 *BuildOrthonormalMatrixZ(M3 *, qword, qword);
void func_001E9858(S *);
}

/* v * 4x4 (translation row applied through vf0.w) */
static inline qword Point(M4 *m, qword v)
{
    qword r = m->a;
    __asm__("vmulax.xyz $ACC,%0,%4x\n\tvmadday.xyz $ACC,%1,%4y\n\tvmaddaz.xyz $ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w"
            : "+j"(r) : "j"(m->b), "j"(m->c), "j"(m->d), "j"(v));
    return r;
}
/* v * 3x3 */
static inline qword Transform(const M3 &m, qword v)
{
    __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
            : "+j"(v) : "j"(m.a.q), "j"(m.b.q), "j"(m.c.q));
    return v;
}
static inline qword PointDifference(qword world, qword hit)
{
    qword value = world;
    __asm__("vsub.xyzw %0,%0,%1" : "+j"(value) : "j"(hit));
    return value;
}
/* r = transpose of the 3x3 matrix m (MMI interleave/recombine) */
static inline void TransposeInto(M3 &r, const M3 &m)
{
    qword lo, hi, lo1, hi1;
    qword a, b, d;
    __asm__("pextlw %0,%5,%4\n\tpextuw %1,%5,%4\n\tpextlw %2,$zero,%6\n\tpextuw %3,$zero,%6"
            : "=&r"(lo), "=&r"(hi), "=&r"(lo1), "=&r"(hi1) : "r"(m.a.q), "r"(m.b.q), "r"(m.c.q));
    __asm__("pcpyld %0,%4,%3\n\tpcpyud %1,%3,%4\n\tpcpyld %2,%6,%5"
            : "=&r"(a), "=&r"(b), "=&r"(d) : "r"(lo), "r"(lo1), "r"(hi), "r"(hi1));
    r.a.q = a; r.b.q = b; r.c.q = d;
}

/** @sly2-annot addr=0x1E9858
 * Purpose: CID 0x6A (SO child 0x160) vt15: PostSoLoad then rebuild each mesh vertex as (offset in path-local
 *   basis, distance along path)
 * Info: proposed name PostCid106Load (T2); class CID106 slot15; TU shape
 * Notes: verifier PLAUSIBLE: Placeholder class (CID 0x6A vtable slot 15). PostSoLoad, then for each mesh node
 *   re-expresses vertices relative to closest point on the path (orthonormal basis from the tangent and the
 *   object's Z axis | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E9858(S *p)
{
    U vertices[128];
    WORK w;
    PostSoLoad(p);
    if (!p->path || !p->head) return;
    qword axis = func_001A6318(p)->c.q;
    NODE *head = p->head;
    for (NODE *node = head; node; node = node->next) {
        int index = node->mesh - p->config.meshes;
        STATE *state = &p->config.states[index];
        OWNER *object = state->object;
        state->object = (OWNER *)&D_002C44F0;
        int j = 0;
        func_0016EF00(&p->config, index);
        if (j < node->n) {
            PHASE *phase = &w.phase;
            do {
                E *e = &node->entries[j];
                int next = j + 1;
                int k = 0;
                func_001AE5A0(vertices, e->vertices, e->n * 16);
                w.phase.request.cb = e->n * 16;
                phase->request.align = 16;
                U *out = (U *)func_001AE1D0(&phase->request);
                e->samples = out;
                for (k = 0; k < e->n; k++) {
                    qword world = Point(&object->mat, vertices[k].q);
                    FindShapeClosestPoint(p->path, world, 0, &phase->hit.q, &w.direction.q, &w.distance, 0);
                    BuildOrthonormalMatrixZ(&w.temporary, w.direction.q, axis);
                    qword difference = PointDifference(world, w.phase.hit.q);
                    w.basis = w.temporary;
                    TransposeInto(w.temporary, w.basis);
                    w.inverse = w.temporary;
                    out->q = Transform(w.inverse, difference);
                    out->f[3] = w.distance;
                    out++;
                }
                j = next;
            } while (j < node->n);
        }
    }
}

#define OFF(T, f) ((unsigned)&((T *)0)->f)
typedef char guard_E[(sizeof(E) == 0x20 && OFF(E, vertices) == 0xC && OFF(E, samples) == 0x1C) ? 1 : -1];
typedef char guard_NODE[(sizeof(NODE) == 16 && OFF(NODE, entries) == 8 && OFF(NODE, next) == 0xC) ? 1 : -1];
typedef char guard_S[(OFF(S, config.meshes) == 0xBC && OFF(S, config.states) == 0xC0 && OFF(S, head) == 0xDC && OFF(S, path) == 0x150) ? 1 : -1];
typedef char guard_OWNER[(OFF(OWNER, mat) == 0x40 && sizeof(STATE) == 40 && OFF(STATE, object) == 4 && sizeof(MESH) == 20) ? 1 : -1];
typedef char guard_WORK[(sizeof(WORK) == 0xC0 && OFF(WORK, direction) == 0x10 && OFF(WORK, temporary) == 0x50 && OFF(WORK, distance) == 0xB0) ? 1 : -1];
