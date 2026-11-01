// SetLoIdProxy: ported from Lilly's personal Sly 2 decomp (src/P2/proxy.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_003DD490 (g_sw)
// 0x001CA118 SetIdProxy__2LOii  (MATCHES Sly 2, T0879)
// Virtual 24: puts the object in the proxy copy tree under idProxy: the path is the ids from the root
// down to the object's current node (at most 16, deepest last) with idProxy added at the end. The
// nodes the object owned (PXR::plo == this) are let go, and the new path's nodes that belong to
// sources of a single object and are not owned yet become the object's. fChildren: the children
// (CploChildren) follow with the same id, inside a stack allocation scope.
// How it matched (re/notes/decomp-compiler-quirks.md section 8): one iterator for all three walks, the
// release and claim walks as `for (...; id != -1; step) { if (...) ...; else break; }`, and the first walk
// testing idProxy in the body and reading it again for the store (gcc's PRE turns that into the copy).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
struct SLOT
{
      SLOT *pslotNext;
};
struct GRABSPOTDATA
{
      TN *ptn;
      undefined4 unk_0x04;
      float sUnk08;
      undefined4 unk_0x0c;
      int nPriority;
      int nUnk14;
      int nGroup;
      uint grfUnk1c;
};
struct GRABSPOT : public GRABSPOTDATA
{
};
struct CELL
{
      qword plane;
      CELL *pcellNeg;
      CELL *pcellPos;
      int ileafNeg;
      int ileafPos;
};
struct LANGSTR
{
      int id;
      char *pchz;
};
struct SVN
{
      int nHash;
      char *pchzName;
      int id;
      SVN *psvnNext;
};
struct SVPATH
{
      int cid;
      short *aid;
};
struct LOUPD
{
      int grf;
      LO *plo;
};
extern SW D_003DD490;
struct PROXYSPARE
{
      struct PROXY *pproxy;
      PROXYSPARE *pspareNext;
      int fLocked;
      undefined4 unk_0xc;
      MATRIX4 mat;
};
struct PROXYREST
{
      qword pos;
      MATRIX3 mat;
};
struct PROXYSRC
{
      int cplo;
      LO **aplo;
      int ccopy;
      int icopyNext;
      int id;
      int nUnk14;
      OID *aoid;
      char achzName[0x40];
      int nUnk5c;
      int fUnk60;
      PROXYREST *aposRest;
      PROXYSPARE *aspare;
};
int IsrcFromIdProxy(int idProxy);
PXR *FindPxr(PXR *ppxrParent, int cid, int *aid);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
inline PROXYSRC *PproxysrcFromIdProxy(int idProxy)
{
    return &D_003DD490.aproxysrc[IsrcFromIdProxy(idProxy)];
}
void LO::SetIdProxy(int idProxy, int fChildren)
{
    int aid[16];
    int cid = 16;
    PXR *ppxrIter;
    for (ppxrIter = ppxr; ppxrIter; ppxrIter = ppxrIter->ppxrParent)
    {
        if (ppxrIter->idProxy == -1)
        {
            break;
        }
        aid[--cid] = ppxrIter->idProxy;
    }
    aid[--cid] = idProxy;
    PXR *ppxrNew = FindPxr(D_003DD490.ppxrRoot, 16 - cid, &aid[cid]);
    for (ppxrIter = ppxr; ppxrIter->idProxy != -1; ppxrIter = ppxrIter->ppxrParent)
    {
        if (ppxrIter->plo == this)
        {
            ppxrIter->plo = 0;
        }
        else
        {
            break;
        }
    }
    for (ppxrIter = ppxrNew; ppxrIter->idProxy != -1; ppxrIter = ppxrIter->ppxrParent)
    {
        if (PproxysrcFromIdProxy(ppxrIter->idProxy)->cplo == 1 && !ppxrIter->plo)
        {
            ppxrIter->plo = this;
        }
        else
        {
            break;
        }
    }
    ppxr = ppxrNew;
    if (fChildren)
    {
        STACKSCOPE scope;
        LO **aplo;
        int cplo = CploChildren(&aplo);
        for (int iplo = 0; iplo < cplo; iplo++)
        {
            aplo[iplo]->SetIdProxy(idProxy, 1);
        }
    }
}
