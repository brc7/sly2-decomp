typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x28]; int x28; signed char x2C; char x2D; long k : 4; unsigned long b : 8; unsigned int f : 1; };
struct E { struct O *po; int fOn; int pad[2]; };
struct L { char pad[0xFC]; int c; struct E ae[1]; };
struct P { char pad[0x38C]; struct O *x38C; struct O *x390; char pad2[0xF64 - 0x394]; struct L *pl; };
extern float D_002A4EE8;
extern float D_002BD4B0[3];
int func_00209A68(struct P *p, int k);
struct O *func_002082D0(struct P *p);
qword func_001A62E8(void *p);
int func_00132B10(struct O *po, int k);
static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
/** @sly2-annot addr=0x14B290
 * Purpose: vt88 (STEPGUARD own slot): choose best candidate object by kind/distance
 * Info: proposed name Carmelita_VUnk88 (T2); class CARMELITA slot88; TU tu_14A008
 * Notes: agent qualifier: | Cid3F->Cid63 (CID 0x3F=63, hex-literal) | class identity from whole_game WE
 *   names/object strings: Cid63_VUnk88 -> Carmelita_VUnk88
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
struct O *func_0014B290(struct P *p)
{
    struct O *poBest;
    struct O *po;
    float gBest;
    int kBest;
    qword pos;
    int i;
    if (func_00209A68(p, 0x24))
        return func_002082D0(p);
    poBest = 0;
    po = p->x390;
    if (po && po->f) {
        poBest = po;
    } else {
    gBest = D_002A4EE8;
    kBest = 999;
    pos = func_001A62E8(p);
    for (i = 0; i < p->pl->c; i++) {
        struct E *pe = &p->pl->ae[i];
        int k;
        float g;
        if (!pe->fOn)
            continue;
        if (func_00132B10(pe->po, 0x34))
            k = 0;
        else if (func_00132B10(pe->po, 0x3A))
            k = 1;
        else
            k = 2;
        g = GDist(pos, func_001A62E8(pe->po));
        if (pe->po == p->x38C)
            g = D_002BD4B0[0] + g * (D_002BD4B0[1] + g * D_002BD4B0[2]);
        if (k < kBest || (k == kBest && g < gBest)) {
            kBest = k;
            gBest = g;
            poBest = pe->po;
        }
    }
    }
    return poBest;
}
