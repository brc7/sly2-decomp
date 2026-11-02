typedef int qword __attribute__((mode(TI)));
struct LO { char pad0[0x70]; qword pos; };
struct FILTER;
struct NODE { NODE *pnodeLeft, *pnodeRight; qword *pplane; };
struct CONTACT;
struct WORLD;
// one collision shape of an object
struct SHAPE {
    LO *plo;                        // 0x00
    FILTER *pfilter;                // 0x04
    float sRadius;                  // 0x08
    int unk0C;
    unsigned long grf;              // 0x10
    int grfFilter;                  // 0x18
    int iFirst;                     // 0x1C
    char pad20[0x40];
    SHAPE *pshapeNext;              // 0x60
    char pad64[8];
    int cnodeMax;                   // 0x6C
    char pad70[4];
    NODE *pnodeRoot;                // 0x74
    int c;                          // 0x78
    char pad7C[0x2C];
    int genOld, gen;                // 0xA8, 0xAC
};
struct SO {
    char pad0[0x20];
    qword pos;                      // 0x20
    float sRadius;                  // 0x30
    SHAPE *pshapeFirst;             // 0x34
    char pad38[0x34];
    CONTACT *pcontactFirst;         // 0x6C
    unsigned long grf;              // 0x70
};
struct STACKSCOPE { STACKSCOPE(); ~STACKSCOPE(); };
// ach[2] is never touched; it only places the scope at sp+2 (target `addiu v0,sp,2`), the same shape as the
// accepted src/game/gameplay/player/grab/func_00192A28.cpp (struct { char c; FRAME frame; int i, j; }). Judgment call: see notes/1A0000.md (T1866).
struct SCOPE2 { char ach[2]; STACKSCOPE scope; };
struct CNODE { int c; };
struct ALLOCREQ { int cb, cbAlign; ALLOCREQ(int c, int a) { cb = c; cbAlign = a; } };
extern NODE D_002BBB20;
extern "C" {
void *func_001AE318(const ALLOCREQ &req);
int func_00118A88(NODE *pnode, int cnodeMax, qword pos, float sRadius, int *pcnode, NODE **ppnode);
int func_001F7530(FILTER *pfilter, int grf);
void func_001A9D38(WORLD *pworld, SHAPE *pshape, SHAPE *pshapeOther, CONTACT **ppcontact);
void func_001A9C40(WORLD *pworld, SHAPE *pshape, SHAPE *pshapeOther, CONTACT **ppcontact);
void func_001A9B28(WORLD *pworld, SHAPE *pshape, SHAPE *pshapeOther, int cnode, NODE *pnode, CONTACT **ppcontact);
void func_001A8B50(WORLD *pworld, SHAPE *pshape, SHAPE *pshapeOther, int cnode, NODE *pnode, NODE *pnodeLast, CONTACT **ppcontact);
void func_001A9FE0(WORLD *pworld, CONTACT *pcontact, CONTACT **ppcontact);
CONTACT *func_001AA020(WORLD *pworld, CONTACT *pcontact);
void func_001AA0B0(WORLD *pworld, SO *pso, SO *psoOther, SHAPE *pshape, int cnodeMax, NODE *pnode, NODE *pnodeRoot, CONTACT **ppcontact);
}
static inline float LengthSq(qword v)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(d));
    return g;
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline int NMin(int a, int b) { return (a < b) ? a : b; }
#define SHAPE_FILTERED (1UL << 48)
#define SHAPE_SPHERE (1UL << 49)

