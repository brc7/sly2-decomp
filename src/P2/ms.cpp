// ms.cpp -- MS (cid 0x31, a static mesh SO with no fields of its own). TU 0x1B4438..0x1B4728 (5 functions, all matching).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_ms
// SLY2_ALIAS: _GLOBAL_$I$*=_GLOBAL_$I$ms
#include "sly2/so.h"
#include "sly2/slotheap.h"
#include "dmas.h"

struct MS : public SO { };   /* same size as SO (CONFIRMED: class descriptor cb) */

struct INTSTACK { char ab[0x410]; };   /* scene render-list scratch (HYPOTHESIS name; size CONFIRMED by match: frame) */

void InitSo(SO *pso);
void InitIntStack(INTSTACK *pis);
void CullSceneRenderList(INTSTACK *pis, GLOBSET *pglobset);
void RenderSoGlobset(GLOBSET *pglobset, int a, int b, float f, float g);
void *PvAllocStackImpl(ALLOCREQ *preq);

/* Glob owner list walked by CollectMsGlobs (HYPOTHESIS: per-world list of mesh owners; fields CONFIRMED by match). */
struct GLOBREF { char pad[0x10]; long grf; char pad2[0xB0 - 0x18]; };
struct GLOBOWNER { char pad[0x20]; GLOBOWNER *pownerNext; char pad1[0xE0 - 0x24]; int cglobref; GLOBREF *aglobref; };
extern GLOBOWNER *g_pglobownerFirst;   /* 0x3E0714 */

// vt slot 7 (Init)
void InitMs(MS *pms)
{
    InitSo(pms);
    *(ulong *)((char *)pms + 0x148) &= ~(1UL << 35);   /* SO flag word at 0x148, bit 35 (bitfield name open) */
}

// vt slot 19 (RenderSelf)
void RenderMsGlobset(MS *pms)
{
    if (pms->globset.aposeset) {   // posed (skinned) globs go through the scene render list
        INTSTACK is;
        InitIntStack(&is);
        CullSceneRenderList(&is, &pms->globset);
    } else {
        RenderSoGlobset(&pms->globset, 0, 0, 1.0f, 1.0f);
    }
}

// Gather every GLOBREF without flag bit 48 into a stack-allocated array.
void CollectMsGlobs(int *pc, GLOBREF ***papglobref)
{
    GLOBOWNER *powner;
    GLOBREF **apglobref, **ppglobref;
    ALLOCREQ req;
    int c = 0, i;
    for (powner = g_pglobownerFirst; powner; powner = powner->pownerNext) c += powner->cglobref;
    req.cb = c * 4;
    req.cbAlign = 0x10;
    apglobref = (GLOBREF **)PvAllocStackImpl(&req);
    ppglobref = apglobref;
    for (powner = g_pglobownerFirst; powner; powner = powner->pownerNext) {
        for (i = 0; i < powner->cglobref; i++) {
            GLOBREF *pglobref = &powner->aglobref[i];
            if (!(pglobref->grf & (1L << 48))) *ppglobref++ = pglobref;
        }
    }
    *pc = ppglobref - apglobref;
    *papglobref = apglobref;
}

SLY2_INSTANTIATE_SLOT_POOLS()
