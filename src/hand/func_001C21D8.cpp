// func_001C21D8: A* path search over the navigation polygons (lilly5, T1831; = lilly4 with the minimal probe).
// BYTE-IDENTICAL (matchtool MATCH), but NOT promotable under AGENT_GUIDE "Not accepted": the target's never-touched
// object at sp+0x30 is still an invented `EMPTY e` (without it every SImode home is 4 bytes low: 18/321, aligned 0).
// T1831: the probe only needs a user-declared DESTRUCTOR (the cleanup makes it addressable -> slot at its declaration):
// `struct EMPTY { ~EMPTY() {} };` and `struct { int n; ~X() {} }` both MATCH; ctor-only empty class = no slot (18/321);
// an unused local whose address goes to an inline helper that ignores it is purged (18/321). So the real object is a
// never-touched RAII-style object (inline empty dtor) or a volatile. Rest of the code is lilly4 (StoreX estimate).
typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct NAV { char pad[0x124]; int index; };
struct EDGE { char pad[0x128]; NAV *pnavDest; int pad12C; qword posBegin; qword posEnd; };
struct ADJ { int cedge; int *aiedge; };
struct GRAPH { char pad[0x10]; int cnav; char pad14[8]; ADJ *aadj; int cedge; EDGE **apedge; };
struct NODE { NAV *pnav; EDGE *pedge; NODE *pnodeParent; float gCost; float gEstimate; int state; NODE *pnodeNext; NODE *pnodePrev; };

extern "C" {
void *func_001AE478(void *pframe);
void func_001AE4A0(void *pframe, int n);
void *func_001AE378(MSG *pmsg);
void *func_001AE318(MSG *pmsg);
void *func_002333B4(void *pv, int b, unsigned int cb);
qword func_001A62E8(void *pv);
NAV *func_001C0D70(qword pos, int f);
void ClearDl__FP2DL(DL *pdl);
void InitDl__FP2DLi(DL *pdl, int ibDle);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
void InsertDlEntryBefore__FP2DLPvT1(DL *pdl, void *pvNext, void *pv);
int FIsDlEmpty__FP2DL(DL *pdl);
int func_001C26E0(GRAPH *pgraph, EDGE *pedge, void *pvActor);
int func_001C0920(NAV *pnav, qword pos, int f);
}

static inline void *PvAllocClear(int cb) { MSG msg; msg.cb = cb; msg.nKind = 0x10; return func_001AE378(&msg); }
static inline void *PvAlloc(int cb) { MSG msg; msg.cb = cb; msg.nKind = 0x10; return func_001AE318(&msg); }
// The current stack-allocation scope (out of line: 0x1AE478 / 0x1AE4A0)
struct STACKSCOPE { STACKSCOPE(); ~STACKSCOPE(); };
static inline qword QwSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(v), "j"(v));

    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
// Unknown object at sp+0x30 with an empty destructor (see the header comment)
struct EMPTY { ~EMPTY() {} };
static inline NAV *PnavFind(const qword &pos) { return func_001C0D70(pos, 0); }
static inline void AppendNode(DL *pdl, NODE *pnode) { AppendDlEntry__FP2DLPv(pdl, pnode); }
static inline void RemoveNode(DL *pdl, NODE *pnode) { RemoveDlEntry__FP2DLPv(pdl, pnode); }
static inline void InsertNode(DL *pdl, NODE *pnodeNext, NODE *pnode) { InsertDlEntryBefore__FP2DLPvT1(pdl, pnodeNext, pnode); }
static inline float GXOfQw(qword qw) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw)); return x; }
static inline float GDistX(const qword &a, const qword &b) { return GXOfQw(QwLength(QwSub(a, b))); }

