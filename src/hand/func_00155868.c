// func_00155868: pick the E whose nearer end (min of |a-pos|, |b-pos|) is farthest from pos.
typedef int qword __attribute__((mode(TI)));
struct E { int x0; void *p4; char pad[0x14 - 8]; void *p14; char pad2[0x20 - 0x18]; int x20; };
struct L { char pad[0x1010]; int c; struct E *ae; };
struct X { char pad[0x10]; struct L *pl; char pad1[0x38 - 0x14]; void *p38; };
extern int D_002BDC50;
extern float D_002A5130;
qword func_001A62E8(void *p);
static inline qword Dot(qword v)
{
    qword d, one;
    __asm__ __volatile__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    return d;
}
static inline qword Sqrt(qword v) { qword d; __asm__ __volatile__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(v)); return d; }
static inline float GFromQ(qword v) { float g; __asm__ __volatile__("qmfc2.ni %0, %1" : "=r"(g) : "j"(v)); return g; }
static inline float GMin(float a, float b) { return a < b ? a : b; }
/** @sly2-annot addr=0x155868
 * Purpose: Chooses the route whose nearer end is farthest from the player
 * Info: proposed name PrteDimitriFarthest (T2); class DIMITRI; TU tu_1553F0
 * Notes: Cid3B->Cid59 (CID 0x3B=59, hex-literal) | class identity from whole_game WE names/object strings:
 *   PrteCid59Farthest -> PrteDimitriFarthest
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
struct E *func_00155868(struct X *p)
{
    struct E *peBest = 0;
    int i;
    qword pos = func_001A62E8(p->p38);
    float gBest = D_002A5130;
    for (i = 0; i < p->pl->c; i++) {
        struct E *pe = &p->pl->ae[i];
        if (D_002BDC50 || !pe->x20) {
            qword a = func_001A62E8(pe->p4);
            qword b = func_001A62E8(pe->p14);
            qword da, db, la, lb;
            float g;
            {
                register qword pp asm("vf1") = pos; /* the one register pin: the in-loop pos copy is vf1 in the target */
                __asm__ __volatile__("vsub.xyzw %0, %1, %2" : "=j"(da) : "j"(a), "j"(pp), "j"(pos), "j"(b), "r"(b), "r"(b));
                /* "j"(pos), "j"(b), "r"(b) x2: allocation-only. "j"(pos) keeps pos in the VF class (spilled to 0(sp));
                   the "r" uses make b a GPR pseudo (it stays in v0), so its qmtc2 is an input reload of this asm, after the
                   two lqc2s, and db reuses that vf3 copy as in the target. */
                __asm__ __volatile__("vsub.xyzw %0, %2, %1" : "=j"(db) : "j"(pp), "j"(b));
            }
            la = Dot(da);
            lb = Dot(db);
            la = Sqrt(la);
            lb = Sqrt(lb);
            { float ga = GFromQ(la); float gb = GFromQ(lb); g = GMin(ga, gb); }
            if (gBest < g) {
                gBest = g;
                peBest = pe;
            }
        }
    }
    return peBest;
}
