struct E { int oidA; int oidB; int grf; int n; };
class LO {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void V5();
    virtual void V6();
    virtual void V7();
    virtual void V8();
    virtual void V9();
};
struct G;
extern G D_003DD490;
extern "C" int GrfFromState(int n);
extern "C" int GrfFromStates(int c, int *ai);
extern "C" int func_00210A88();
extern "C" void *PloFindSwObject__FP2SWi3OIDP2LO(G *pg, int kind, int oid, void *owner);
extern "C" void ApplyStateLoVisibility(void *p, int nState, int c, E *ae)
{
    int i;
    int grfState = GrfFromState(nState);
    int grfAll;
    if (nState == 4 || nState == 5) {
        int ai[2] = { 4, 5 };
        grfAll = GrfFromStates(2, ai);
    } else {
        int ai[4] = { 0, 1, 2, 3 };
        grfAll = GrfFromStates(4, ai);
    }
    for (i = 0; i < c; i++) {
        E *pe = &ae[i];
        if (!(pe->grf & grfAll))
            continue;
        if (pe->n != -1 && pe->n != func_00210A88())
            continue;
        void *pOwner = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, pe->oidB, 0);
        LO *plo = (LO *)PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, pOwner ? 4 : 5, pe->oidA, pOwner);
        if (!plo)
            continue;
        if (pe->grf & grfState)
            plo->V8();
        else
            plo->V9();
    }
}
