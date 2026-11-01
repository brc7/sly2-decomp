/* func_001A3F78: collect the best-ranked objects for a query position.
 *
 * Walks the world's plane tree (BSP) down to one leaf using the sign of
 * dot(plane.xyz, pos) - plane.w, ORs that leaf's 256-bit object mask into a local copy of the
 * world mask, then visits every set bit.  Each object's bounding SPHERE gives
 * score = bias + weight * |pos - centre|, and the (score, object) pairs are kept in a
 * descending, capacity-bounded insertion list in `results`.
 *
 * Tricks that mattered (all plain C++, one VU asm per VU op):
 *  - `pos` is taken by const reference inside Score(): that gives it a stack home, so the
 *    pos load is a hoistable invariant instead of a per-iteration reload.
 *  - BitIndex computes `base + 1` into its own local so loop.c can hoist it out of the loop.
 *  - the qmfc2 -> mtc1 pair is written as an `mtc1` asm fed by a qword "r" operand; gcc then emits
 *    the qmfc2 itself (a reload), which fixes the scheduler order and the spill-register choice.
 *  - one function-scope `score` is shared by the tree walk and the ranking loop (target keeps
 *    both in $f2).
 */
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
#define FIELD_OFFSET(t, m) ((uint)&(((t *)0)->m))

/* 16-byte vector whose default constructor is the shared vector initialiser (func_002564B8). */
union U { qword q; float f[4]; U(); };
typedef union { qword q[2]; uint a[8]; } MASK;      /* 256-bit object mask */
struct HOME { qword q; };
struct WORK { MASK mask; HOME distance, unity; };  /* local frame: mask + two vector temporaries */
inline void *operator new(unsigned, void *destination) { return destination; }

typedef struct NODE {                               /* BSP plane node, 32 bytes */
    qword plane;
    struct NODE *left, *right;
    int leftLeaf, rightLeaf;
} NODE;
struct SPHERE {                                     /* bounding sphere at object+0x100 */
    U position;
    float bias, weight;
    float Score(const qword &pos);
};
typedef struct OBJ { char pad[0x100]; SPHERE sphere; } OBJ;
typedef struct { float score; OBJ *object; } RESULT;
typedef struct {
    char pad[0x4154];
    OBJ *objects[256];
    int word4554, word4558;
    NODE *tree;
    int word4560;
    MASK *leaves;
    int word4568, word456C;
    MASK mask;
} WORLD;

extern "C" void *func_002333B4(void *, int, uint);  /* memset */
extern "C" U *func_002564B8(U *);                   /* vector init, returns its receiver */
inline U::U() { (void)func_002564B8(this); }

/* dot(plane.xyz, pos.xyz) - plane.w, via VU0 macro mode */
static inline float PlaneDistance(HOME *first, HOME *second, qword plane, qword pos)
{
    U *d = new ((void *)first) U;
    U *one = new ((void *)second) U;
    qword r, unit;
    float f;
    __asm__("vmul.xyz %0,%0,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddaz.x $ACC,%1,%0z\n\tvmsubw.x %0,%1,%0w"
            : "=j"(r), "=&j"(unit) : "0"(plane), "j"(pos));
    d->q = r;
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(d->q));    /* gcc supplies the qmfc2 for the "r" qword operand */
    one->q = unit;
    return f;
}

/* |a - b| via VU0 macro mode */
static inline float Length(const qword &a, qword b)
{
    qword r, one;
    float f;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(r), "=&j"(one));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(r));
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(r));        /* gcc supplies the qmfc2 */
    return f;
}
inline float SPHERE::Score(const qword &pos) { return bias + weight * Length(pos, position.q); }

/* index of the single set bit `isolated` inside word number base/32 (base = word * 32) */
static inline int BitIndex(uint isolated, int base)
{
    int one = base + 1;                              /* own local so it can be hoisted */
    int result = (isolated & 0xAAAAAAAAU) ? one : base;
    if (isolated & 0xCCCCCCCCU) result += 2;
    if (isolated & 0xF0F0F0F0U) result += 4;
    if (isolated & 0xFF00FF00U) result += 8;
    if (isolated & 0xFFFF0000U) result += 16;
    return result;
}

typedef char Offset_objects[FIELD_OFFSET(WORLD, objects) == 0x4154 ? 1 : -1];
typedef char Offset_tree[FIELD_OFFSET(WORLD, tree) == 0x455C ? 1 : -1];
typedef char Offset_leaves[FIELD_OFFSET(WORLD, leaves) == 0x4564 ? 1 : -1];
typedef char Offset_mask[FIELD_OFFSET(WORLD, mask) == 0x4570 ? 1 : -1];
typedef char Offset_sphere[FIELD_OFFSET(OBJ, sphere) == 0x100 ? 1 : -1];
typedef char Size_NODE[sizeof(NODE) == 32 ? 1 : -1];
typedef char Size_RESULT[sizeof(RESULT) == 8 ? 1 : -1];

/** @sly2-annot addr=0x1A3F78
 * Purpose: Walk the light tree for a point and rank up to N lights affecting it
 * Info: proposed name FindSwLights (T2); TU light
 * Notes: names_v2 C proposed PrepareSwLightsForDraw__FP2SW (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
extern "C" void func_001A3F78(WORLD *p, qword pos, int capacity, RESULT *results)
{
    WORK w;
    NODE *n;
    MASK *leaf;
    uint i;
    float score;
    func_002333B4(results, 0, capacity * sizeof(RESULT));
    w.mask.q[0] = p->mask.q[0];
    w.mask.q[1] = p->mask.q[1];
    if (p->tree) {
        n = p->tree;
        do {
            score = PlaneDistance(&w.distance, &w.unity, n->plane, pos);
            if (score < 0.0f) {
                if (n->left) n = n->left;
                else { leaf = &p->leaves[n->leftLeaf]; break; }
            } else {
                if (n->right) n = n->right;
                else { leaf = &p->leaves[n->rightLeaf]; break; }
            }
        } while (n);
        for (i = 0; i < 8; i++) w.mask.a[i] |= leaf->a[i];
    }
    for (i = 0; i < 8; i++) {
        uint word = w.mask.a[i];
        if (word) {
            do {
                uint remaining = word & (word - 1);
                uint isolated = word & ~remaining;
                OBJ *o = p->objects[BitIndex(isolated, i * 32)];
                score = o->sphere.Score(pos);
                int j = capacity;
                while (j > 0) {
                    if (score < results[j - 1].score) break;
                    if (j < capacity) results[j] = results[j - 1];
                    j--;
                }
                if (j < capacity) { results[j].score = score; results[j].object = o; }
                word = remaining;
            } while (word);
        }
    }
}
