typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct X;
typedef struct { short d; short pad; M4 *(*pfn)(struct X *); } VT;
struct X { int x0, x4, x8; VT *vt; char pad[0xE8 - 0x10]; int c; char pad2[0xF4 - 0xEC]; qword *apos; };
int func_001BC208(qword a, qword b);
int func_001BC278(qword a, qword b, qword c);
int func_001BC530(struct X *p, int i);
static inline qword MulPt(M4 *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0]), "j"(v), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3])); return r; }
static inline M4 *GetMat(struct X *p) { return p->vt[31].pfn((struct X *)((char *)p + p->vt[31].d)); }
/** @sly2-annot addr=0x1BC2F8
 * Purpose: Classify polygon (too few points / convex CW / CCW / not convex) from turn signs
 * Info: proposed name ClassifyCid14Polygon (T2); class CID14; TU tu_1BC050
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest+verdicts_B
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BC2F8(struct X *p)
{
    int cFlip, i, sPrev, sgn;
    M4 m;
    if (p->c < 2)
        return 2;
    cFlip = 0;
    i = 0;
    sPrev = 0;
    m = *GetMat(p);
    sgn = func_001BC208(MulPt(&m, p->apos[0]), MulPt(&m, p->apos[1]));
    for (; i < p->c - 1; i++) {
        int i0 = func_001BC530(p, i);
        int i1 = func_001BC530(p, i0 + 1);
        int i2 = func_001BC530(p, i1 + 1);
        qword c = MulPt(&m, p->apos[i2]);
        qword b = MulPt(&m, p->apos[i1]);
        qword a = MulPt(&m, p->apos[i0]);
        int s = func_001BC208(b, c);
        int t;
        if (s) {
            if (s == -sgn)
                cFlip++;
        }
        sgn = s;
        t = func_001BC278(a, b, c);
        if (t) {
            if (t == -sPrev)
                return 0;
            sPrev = t;
        }
    }
    if (cFlip >= 3)
        return sPrev == 0;
    switch (sPrev) {
    case -1:
        return 4;
    case 1:
        return 3;
    }
    return 2;
}
