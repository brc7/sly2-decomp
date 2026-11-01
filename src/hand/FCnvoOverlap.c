typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
typedef struct { char pad[0x60]; } HIT;
struct X;
typedef struct { short d; short pad; M4 *(*pfn)(struct X *); } VT;
struct X { int x0, x4, x8; VT *vt; char pad[0xE8 - 0x10]; int c; char pad2[0xF4 - 0xEC]; qword *apos; };
int ClsgClipEdgeToCnvo(struct X *p, qword a, qword b, int n, HIT *phit);
int FPosInCnvo(struct X *p, qword pos);
static inline qword MulPt(M4 *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0]), "j"(v), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3])); return r; }
static inline M4 *GetMat(struct X *p) { return p->vt[31].pfn((struct X *)((char *)p + p->vt[31].d)); }
/** @sly2-annot addr=0x1BD8A8
 * Purpose: Two convex polygons overlap (edge clip or containment)
 * Info: proposed name FCnvoOverlap (T1); TU tu_1BC050
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+verdicts_A2+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int FCnvoOverlap(struct X *p, struct X *q)
{
    int i;
    M4 m = *GetMat(p);
    qword v = MulPt(&m, p->apos[0]);
    HIT hit;
    i = 1;
    if (i < p->c) {
        do {
            qword w = MulPt(&m, p->apos[i]);
            if (ClsgClipEdgeToCnvo(q, v, w, 1, &hit))
                break;
            v = w;
        } while (++i < p->c);
    }
    if (i < p->c)
        return 1;
    return FPosInCnvo(p, MulPt(GetMat(q), q->apos[0])) != 0;
}
