// func_001C9B08: ported from Lilly's personal Sly 2 decomp (src/P2/proxy.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x001C9B08 LoadProxyFromBrx__FP2SWP2SO3CID3OIDiP18CBinaryInputStreamP14CLoadCollector  (MATCHES Sly 2, T0925)
// Reads one proxy of an object list (LoadSwObjectsFromBrx calls it for class 6 and its subclasses):
//   s32 idSource, s8 mode, s16 ixf, 12 x f32 placement, u32 vis mask, then
//   mode 1 (or -1 when the source has fUnk60): a count of PROXYINIT records (what the proxy changes about
//     its copy later: kind, class, splice frame, option bytes), then a nested object list that is only skipped;
//   otherwise: a count of the same records that are applied now, then the nested object list.
// mode 1 makes ONE object of class cid (a PROXY) that stands at the placement and keeps the source;
// the copy of the source is not taken (its objects stay out of the world until PROXY asks for them).
// Any other mode takes the source's next copy (AddProxyCopy) right now, applies the records to its
// objects (kind 5 / 6 = its first object, else the object found by oid kind below the new PXR node),
// and loads the nested list under the proxy's XF with each of its objects tagged and moved like the copy's.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/sw.h"
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
void SetLoVisMask(LO *plo, uint grfvis);
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    void Read(int cb, void *pv);
    void Skip(int cb);
    uchar U8Read();
    ushort U16Read();
    uint U32Read();
    char S8Read();
    short S16Read();
    int S32Read();
    void ReadMatrix4(MATRIX4 *pmat);
};
XF *PxfFromIxf(int ixf, SO *psoParent, SO *pso);
void PushXf(XF *pxf);
void PopXf(XF *pxf);
void InvalidateXfmWorld(XFM *pxfm);
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
LO *PloNew(CID cid, SO *psoParent, OID oid, int isplice);
void LoadOptionsFromBrx(void *pvStruct, CBinaryInputStream *pbis);
class CLoadCollector
{
public:
    virtual void Add(int cplo, LO **aplo);
};
class CLoadCollectorArray : public CLoadCollector
{
public:
    CLoadCollectorArray();
    int m_unk4;
    int m_cplo;
    LO **m_aplo;
};
void LoadSwObjectsFromBrx(SO *psoParent, CBinaryInputStream *pbis, CLoadCollector *pcollector);
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
struct PROXYINIT
{
      int kind;
      CID cid;
      CFrame *pframe;
      int cb;
      void *pvOptions;
      LO *plo;
};
int IdProxy(int isrc, int icopy);
int FindProxysrc(SW *psw, int idSource);
PXR *FindPxr(PXR *ppxrParent, int cid, int *aid);
void AddProxyCopy(SW *psw, int isrc, SO *psoParent, XF *pxfParent, OID oid, MATRIX4 *pmatProxy, CLoadCollector *pcollector);
int CploFindSwObjects(SW *psw, GRFFSO grffso, OID oid, LO *ploContext, PXR *ppxr, int cploMax, LO **aplo);
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc);
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
void InitStackImpl();
void FreeStackImpl();
CFrame *PframeFromIsplice(SW *psw, int isplice, LO *plo);
inline void *PvAllocSwClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
inline void *PvAllocSw(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
void LoadProxyFromBrx(SW *psw, SO *psoParent, CID cid, OID oid, int isplice, CBinaryInputStream *pbis, CLoadCollector *pcollector) __asm__("func_001C9B08");
void LoadProxyFromBrx(SW *psw, SO *psoParent, CID cid, OID oid, int isplice, CBinaryInputStream *pbis, CLoadCollector *pcollector)
{
    int idSource = pbis->S32Read();
    int mode = pbis->S8Read();
    int ixf = pbis->S16Read();
    XF *pxf = PxfFromIxf(ixf, psoParent, 0);
    MATRIX4 mat;
    pbis->ReadMatrix4(&mat);
    uint grfvis = pbis->U32Read();
    int isrc = FindProxysrc(psw, idSource);
    int idProxy = IdProxy(isrc, psw->aproxysrc[isrc].icopyNext);
    int fOne = 0;
    if (mode == 1 || (mode == -1 && psw->aproxysrc[isrc].fUnk60))
    {
        fOne = 1;
    }
    if (fOne)
    {
        PROXY *pproxy = (PROXY *)PloNew(cid, psoParent, oid, -1);
        pproxy->pxfParent = pxf;
        pproxy->mat = mat;
        InvalidateXfmWorld(pproxy);
        SetLoVisMask(pproxy, grfvis);
        pproxy->psrc = &psw->aproxysrc[isrc];
        if (pproxy->psrc->cplo > 0)
        {
            pproxy->sBusy = pproxy->psrc->aplo[pproxy->psrc->cplo * pproxy->psrc->ccopy - 1]->sBusy;
        }
        pproxy->cinit = pbis->U8Read();
        pproxy->ainit = (PROXYINIT *)PvAllocSwClear(pproxy->cinit * sizeof(PROXYINIT), 0x10);
        for (int iinit = 0; iinit < pproxy->cinit; iinit++)
        {
            PROXYINIT *pinit = &pproxy->ainit[iinit];
            pinit->kind = pbis->S32Read();
            pinit->cid = (CID)pbis->S16Read();
            int isplice = pbis->S16Read();
            if (isplice != -1)
            {
                pinit->pframe = PframeFromIsplice(psw, isplice, 0);
            }
            if (pinit->kind == 5)
            {
                pproxy->pframe = pinit->pframe;
                int cb = pbis->U16Read();
                if (!FIsBasicDerivedFrom(pproxy, pinit->cid))
                {
                    pbis->Skip(cb);
                }
                else
                {
                    LoadOptionsFromBrx(pproxy, pbis);
                }
            }
            else
            {
                pinit->cb = pbis->U16Read();
                pinit->pvOptions = PvAllocSw(pinit->cb, 0x10);
                pbis->Read(pinit->cb, pinit->pvOptions);
            }
        }
        int cbList = pbis->S32Read();
        pbis->S16Read();
        pbis->Skip(cbList - 2);
        if (pcollector)
        {
            pcollector->Add(1, (LO **)&pproxy);
        }
    }
    else
    {
        if (!pcollector)
        {
            InitStackImpl();
        }
        CLoadCollectorArray listCopy;
        AddProxyCopy(psw, isrc, psoParent, pxf, oid, &mat, &listCopy);
        for (int iplo = 0; iplo < listCopy.m_cplo; iplo++)
        {
            LO *plo = listCopy.m_aplo[iplo];
            if (!plo->fUnk58)
            {
                SetLoVisMask(plo, grfvis);
            }
        }
        PXR *ppxr = FindPxr(psw->ppxrRoot, 1, &idProxy);
        int cinit = pbis->U8Read();
        for (int iinit = 0; iinit < cinit; iinit++)
        {
            LO *plo = 0;
            int kind = pbis->S32Read();
            CID cidTarget = (CID)pbis->S16Read();
            if ((uint)(kind - 5) < 2)
            {
                if (listCopy.m_cplo > 0)
                {
                    plo = listCopy.m_aplo[0];
                }
            }
            else
            {
                CploFindSwObjects(psw, (GRFFSO)0xA05, (OID)kind, 0, ppxr, 1, &plo);
            }
            if (plo && !FIsBasicDerivedFrom(plo, cidTarget))
            {
                plo = 0;
            }
            int isplice = pbis->S16Read();
            if (plo && isplice != -1)
            {
                CFrame *pframe = PframeFromIsplice(psw, isplice, plo);
                if (plo->pframe)
                {
                    pframe->AddParent(plo->pframe);
                }
                plo->pframe = pframe;
            }
            int cb = pbis->U16Read();
            if (plo)
            {
                LoadOptionsFromBrx(plo, pbis);
            }
            else
            {
                pbis->Skip(cb);
            }
        }
        CLoadCollectorArray listNested;
        pbis->S32Read();
        PushXf(pxf);
        LoadSwObjectsFromBrx(psoParent, pbis, &listNested);
        PopXf(pxf);
        for (int iplo = 0; iplo < listNested.m_cplo; iplo++)
        {
            LO *plo = listNested.m_aplo[iplo];
            plo->SetIdProxy(idProxy, 1);
            plo->ApplyProxy(&mat);
            if (!plo->fUnk58)
            {
                SetLoVisMask(plo, grfvis);
            }
        }
        if (!pcollector)
        {
            FreeStackImpl();
        }
        else
        {
            pcollector->Add(listCopy.m_cplo, listCopy.m_aplo);
            pcollector->Add(listNested.m_cplo, listNested.m_aplo);
        }
    }
}
