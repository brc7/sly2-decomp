typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct X;
typedef struct { short d; short pad; M4 *(*pfn)(struct X *); } VT;
struct O { char pad[0xA4]; int xA4; };
struct X { int x0, x4, x8; VT *vt; char pad0[0x50 - 0x10]; struct O *po; char pad[0xE8 - 0x54]; int c; char pad2[0xF4 - 0xEC]; qword *apos;
           char pad3[0x1E8 - 0xF8]; int k; qword *aplane; };
extern qword D_002A2DC0;
void InitPlaneFromNormalPos(qword *pplane, qword n, qword pos);
static inline qword MulPt(M4 *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0]), "j"(v), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3])); return r; }
static inline M4 *GetMat(struct X *p) { return p->vt[31].pfn((struct X *)((char *)p + p->vt[31].d)); }
static inline qword Normalize(qword n)
{
    qword l, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(n));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(l));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(n));
    return r;
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Cross(qword a, qword b) { qword r; __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x1BD400
 * Purpose: (Re)build edge planes of the polygon in world space when its transform changed
 * Info: proposed name BuildCnvoPlanes (T2); TU tu_1BC050
 * Notes: names_v2 B proposed PsoGetJtEffect__FP2JTPi (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BD400(struct X *p)
{
    int i;
    int k = p->po ? p->po->xA4 : 0;
    if (p->k != k) {
        M4 m;
        qword v;
        i = 1;
        m = *GetMat(p);
        v = MulPt(&m, p->apos[0]);
        if (i < p->c) {
            do {
                qword v0 = v;
                v = MulPt(&m, p->apos[i]);
                InitPlaneFromNormalPos(&p->aplane[i - 1], Normalize(Cross(Sub(v, v0), D_002A2DC0)), v0);
            } while (++i < p->c);
        }
        p->k = k;
    }
}
