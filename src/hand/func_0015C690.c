// (Claude agent) MATCH: g8 if/else reload form, max via local h=0.5f, pfrm transform split into mul asm + vadd asm with t loaded late.
typedef int qword __attribute__((mode(TI)));
typedef struct EMITP { int x0; int x4; int x8; int xC; int x10; } EMITP;
typedef struct XF4 { qword v; } XF4;
typedef struct X4 { char pad[0xF4]; XF4 *p; } X4;
typedef struct FRM { qword pos; qword m[3]; } FRM;
typedef struct EMITB {
    int x0; int x4; EMITP *pemitp; int xC; int x10; int x14; int x18; FRM *pfrm; int pad20[4];
    union { qword v; qword *pm; EMITP *pemitp; struct { char pad[0x20]; qword v; } *pq; } u30;
    qword v40;
    float g50, g54;
    char pad58[0xD8 - 0x58];
    float gD8, gDC;
    qword vE0[2];
} EMITB;
typedef struct SND { int id; float g4, g8, gC, g10; } SND;
typedef struct EMITTER { char pad[0xE4]; EMITB *pemitb; int xE8, xEC; SND *psnd; float gF4, gF8; int xFC; int x100; int x104; } EMITTER;
typedef struct EMITO { int grf; int x4; float g8; float gC; qword v10; qword v20; int x30; int x34; int x38; } EMITO;
typedef struct EMITR { int *p104; int *p100; EMITTER *p; } EMITR;

extern char D_002BDEC0[];
extern float GRandInRange__Fff(float lo, float hi);
extern float func_0011D948(void *p, float g);
typedef union V4 { qword q; float a[4]; } V4;
extern V4 func_00256CE8(float g);
extern V4 func_00256D08(qword *pv);
extern qword func_00256D28(qword a, qword b);
extern void func_00256D18(qword *pv, qword a);
extern void func_0015BD20(int c, EMITB *pb, EMITR *pr);
extern qword func_001A6F00(int x, int y, qword v);
extern void *func_001FBB90(int id);
extern void func_001FB2F8(void *p, void *pso);
extern void func_001FB3F0(void *p, qword v);
extern void func_001FB4F0(void *p, float a, float b);
extern void func_001FB1F8(void *p, float g);
extern void func_001FB2C0(void *p, float g);

void func_0015C690(EMITTER *p, EMITO *po)
{
    EMITP emitp;
    EMITB b;
    EMITR r;
    EMITR *pr;
    int c;
    V4 pos;
    V4 h;

    emitp = *p->pemitb->pemitp;
    b = *p->pemitb;
    b.pemitp = &emitp;
    if (po->grf & 0x80)
        c = po->x38;
    else
        c = (int)GRandInRange__Fff(p->gF4, p->gF8);
    if (po->grf & 1)
        emitp.x0 = po->x4;
    if (po->grf & 2)
        b.v40 = po->v10;
    if (po->grf & 4) {
        b.u30.v = po->v20;
        b.x10 = 0;
    } else if (po->grf & 0x40) {
        b.pfrm = (FRM *)po->x34;
    }
    if ((po->grf & 8) && b.x4 == 0) {
        float g;
        float gs;
        int n;
        if (po->g8 == 0.0f)
            g = po->gC * 0.01f;
        else
            g = po->g8;
        if (g < 1.0f)
            c = (int)(c * g);
        else
            c = (int)(c / g);
        n = (int)p->gF8 / 2;
        if (!(n < c))
            c = n;
        gs = func_0011D948(D_002BDEC0, g);
        b.g50 *= gs;
        b.g54 *= gs;
        { float h = 0.5f; g = (h < g) ? g : h; }
        func_00256D18(b.vE0, func_00256D28(func_00256CE8(g).q, func_00256D08(b.vE0).q));
    }
    if (b.x4 == 1) {
        if (po->grf & 0x10)
            b.gDC = b.gD8 = po->g8;
        if (po->grf & 0x20)
            b.pemitp->x10 = po->x30;
    }
    if (p->xFC) {
        r.p104 = &p->x104;
        r.p100 = &p->x100;
        r.p = p;
        pr = &r;
    } else {
        pr = 0;
    }
    if (emitp.x0 == 0 && b.x10 != 2 && !(po->grf & 0x48))
        emitp.x0 = (int)p;
    func_0015BD20(c, &b, pr);
    if (p->psnd) {
        void *ps;
        switch (b.x10) {
        case 0:
            pos.q = b.u30.v;
            break;
        case 1: {
            float half = 0.5f;
            {
                register qword hv __asm__("vf1");
                __asm__("qmtc2.ni %1, %0" : "=j"(hv) : "r"(half));
                h.q = hv;
                {
                    qword *pm = b.u30.pm;
                    register qword m1 __asm__("vf2") = pm[1];
                    register qword m0 __asm__("vf3") = pm[0];
                    register qword r __asm__("vf1");
                    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(m0), "j"(m1), "j"(hv));
                    pos.q = r;
                }
            }
            break;
        }
        case 2:
            pos.q = ((X4 *)b.pemitp->x4)->p->v;
            break;
        case 3:
            pos.q = b.u30.pq->v;
            break;
        }
        if (b.pfrm) {
            FRM *pf = b.pfrm;
            qword *pm = pf->m;
            register qword v __asm__("vf3") = pos.q;
            register qword m0 __asm__("vf4") = pm[0];
            register qword m1 __asm__("vf1") = pm[1];
            register qword m2 __asm__("vf5") = pm[2];
            __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "0"(v), "j"(m0), "j"(m1), "j"(m2));
            {
            register qword t __asm__("vf2") = pf->pos;
            __asm__("vadd.xyzw %0, %0, %1" : "=j"(v) : "j"(t), "0"(v));
            }
            pos.q = v;
        }
        pos.q = func_001A6F00(emitp.x0, 0, pos.q);
        ps = func_001FBB90(p->psnd->id);
        func_001FB2F8(ps, 0);
        func_001FB3F0(ps, pos.q);
        func_001FB4F0(ps, p->psnd->g4, p->psnd->g8);
        func_001FB1F8(ps, p->psnd->gC);
        func_001FB2C0(ps, p->psnd->g10);
    }
}
