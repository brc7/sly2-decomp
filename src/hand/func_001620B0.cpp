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
extern "C" int func_00161FC8(int n);
extern "C" int func_00161FF0(int c, int *ai);
extern "C" int func_00210A88();
extern "C" void *func_0015E938(G *pg, int kind, int oid, void *owner);
/** @sly2-annot addr=0x1620B0
 * Purpose: For a job/task state change: for each {oidLo, oidContext, grfStates, level} record whose mask
 *   covers the state group (0-3 or 4-5) and level matches, AddLo (vt8) if the state bit is set else RemoveLo
 *   (vt9)
 * Info: proposed name ApplyStateLoVisibility (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_001620B0(void *p, int nState, int c, E *ae)
{
    int i;
    int grfState = func_00161FC8(nState);
    int grfAll;
    if (nState == 4 || nState == 5) {
        int ai[2] = { 4, 5 };
        grfAll = func_00161FF0(2, ai);
    } else {
        int ai[4] = { 0, 1, 2, 3 };
        grfAll = func_00161FF0(4, ai);
    }
    for (i = 0; i < c; i++) {
        E *pe = &ae[i];
        if (!(pe->grf & grfAll))
            continue;
        if (pe->n != -1 && pe->n != func_00210A88())
            continue;
        void *pOwner = func_0015E938(&D_003DD490, 5, pe->oidB, 0);
        LO *plo = (LO *)func_0015E938(&D_003DD490, pOwner ? 4 : 5, pe->oidA, pOwner);
        if (!plo)
            continue;
        if (pe->grf & grfState)
            plo->V8();
        else
            plo->V9();
    }
}
