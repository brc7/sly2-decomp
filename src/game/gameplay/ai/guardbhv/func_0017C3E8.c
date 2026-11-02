typedef int qword __attribute__((mode(TI)));
extern float D_002BE748;
struct D { char pad[0x10]; void *p10; };
struct C { char pad[0xDC]; struct D *pDC; };
struct B { char pad[0xE8]; int nE8; char pad2[8]; qword *pF4; };
struct K { char pad[0xC]; void *pC; char pad2[8]; float f18; };
struct A { char pad[0xE8]; struct C *pE8; char pad2[0x294 - 0xEC]; struct B *p294; char pad3[0x324 - 0x298]; char a324[0xA8]; struct K *p3CC; };
struct T { char pad[0x70]; float f70; };
struct S { char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x14]; int n30; };
void UpdateBhv(struct S *p);
float func_0020A690(struct K *k);
qword func_001A62E8(void *p);
void GetVectorCylind(qword v, float *a, float *b, int n);
float RadNormalize__Ff(float f);
int FCheckLm__FP2LMf(void *p, float f);
qword func_0017BFF8(struct S *p, void *q, float f, qword v);
void func_001C02D0(void *q, qword a, qword b, struct B *c);
void SetBhvpathShape(struct T *t, struct B *b, int n);
/** @sly2-annot addr=0x17C3E8
 * Purpose: BHVCIRCLE Update: when out of the distance band or off-heading relative to the target, re-path to
 *   a new spacing point
 * Info: proposed name UpdateBhv179 (T2); class BHV179 slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvcircle -> UpdateBhv179
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C3E8(struct S *p) {
    struct A *a;
    struct C *c;
    struct D *d;
    void *q;
    struct B *b;
    struct K *k;
    void *o;
    float g;
    struct T *t;
    float out[2];
    qword pos;
    UpdateBhv(p);
    d = 0;
    if (p->n30 != 0) {
    a = p->p10;
    c = a->pE8;
    b = a->p294;
    if (c) d = c->pDC;
    q = d ? d->p10 : 0;
    k = a->p3CC;
    o = k->pC;
    g = func_0020A690(k);
    pos = func_001A62E8(o);
    t = p->p18;
    if (t->f70 > D_002BE748) goto doit;
    {
        qword dv;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(dv) : "j"(b->pF4[b->nE8 - 1]), "j"(pos));
        GetVectorCylind(dv, &out[0], &out[1], 0);
    }
    if (__builtin_fabsf(RadNormalize__Ff(out[0] - g)) > k->f18) goto doit;
    if (FCheckLm__FP2LMf(p->p10->a324, out[1])) return;
doit:
    {
        qword v = func_001A62E8(p->p10);
        func_001C02D0(q, v, func_0017BFF8(p, q, g, pos), b);
    }
    SetBhvpathShape(t, b, 0);
    }
}
