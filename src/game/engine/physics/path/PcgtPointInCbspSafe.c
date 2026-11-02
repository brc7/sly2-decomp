typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
typedef struct { char b[12]; } LEAF;
typedef union { qword q; struct { char pad[0xC]; unsigned short a[2]; } s; } NODE;
struct T { char pad[0xC]; LEAF *aleaf; int cnode; NODE *anode; };
extern float D_002C0C00;
/** @sly2-annot addr=0x1BF1F8
 * Purpose: Tolerant BSP walk returning nearest triangle
 * Info: proposed name PcgtPointInCbspSafe (T1); TU path
 * Notes: names_v2 B proposed FindClosestPointInCg__FP2CGP6VECTORT1 (0.856)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
LEAF *PcgtPointInCbspSafe(struct T *p, NODE *pn, qword pos, int fl)
{
    union { qword q; float f; } u;
    NODE *pnEnd, *anode;
    int mask;
    qword q;
    q = Make(GetX(pos), GetY(pos), -1.0f);
    u.q = q;
    pnEnd = &p->anode[p->cnode];
    anode = p->anode;
    mask = (fl & 1) ? 0x8000 : 0x4000;
    while (pn != pnEnd) {
        float d = Dot(pn->q, q);
        unsigned int k;
        if (d < -D_002C0C00) {
            k = pn->s.a[0];
        } else if (D_002C0C00 < d) {
            k = pn->s.a[1];
        } else {
            k = pn->s.a[1];
            if (k & 0x8000) {
                if (k & mask) {
                    k &= 0x3FFF;
                    return &p->aleaf[k];
                }
            } else {
                NODE *pnT = anode + k;
                if (pnT != pnEnd) {
                    LEAF *pl = PcgtPointInCbspSafe(p, pnT, pos, fl);
                    if (pl)
                        return pl;
                }
            }
            k = pn->s.a[0];
        }
        if (k & 0x8000) {
            if (!(k & mask))
                return 0;
            k &= 0x3FFF;
            return &p->aleaf[k];
        }
        pn = anode + k;
    }
    return 0;
}
