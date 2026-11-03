// CloneSo: ported from Lilly's personal Sly 2 decomp (src/P2/so.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C44F0 (g_xfWorld)
// 0x001F6068 Clone__2SOP2LO  (MATCHES Sly 2, T1075)
// Virtual 40. LO's clone (the byte copy) took the base's child list, tree record and removed-children mark along: the clone's own
// are put back. Then everything the SO owns is copied for the clone and every pointer into the base's XFs is re-pointed at the
// clone's (CloneXfRef): the XF block (each XF's parent, the record its slot holds in roles 4 / 13 / 15 / 16, its actuators - cloned
// in their order of priority by PactNewClone + InsertNodeBySlot), the three XFs of the object (position, rotation, root), the
// model (CloneGlobset), the collision parts (geometry, BSP and the per-geometry arrays) and the physics record with its region
// entry. A clone in the world joins the contact list, and it gets a bounds record of its own (EnsureSoBounds).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/cm.h"
struct EDGE
{
    ushort aipos[2];
    ushort aisurf[2];
    EDGE *pedgeNext;
    EDGE *pedgeOtherNext;
};
struct SURF
{
    VECTOR normal;
    float gDot;
    EDGE *pedge;
    EDGE *pedgeOther;
    uint grf;
    int ipos;
};
void CloneGeom(GEOM *pgeomSrc, MATRIX4 *pdmat, GEOM *pgeomDst);
void ClearDle(DLE *pdle);
inline int FIsLoInWorld(LO *plo)
{
    return plo->fInWorld != 0;
}
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
struct ACT
{
      int ack;
      SO *pso;
      XF *pxf;
      int nPriority;
      ACT *pactNext;
      int nRole;
    ACT() {}
    ACT(int ack) : ack(ack) {}
};
void InsertNodeBySlot(SO *pso, XF *pxf, ACT *pact);
ACT *PactNewClone(ACT *pactBase, SO *pso, XF *pxf);
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
};
void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase);
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocStackImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
void EnsureSoBounds(SO *pso);
struct IKLIMB
{
      XF *pxfGoal;
      XF *pxfS;
      XF *pxfE;
      XF *pxfEnd;
      undefined1 unk_0x10[0xA0];
      qword qwEndLocal;
      float gLenUpper;
      float gLenLower;
      undefined4 unk_0xc8[6];
};
struct XFLINK
{
      XF *pxfFrom;
      XF *pxfTo;
      undefined4 unk_0x08[2];
};
struct CHAINLINK
{
      undefined1 unk_0x00[0x60];
      CHAIN *pchain;
      XF *pxf;
      undefined4 unk_0x68[2];
};
struct CHAIN
{
      int clink;
      CHAINLINK *alink;
      undefined4 unk_0x08[18];
};
extern XF D_002C44F0;
void AddSoParts(SO *pso);
void CloneGlobset(GLOBSET *pglobset, SO *pso, LO *ploBase);
void CloneBspc(BSPC *pbspcBase, GEOM *pgeomBase, BSPC *pbspc, GEOM *pgeom);
inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocSwCopyImpl(pswalloc, pvBase);
}
inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocStackImpl(pswalloc);
}
void SO::Clone(LO *ploBase)
{
    SO *psoBase = (SO *)ploBase;
    SOTREE *ptreeSave = ptree;
    LO *ploRemovedFirstSave = ploRemovedFirst;
    DL dlChildSave = dlChild;
    int ixf = 0;
    LO::Clone(ploBase);
    ptree = ptreeSave;
    dlChild = dlChildSave;
    ploRemovedFirst = ploRemovedFirstSave;
    psobounds = 0;
    axf = (XF *)PvAllocSwCopy(cxf * sizeof(XF), axf);
    for (ixf = 0; ixf < cxf; ixf++)
    {
        XF *pxf = &axf[ixf];
        CloneXfRef(ploBase, &pxf->pxfParent);
        switch (pxf->nRole)
        {
        case 4:
            CloneXfRef(ploBase, &pxf->pxfRef);
            break;
        case 13:
        {
            IKLIMB *plimb = (IKLIMB *)PvAllocSwCopy(sizeof(IKLIMB), pxf->pxfRef);
            CloneXfRef(ploBase, &plimb->pxfGoal);
            CloneXfRef(ploBase, &plimb->pxfS);
            CloneXfRef(ploBase, &plimb->pxfE);
            CloneXfRef(ploBase, &plimb->pxfEnd);
            plimb->pxfS->pxfRef = (XF *)plimb;
            plimb->pxfE->pxfRef = (XF *)plimb;
            break;
        }
        case 15:
        {
            XFLINK *pxflink = (XFLINK *)PvAllocSwCopy(sizeof(XFLINK), pxf->pxfRef);
            pxf->pxfRef = (XF *)pxflink;
            if (pxflink->pxfFrom != &D_002C44F0)
            {
                CloneXfRef(ploBase, &pxflink->pxfFrom);
            }
            CloneXfRef(ploBase, &pxflink->pxfTo);
            break;
        }
        case 16:
        {
            CHAINLINK *plink = (CHAINLINK *)pxf->pxfRef;
            CHAIN *pchain = plink->pchain;
            if (plink == pchain->alink)
            {
                pchain = (CHAIN *)PvAllocSwCopy(sizeof(CHAIN), pchain);
                pchain->alink = (CHAINLINK *)PvAllocSwCopy(pchain->clink * sizeof(CHAINLINK), pchain->alink);
                for (int ilink = 0; ilink < pchain->clink; ilink++)
                {
                    CHAINLINK *plinkNew = &pchain->alink[ilink];
                    plinkNew->pchain = pchain;
                    CloneXfRef(ploBase, &plinkNew->pxf);
                    plinkNew->pxf->pxfRef = (XF *)plinkNew;
                }
            }
            break;
        }
        }
        if (pxf->pactFirst)
        {
            int cact = 0;
            ACT *pact = pxf->pactFirst;
            do
            {
                pact = pact->pactNext;
                cact++;
            } while (pact);
            STACKSCOPE scope;
            ACT **apact = (ACT **)PvAllocStack(cact * sizeof(ACT *));
            int iact = 0;
            for (pact = pxf->pactFirst; pact; pact = pact->pactNext)
            {
                apact[iact++] = pact;
            }
            pxf->pactFirst = 0;
            for (iact = cact - 1; iact >= 0; iact--)
            {
                ACT *pactNew = PactNewClone(apact[iact], this, pxf);
                InsertNodeBySlot(this, pxf, pactNew);
            }
        }
    }
    CloneXfRef(ploBase, &pxfUnk58);
    CloneXfRef(ploBase, &pxfUnk5c);
    CloneXfRef(ploBase, &pxfRoot);
    CloneGlobset(&globset, this, ploBase);
    int cpartExtra = CpartExtra();
    apart = (SOPART *)PvAllocSwCopy((cpart + cpartExtra) * sizeof(SOPART), apart);
    for (int ipart = 0; ipart < cpart; ipart++)
    {
        SOPART *ppart = &apart[ipart];
        CloneXfRef(ploBase, &ppart->pxf);
        ppart->pso = this;
        ClearDle(&ppart->dle);
        GEOM geom;
        CloneGeom(&ppart->geomWorld, 0, &geom);
        ppart->geomWorld = geom;
        SOPART *ppartBase = &psoBase->apart[ipart];
        CloneBspc(&ppartBase->bspc, &ppartBase->geomWorld, &ppart->bspc, &ppart->geomWorld);
        if (ppart->plvo)
        {
            int cb = sizeof(LVO);
            ppart->plvo = (LVO *)PvAllocSwCopy(cb, ppart->plvo);
            ppart->plvo->mpiposvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposvLocal);
            ppart->plvo->mpiposdvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposdvLocal);
            ppart->plvo->mpisurfsurfx = (SURFX *)PvAllocSwCopy(ppart->geomLocal.csurf * 0xC, ppart->plvo->mpisurfsurfx);
        }
    }
    if (psophys)
    {
        psophys = (SOPHYS *)PvAllocSwCopy(sizeof(SOPHYS), psophys);
        if (psophys->pregent)
        {
            psophys->pregent = (REGENT *)PvAllocSwCopy(sizeof(REGENT), psophys->pregent);
            psophys->pregent->pso = this;
        }
    }
    if (FIsLoInWorld(this))
    {
        AddSoParts(this);
    }
    EnsureSoBounds(this);
}
