typedef int qword __attribute__((mode(TI)));
struct LO { int k; };
struct Q4 { char pad[0x40]; struct LO *x40; };
struct E { int pad[2]; void *x8; void *pxC; char pad2[0x30]; };
struct SO { struct E ae[2]; char pad[0xAC - 0x80]; struct SO *pnext; };
struct LN { struct SO *pso; struct LN *pnext; };
struct X { char pad[0x64]; struct LN *x64; };
struct PO { char pad[0xEC]; struct X *xEC; char pad2[0x2DC - 0xF0]; int x2DC; char pad3[0x1084 - 0x2E0]; struct Q4 *x1084; };
struct T { char pad[0x130]; float x130; };
struct R { char pad[0x4C]; int x4C; };
struct P { struct PO *p0; char pad[0xC]; qword pos; float d20; float t24; int f28; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002BD2D0;
extern float D_002BD2D4;
extern char D_003DD490[];
extern char D_002DEB30[];
int func_00144068(struct PO *);
int FSwHandsOff(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct T *, int);
struct R *func_00205458(void *, int);
void BroadcastGameEvent(void *, int, struct T *);
void FadeSoOut(struct T *, float);
qword func_001A62E8(void *p);
int func_00173610(float, float, qword *, int, int);
int func_0017D608(qword *);
void func_0019F540(struct PO *, qword);

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

static inline int IOther(struct PO *p0, struct SO *pso)
{
    return p0 == pso->ae[0].x8 ? 0 : 1;
}

void func_00144640(struct P *p)
{
    if (p->f28) {
        if (p->p0->x2DC == -1)
            p->f28 = 0;
        return;
    }
    if (!func_00144068(p->p0))
        return;
    if (D_002DDED0.t < p->t24)
        return;
    p->t24 = D_002DDED0.t + D_002BD2D0;
    if (FSwHandsOff(D_003DD490)) {
        int fAny = 0;
        struct LN *pln;
        for (pln = p->p0->xEC->x64; pln; pln = pln->pnext) {
            struct SO *pso;
            for (pso = pln->pso; pso; pso = pso->pnext) {
                int i = IOther(p->p0, pso);
                struct T *po = pso->ae[1 - i].x8;
                if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x38)) {
                    struct R *pr = func_00205458(po, 0xA9);
                    if (!pr || !pr->x4C) {
                        if (0.0f <= po->x130)
                            BroadcastGameEvent(D_002DEB30, 0x17, po);
                        FadeSoOut(po, 0.5f);
                        fAny = 1;
                    }
                }
            }
        }
        if (fAny)
            return;
    }
    if (p->p0->x1084) {
        if (p->p0->x1084->x40->k != 0x24)
            return;
    } else {
        struct R *pr = func_00205458(p->p0, 0xA8);
        if (!pr)
            return;
        {
            int k = pr->x4C;
            if (k >= 2)
                return;
            if (k < 0)
                return;
        }
    }
    {
        qword r = func_001A62E8(p->p0);
        float g = GDist(r, p->pos);
        if (p->d20 - g < D_002BD2D4 * D_002BD2D0) {
            qword v = p->pos;
            if (func_00173610(100.0f, -150.0f, &v, 0, 0) || func_0017D608(&v)) {
                func_0019F540(p->p0, v);
                p->f28 = 1;
            }
        }
        p->d20 = g;
    }
}
