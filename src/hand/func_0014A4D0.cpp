// func_0014A4D0 (Claude agent, lane D16). From v4: the vsub reads q (the pinned Add result) with `posT = q` after it, and its posMe operand is repeated as an allocation-only operand (reload then puts posMe in vf4 like the target).
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VTE { short d; short pad; void *pfn; };
struct G;
struct XEC { char pad[0x20]; qword pos; float g30; };
struct SO { char pad[0xC]; VTE *vt; char pad1[0xE8 - 0x10]; int grfE8; XEC *pxEC; char pad2[0x174 - 0xF0]; SO *pnext; };
struct PATH { char pad[0x90]; int c; qword *av; };
struct PSO { char pad[0xC]; VTE *vt; char pad1[0x258 - 0x10]; PATH *ppath; char pad2[0x2DC - 0x25C]; int x2DC; };
struct LO { char pad[0xC]; VTE *vt; };
struct W { char pad[0x1010]; LO *plo; };
struct P { char pad[0x10]; W *pw; char pad1[0x48 - 0x14]; float t48; };
struct CLOCK { float t; float dt; };
extern "C" {
extern G D_003DD490;
extern SO *D_003E1450;
extern CLOCK D_002DDED0;
extern float D_002BD460;
extern float D_002BD468[2];
extern float D_002BD470, D_002BD474, D_002BD478, D_002BD47C;
PSO *PpoHero(G *pg);
qword func_001A62E8(void *p);
float GRandInRange__Fff(float a, float b);
int FSwHandsOff(G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(PSO *p, int n);
void func_00208540(W *pw, void *p);
int func_001BBE48(SO *p);
int CploFindSwObjectsByClass(G *pg, int a, int b, void *c, int d, SO **apso);
int NRandInRange__Fii(int a, int b);
qword SetVectorSphere(float a, float b, float c);
float RadBetweenVectors(qword a, qword b, void *p);
float func_002458F0(float y, float x);
qword SetVectorCylind(float a, float b, float c);
void func_0014A4D0(P *p);
}
static inline int FLess(float a, float b) { return a < b; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword SubX(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b), "j"(b)); /* extra operand: allocation only */ return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float Len(qword a)
{
    qword d, one; float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static inline float LenSq(qword a)
{
    qword d, one; float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "0"(a));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
static inline float Len2(qword a)
{
    qword d; float g;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=&j"(d) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static inline float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static inline float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
static inline float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
static inline qword AddScaledNorm(qword a, qword v, VU_FLOAT s)
{
    qword d, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(d) : "j"(d), "j"(v));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(d), "j"(s.q));
    return r;
}

void func_0014A4D0(P *p)
{
    G *pg = &D_003DD490;
    PSO *pso = PpoHero(pg);
    qword posPlayer = func_001A62E8(pso);
    qword posMe = func_001A62E8(p->pw);
    qword d = Sub(posPlayer, posMe);
    qword v;
    qword pos;
    {
        VTE *e = &pso->vt[33];
        v = ((qword (*)(void *))e->pfn)((char *)pso + e->d);
    }
    if (FLess(D_002BD460, Len(v)))
        p->t48 = D_002DDED0.t;
    float dt = D_002DDED0.t - p->t48;
    if (GRandInRange__Fff(D_002BD468[0], D_002BD468[1]) < dt && !FSwHandsOff(pg)) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x34) && pso->x2DC == 0x11) {
            PATH *ppath = pso->ppath;
            pos = func_001A62E8(pso);
            int c = ppath->c;
            for (int i = 0; i < c; i++) {
                if (GetZ(pos) < GetZ(ppath->av[i]))
                    pos = ppath->av[i];
            }
            goto done;
        }
        func_00208540(p->pw, pso);
        return;
    }
    for (SO *ps = D_003E1450; ps; ps = ps->pnext) {
        if (!func_001BBE48(ps)) {
            SO *apso[8];
            qword posT = func_001A62E8(ps);
            float r = D_002BD478;
            if (ps->pxEC) {
                posT = ps->pxEC->pos;
                r = ps->pxEC->g30;
            }
            int c = CploFindSwObjectsByClass(&D_003DD490, 1, 0x12, ps, 8, apso);
            int k = 0;
            for (int i = 0; i < c; i++) {
                if (apso[i]->grfE8 & 0x17E)
                    apso[k++] = apso[i];
            }
            if (k) {
                SO *pt = apso[NRandInRange__Fii(0, k - 1)];
                posT = func_001A62E8(pt);
                r = *(float *)&pt->pxEC;
            }
            if (!FLess(D_002BD478 * D_002BD478, LenSq(Sub(posT, posPlayer)))) {
                float pan = GRandInRange__Fff(-3.1415927f, 3.1415927f);
                float tilt = GRandInRange__Fff(-3.1415927f, 3.1415927f);
                register qword q asm("vf3"); /* register pin: the target's vadd writes posT's new value to vf3 */
                q = Add(posT, SetVectorSphere(pan, tilt, GRandInRange__Fff(0.0f, r)));
                qword e = SubX(q, posMe);
                posT = q;
                if (FLess(Len2(e), Len2(d))) {
                    pos = posT;
                    goto done;
                }
                float g = RadBetweenVectors(d, e, 0);
                if (!(__builtin_fabsf(g) * 57.295776f <= D_002BD47C)) {
                    pos = posT;
                    goto done;
                }
            }
        }
    }
    if (FLess(Len2(v), D_002BD460)) {
        qword e = Sub(posMe, posPlayer);
        float rad = func_002458F0(GetY(e), GetX(e));
        qword pp = func_001A62E8(pso);
        pos = Add(pp, SetVectorCylind(GRandInRange__Fff(rad - 1.5707964f, rad + 1.5707964f), D_002BD470, D_002BD474));
    } else {
        pos = AddScaledNorm(posPlayer, v, VU_FLOAT(-D_002BD470));
    }
done:
    {
        LO *plo = p->pw->plo;
        VTE *e = &plo->vt[29];
        ((void (*)(void *, qword))e->pfn)((char *)plo + e->d, pos);
    }
    func_00208540(p->pw, p->pw->plo);
}