/** @sly2-annot addr=0x1AA1D8
 * Purpose: Build all contacts between two SOs: order by radius, per shape pair dispatch sphere/sphere,
 *   sphere/BSP, BSP/BSP, sweep, zap responders
 * Info: proposed name AddSoXps (T1); TU mark
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
extern "C" CONTACT *func_001AA1D8(WORLD *pworld, SO *psoSmall, SO *psoLarge)
{
    CONTACT *pcontactFirst = 0;
    SO *psoT = psoSmall;
    if (psoSmall->sRadius > psoLarge->sRadius) {
        psoSmall = psoLarge;
        psoLarge = psoT;
    }
    int cnodeMax = 0;
    SHAPE *pshape;
    SHAPE *pshapeOther;
    for (pshape = psoLarge->pshapeFirst; pshape; pshape = pshape->pshapeNext)
        if (cnodeMax < pshape->cnodeMax)
            cnodeMax = pshape->cnodeMax;
    SCOPE2 stackscope;
    cnodeMax = NMin(cnodeMax, 0x1555);
    NODE *anode = (NODE *)func_001AE318(ALLOCREQ(cnodeMax * sizeof(NODE), 0x10));
    for (pshape = psoLarge->pshapeFirst; pshape; pshape = pshape->pshapeNext) {
        int fFilter = pshape->grfFilter != 0;
        if ((pshape->grf & SHAPE_FILTERED) && !fFilter)
            continue;
        NODE *pnodeFirst;
        int cnode;
        NODE *pnodeLast;
        int status = 2;
        if ((pshape->grf & SHAPE_SPHERE) || (psoSmall->grf & 0x20)) {
            pnodeLast = 0;
            pnodeFirst = 0;
            cnode = 0;
        } else {
            CNODE query;
            query.c = cnodeMax;
            pnodeLast = anode;
            pnodeFirst = anode;
            status = func_00118A88(pshape->pnodeRoot, 0x7FFFFFFF, psoSmall->pos, psoSmall->sRadius, &query.c, &pnodeLast);
            if (status == 1)
                continue;
            if (status == 0) {
                cnode = 0;
                pnodeLast = &D_002BBB20;
                pnodeFirst = &D_002BBB20;
            } else {
                cnode = pnodeLast - pnodeFirst;
                pnodeLast--;
            }
        }
        for (pshapeOther = psoSmall->pshapeFirst; pshapeOther; pshapeOther = pshapeOther->pshapeNext) {
            int fFilterOther = pshapeOther->grfFilter != 0;
            if ((pshapeOther->grf & SHAPE_FILTERED) && !fFilterOther)
                continue;
            if ((pshape->grf & SHAPE_FILTERED) && (pshapeOther->grf & SHAPE_FILTERED))
                continue;
            CONTACT *pcontactTemp = 0;
            CONTACT **ppcontact = &pcontactFirst;
            if (fFilter) {
                int fMatch = func_001F7530(pshapeOther->pfilter, pshape->grfFilter) != 0;
                if (pshape->grf & SHAPE_FILTERED) {
                    if (!fMatch)
                        continue;
                    ppcontact = &pcontactTemp;
                } else if (fMatch) {
                    if (!(pshapeOther->grf & SHAPE_FILTERED))
                        ppcontact = &pcontactTemp;
                }
            }
            if (fFilterOther) {
                int fMatch = func_001F7530(pshape->pfilter, pshapeOther->grfFilter) != 0;
                if (pshapeOther->grf & SHAPE_FILTERED) {
                    if (!fMatch)
                        continue;
                    ppcontact = &pcontactTemp;
                } else if (fMatch && !(pshape->grf & SHAPE_FILTERED)) {
                    ppcontact = &pcontactTemp;
                }
            }
            CONTACT *pcontactPrev = *ppcontact;
            if (pshapeOther->gen != pshapeOther->genOld || pshape->gen != pshape->genOld) {
                if (pshapeOther->iFirst < pshapeOther->c)
                    func_001A9D38(pworld, pshapeOther, pshape, ppcontact);
                if (pshape->iFirst < pshape->c)
                    func_001A9D38(pworld, pshape, pshapeOther, ppcontact);
            }
            if (*ppcontact == pcontactPrev) {
                if (pshape->grf & SHAPE_SPHERE) {
                    if (pshapeOther->grf & SHAPE_SPHERE)
                        func_001A9C40(pworld, pshapeOther, pshape, ppcontact);
                    else
                        func_001A9B28(pworld, pshape, pshapeOther, 0, 0, ppcontact);
                } else if (pshapeOther->grf & SHAPE_SPHERE) {
                    int fFar = 0;
                    if (!status || 1.0f < LengthSq(Sub(pshape->plo->pos, psoSmall->pos)) ||
                        1.0f < __builtin_fabsf(pshape->sRadius - psoSmall->sRadius))
                        fFar = 1;
                    func_001A9B28(pworld, pshapeOther, pshape, fFar ? 0 : cnode, fFar ? 0 : pnodeFirst, ppcontact);
                } else {
                    func_001A8B50(pworld, pshapeOther, pshape, cnode, pnodeFirst, pnodeLast, ppcontact);
                }
            }
            if (pcontactTemp) {
                int fCopied = 0;
                if (fFilter) {
                    if (pshape->grf & SHAPE_FILTERED)
                        func_001A9FE0(pworld, pcontactTemp, &psoLarge->pcontactFirst);
                    else if (!(pshapeOther->grf & SHAPE_FILTERED)) {
                        fCopied = 1;
                        func_001A9FE0(pworld, func_001AA020(pworld, pcontactTemp), &psoLarge->pcontactFirst);
                    }
                }
                if (fFilterOther) {
                    if (pshapeOther->grf & SHAPE_FILTERED)
                        func_001A9FE0(pworld, pcontactTemp, &psoSmall->pcontactFirst);
                    else if (!(pshape->grf & SHAPE_FILTERED)) {
                        fCopied = 1;
                        func_001A9FE0(pworld, func_001AA020(pworld, pcontactTemp), &psoSmall->pcontactFirst);
                    }
                }
                if (fCopied)
                    func_001A9FE0(pworld, pcontactTemp, &pcontactFirst);
            }
        }
        if (!(pshape->grf & SHAPE_FILTERED))
            func_001AA0B0(pworld, psoSmall, psoLarge, pshape, pshape->cnodeMax, pshape->pnodeRoot, pshape->pnodeRoot, &pcontactFirst);
    }
    for (pshapeOther = psoSmall->pshapeFirst; pshapeOther; pshapeOther = pshapeOther->pshapeNext)
        if (!(pshapeOther->grf & SHAPE_FILTERED))
            func_001AA0B0(pworld, psoLarge, psoSmall, pshapeOther, pshapeOther->cnodeMax, pshapeOther->pnodeRoot, pshapeOther->pnodeRoot, &pcontactFirst);
    return pcontactFirst;
}
