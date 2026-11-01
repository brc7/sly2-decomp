struct VTE { short d; short pad; void (*pfn)(); };
struct EV { float g0; unsigned int k; void *pv; union { float g; int n; } uC; };
struct Q { char pad[0xA0]; int cev; struct EV *aev; };
struct A0 { int x0; int x4; int x8; int xC; float g10; float g14; float g18; };
struct A17 { int x0; int x4; int x8; float gC; };
struct A20 { int x0; int x4; float g8; };
struct B { int pad[3]; virtual void Begin(); virtual void End(); virtual void F3(int, int, int, void **, int); virtual void F4(int, struct MSG *); };
struct MSG;
struct P : B { int x10; struct Q *pq; void *po; float g1C; char pad2[0x40 - 0x20]; float g40;
    char pad3[0x5C - 0x44]; char a5C[4]; };
struct G { P *p; G(P *pp) : p(pp) { p->Begin(); } ~G() { p->End(); } };
struct MSG { struct P *p; float g; void *pv; };
extern char D_003DD490[];
extern "C" {
extern char D_002DDF40[];
extern char D_002DFC00[];
void *func_00131270(struct P *);
void func_00210740(void *, int, int, int, int, void *, void *, struct Q *, void *, float, float, float);
void func_002108E8(void *, void *, struct Q *, void *);
void func_0014F750(void *, void *);
void func_00187E28(void *, void *, float);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *, int, void *, void *);
int CidBasic(void *);
void func_0012F9B8(void *, int, int, int, float, float);
void func_001DFD58(void *, int);
void func_00130D30(struct P *, int, float, float);
void func_00130B18(struct P *);
void func_00131990(struct P *, int);
void func_00210338(void *, int, void *, void *, struct Q *, float);
void func_00210538(void *, int, int, int, void *, struct Q *, float);
void func_00210408(void *, int, void *, int, void *, void *, struct Q *);
void func_002104A0(void *, int, void *, int, void *, void *, struct Q *);
void func_001618F8(void *, float);
void func_00161958(void *);
void func_00210678(void *, int, int, void *, struct Q *, float);
void func_00230CF0(struct P *, void *);
void func_001F81E0(void *, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_00143090(void *, void *, void *, void *);
}
extern "C" void func_00131290(struct P *p, struct EV *pev)
{
    G g(p);
    struct MSG msg;
    void *pv;
    struct Q *pq;
    int f;
    p->g1C = pev->g0;
    pq = p->pq;
    f = 0;
    if (p->g40 < 0.0f && pev >= pq->aev)
        f = pev < pq->aev + pq->cev;
    switch (pev->k) {
    case 0: {
        void *v = func_00131270(p);
        struct A0 *pa = (struct A0 *)pev->pv;
        func_00210740(D_003DD490, pa->x0, pa->x4, pa->x8, pa->xC, p->po, v, pq, p->a5C, pa->g10, pa->g14, pa->g18);
        break;
    }
    case 1:
        func_002108E8(D_003DD490, pev->pv, pq, p->a5C);
        break;
    case 2:
        func_0014F750(D_002DDF40, pev->pv);
        break;
    case 3:
        func_00187E28(D_002DFC00, pev->pv, pev->uC.g);
        break;
    case 4: {
        void *po = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pev->pv, func_00131270(p));
        if (po) {
            switch (CidBasic(po)) {
            case 0x6D:
                func_0012F9B8(po, 0, 0, 0, 0.0f, 1.0f);
                break;
            case 0x72:
                func_001DFD58(po, 0);
                break;
            }
        }
        break;
    }
    case 5:
        func_00130D30(p, 0, *(float *)&pev->pv, pev->uC.g);
        break;
    case 6:
        func_00130B18(p);
        break;
    case 9:
    case 10:
        {
            int fT = (pev->k == 9);
            fT ^= f;
            func_00131990(p, fT);
        }
        break;
    case 7:
    case 8: {
        int fT = (pev->k == 7);
        fT ^= f;
        func_00210338(D_003DD490, fT, pev->pv, func_00131270(p), pq, pev->uC.g);
        break;
    }
    case 11: {
        msg.p = p;
        msg.g = pev->g0;
        msg.pv = pev->pv;
        p->F4(9, &msg);
        break;
    }
    case 12: {
        { void **ppv = &pv; *ppv = &pev->pv; p->F3(4, 0, 1, ppv, 0); }
        break;
    }
    case 17: {
        struct A17 *pa = (struct A17 *)pev->pv;
        func_00210538(D_003DD490, pa->x0, pa->x4, pa->x8, func_00131270(p), pq, pa->gC);
        break;
    }
    case 13:
    case 14: {
        int fT = (pev->k == 13);
        fT ^= f;
        func_00210408(D_003DD490, fT, pev->pv, pev->uC.n, p->po, func_00131270(p), pq);
        break;
    }
    case 15:
    case 16: {
        int fT = (pev->k == 15);
        fT ^= f;
        func_002104A0(D_003DD490, fT, pev->pv, pev->uC.n, p->po, func_00131270(p), pq);
        break;
    }
    case 18:
        func_001618F8(D_003DD490, p->g1C);
        break;
    case 19:
        func_00161958(D_003DD490);
        break;
    case 20: {
        struct A20 *pa = (struct A20 *)pev->pv;
        func_00210678(D_003DD490, pa->x0, pa->x4, func_00131270(p), pq, pa->g8);
        break;
    }
    case 21:
        func_00230CF0(p, pev->pv);
        break;
    case 22:
        func_001F81E0(p->po, (int)pev->pv);
        break;
    case 23:
        func_001F81E0(p->po, -1);
        break;
    case 24:
        if (p->po && FIsBasicDerivedFrom__FP5BASIC3CID(p->po, 0x34)) {
            char *pb = (char *)pev->pv;
            func_00143090(p->po, pb, pb + 8, pb + 0x10);
        }
        break;
    }
}