static inline void StoreX(float *p, qword q) { __asm__("sw %1,%0" : "=m"(*p) : "r"(q)); }
extern "C" int func_001C21D8(GRAPH *pgraph, void *pvActor, qword posGoal, int fExact, int cpedgeMax, EDGE **apedge)
{
    STACKSCOPE scope;
    NODE *anode;
    int *afAllowed;
    anode = (NODE *)PvAllocClear(pgraph->cnav * sizeof(NODE));
    afAllowed = (int *)PvAlloc(pgraph->cedge * sizeof(int));
    func_002333B4(afAllowed, 0xFF, pgraph->cedge * sizeof(int));
    qword posStart = func_001A62E8(pvActor);
    NAV *pnavStart = PnavFind(posStart);
    NAV *pnavGoal = PnavFind(posGoal);

    if (!pnavStart || !pnavGoal) {
        return 0;
    }
    DL dlOpen;
    DL dlClosed;
    ClearDl__FP2DL(&dlOpen);
    InitDl__FP2DLi(&dlOpen, 0x18);
    ClearDl__FP2DL(&dlClosed);
    InitDl__FP2DLi(&dlClosed, 0x18);
    EMPTY e;
    NODE *pnode = &anode[pnavStart->index];
    pnode->pnav = pnavStart;
    StoreX(&pnode->gEstimate, QwLength(QwSub(posStart, posGoal)));


    pnode->state = 2;
    AppendNode(&dlOpen, pnode);
    while (!FIsDlEmpty__FP2DL(&dlOpen)) {
        pnode = (NODE *)dlOpen.pvFirst;
        if (fExact) {
            if (pnode->pnav == pnavGoal)
                break;
        } else if (func_001C0920(pnode->pnav, posGoal, 1)) {
            break;
        }
        RemoveNode(&dlOpen, pnode);
        pnode->state = 1;
        AppendNode(&dlClosed, pnode);
        qword posFrom;
        if (pnode->pedge)
            posFrom = pnode->pedge->posEnd;
        else
            posFrom = posStart;
        ADJ *padj = &pgraph->aadj[pnode->pnav->index];
        for (int i = 0; i < padj->cedge; i++) {
            int iedge = padj->aiedge[i];
            EDGE *pedge = pgraph->apedge[iedge];
            if (afAllowed[iedge] == -1)
                afAllowed[iedge] = func_001C26E0(pgraph, pedge, pvActor);
            if (!afAllowed[iedge])
                continue;

            float gDist = GDistX(posFrom, pedge->posBegin) + GDistX(pedge->posBegin, pedge->posEnd);
            NAV *pnavDest = pedge->pnavDest;
            float gCost = pnode->gCost + gDist;
            NODE *pnodeDest = &anode[pnavDest->index];
            float gEstimate = gCost + GDistX(pedge->posEnd, posGoal);
            if (pnodeDest->state && pnodeDest->gEstimate <= gEstimate)
                continue;
            switch (pnodeDest->state) {
            case 0:
                break;
            case 1:
                RemoveNode(&dlClosed, pnodeDest);
                break;
            case 2:
                RemoveNode(&dlOpen, pnodeDest);
                break;
            }
            pnodeDest->pnav = pnavDest;
            pnodeDest->pedge = pedge;
            pnodeDest->gCost = gCost;
            pnodeDest->pnodeParent = pnode;
            pnodeDest->gEstimate = gEstimate;
            pnodeDest->state = 2;
            NODE *pnodeNext;
            for (pnodeNext = (NODE *)dlOpen.pvFirst; pnodeNext; pnodeNext = pnodeNext->pnodeNext) {
                if (pnodeNext->gEstimate > pnodeDest->gEstimate)
                    break;
            }
            InsertNode(&dlOpen, pnodeNext, pnodeDest);
        }
    }
    int c = 0;
    if (!FIsDlEmpty__FP2DL(&dlOpen)) {
        if (!fExact && !pnode->pnodeParent) {
            apedge[0] = 0;
            c = 1;
        } else {
            int cpedge = 0;
            NODE *pnodeT;
            for (NODE *pnodeCount = pnode; pnodeCount->pnodeParent; pnodeCount = pnodeCount->pnodeParent)
                cpedge++;
            if (cpedge < cpedgeMax)
                c = cpedge;
            else
                c = cpedgeMax;
            for (pnodeT = pnode; pnodeT->pnodeParent; pnodeT = pnodeT->pnodeParent) {
                if (cpedge <= cpedgeMax)
                    apedge[cpedge - 1] = pnodeT->pedge;
                cpedge--;
            }
        }
    }
    return c;
}
