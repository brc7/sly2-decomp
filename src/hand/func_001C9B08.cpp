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
typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
typedef uint16_t    ushort;
typedef uint32_t    uint;
typedef uint64_t    ulong;
typedef uint    undefined4;
typedef unsigned long long qword __attribute__((mode(TI)));
typedef uchar byte;
enum CID
{
};
enum MSGID
{
};
struct CBinaryInputStream;
struct LO;
struct RGBA
{
    uchar bRed;
    uchar bGreen;
    uchar bBlue;
    uchar bAlpha;
    inline void operator=(const int rgba)
    {
        bRed = (rgba >> 24) & 0xFF;
        bGreen = (rgba >> 16) & 0xFF;
        bBlue = (rgba >> 8) & 0xFF;
        bAlpha = rgba & 0xFF;
    }
};
struct SO;
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct MATRIX3
{
	float mat[3][4];
} __attribute__((aligned(16)));
enum OID
{
};
struct SL
{
    void *head;
    void *tail;
    int ibSle;
};
struct SLE
{
    void *next;
};
struct DL
{
    void *head;
    void *tail;
    int ibDle;
};
struct DLE
{
    void *next;
    void *prev;
};
struct XF;
template <class T>
class CRefPtr
{
public:
    T *m_p;
    CRefPtr() {}
    CRefPtr(T *p) { Attach(p); }
    CRefPtr(const CRefPtr &ref)
    {
        m_p = ref.m_p;
        if (m_p)
        {
            m_p->m_cref++;
        }
    }
    ~CRefPtr()
    {
        if (m_p)
        {
            m_p->Release();
        }
    }
    void operator=(T *p);
    void operator=(const CRefPtr &ref) { *this = ref.m_p; }
    operator T *() { return m_p; }
    T *operator->() { return m_p; }
};
class CSidebag;
class CFrame;
struct CLASSINFO
{
      CLASSINFO *pclassinfoSuper;
      CID cid;
      CID cidMax;
      int cb;
};
struct BASIC
{
      CRefPtr<CFrame> pframe;
      CRefPtr<CSidebag> psidebag;
      struct REFNODE *prefnodeFirst;
      virtual void VUnk00();
      virtual void VUnk01();
      virtual void HandleSpliceEvent(uint symidEvent, int fPost, int cargs, void **ppvargs, int nUnk);
      virtual void SendMessage(MSGID msgid, void *pv);
      virtual void HandleMessage(MSGID msgid, void *pv);
      virtual CLASSINFO *Pclassinfo();
};
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
template <class T>
class CGcObj
{
public:
      int m_cref;
      uint m_grfGc;
    ~CGcObj() { Unlink(); }
    void Release()
    {
        if (--m_cref == 0)
        {
            delete (T *)this;
        }
    }
};
class CVarb;
class CFrame : public CGcObj<CFrame>
{
public:
      int m_cpframeParent;
      CRefPtr<CFrame> m_apframeParent[4];
      CRefPtr<CVarb> m_pvarb;
      int m_fVisited;
    void AddParent(CFrame *pframeParent);
};
struct LOROOT
{
      LO *plo;
      DLE dle;
      DLE dleBusy;
};
struct LORUN
{
      void *pv;
      LORUN *plorunNext;
};
struct PXR
{
      int idProxy;
      PXR *ppxrParent;
      int oidProxy;
      SLE slePxrOid;
      LO *plo;
      SLE sleSibling;
      SL slChild;
      undefined4 unk_0x24;
};
struct LO : public BASIC
{
      SO *psoParent;
      LOROOT *ploroot;
      OID oid;
      SLE sleOid;
      SLE sleCid;
      DLE dleChild;
      char cpaloFindSwObjects;
      uchar grfvis;
      long nUnk48 : 4;
               long busyk : 4;
               ulong fRemoved : 1;
               ulong fUnk57 : 1;
               ulong fUnk58 : 1;
               ulong fUnk59 : 1;
               ulong fInWorld : 1;
      float gUnk30;
      LORUN *plorunFirst;
      float sBusy;
      PXR *ppxr;
      int grfUnk40;
      undefined4 unk_0x44[3];
      virtual void Init();
      virtual void Add();
      virtual void Remove();
      virtual void SetParent(SO *psoParent, XF *pxfParent, int grf);
      virtual void AddHierarchy();
      virtual void RemoveHierarchy();
      virtual void UpdateLoroot(LO *plo, SO *psoOld);
      virtual void LoadFromBrx(CBinaryInputStream *pbis);
      virtual void VUnk14();
      virtual void VUnk15();
      virtual void VUnk16();
      virtual void UpdateXfWorldHierarchy();
      virtual void RenderSelf();
      virtual void VUnk19();
      virtual void VUnk20(int fOn);
      virtual void Freeze();
      virtual void Unfreeze();
      virtual void VUnk23();
      virtual void SetIdProxy(int idProxy, int fChildren);
      virtual void VUnk25(LO *plo);
      virtual void VUnk26(LO *plo);
      virtual int CploChildren(LO ***paplo);
      virtual void TranslateToPos(qword qwPos);
      virtual void RotateToMat(MATRIX4 *pmatWorld);
      virtual MATRIX4 *PmatWorld();
      virtual XF *PxfLocal(MATRIX4 *pmatLocal);
      virtual qword GetVelocity();
      virtual qword GetAngularVelocity();
      virtual void SetVelocity(qword qwVel);
      virtual void SetAngularVelocity(qword qwW);
      virtual void ApplyProxy(MATRIX4 *pmatProxy);
};
void SetLoVisMask(LO *plo, uint grfvis);
struct CLQ
{
    union
    {
        struct
        {
            float g0, g1, g2, gUnused;
        };
        float ag[4];
    };
};
struct LM
{
	float gMin, gMax;
};
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
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
};
void InvalidateXfmWorld(XFM *pxfm);
struct SLOT
{
      SLOT *pslotNext;
};
struct SLOTHEAP
{
      int cb;
      int c;
      int cbAlign;
      SLOT *pslotFree;
      int cAlloc;
      byte *ab;
};
struct FILEREC
{
      uint aw[16];
};
struct TN;
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
struct JTGRABCAND
{
      LO *plo;
      GRABSPOT *pspot;
      float sDist;
      int nPriority;
      LO *ploUnk10;
      float tUnk14;
      float sParam;
      float dsParam;
      qword qwPos;
};
struct SPLCS;
struct PO;
struct PROXYSRC;
struct CID116;
struct CID17;
struct REGTREE;
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
struct SVNHASH
{
      SVN *apsvn[0x35];
      int csvn;
      SVN *asvn;
};
struct SAVEVARS
{
      int crec;
      byte *arec;
      SVNHASH hash;
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
struct HUDSTACK
{
      struct WIDGET *pwidget;
      float dyVel;
      int fSnap;
};
struct SW : public BASIC
{
      DL dlChild;
      LO *ploRemovedFirst;
      DL dlRoot;
      DL dlBusy;
      SL aslOid[512];
      SL aslPxrOid[512];
      SL aslCid[199];
      SLOTHEAP slotheapLoroot;
      LOROOT *plorootSettled;
      uint grfvisBusy;
      uint32_t   _pad_185    [ 1 ] ;
      VECTOR4 posCmBusy;
      VECTOR4 posHeroBusy;
      void *aUnk39d0;
      int cUnk39d4;
      void *aUnk39d8;
      int cUnk39dc;
      LO **aploNew;
      LO **pploNew;
      SPLCS *psplcs;
      SPLCS *psplcsHead;
      SL slEvent;
      SLOTHEAP slotheapEvent;
      int fUnk3a14;
      SAVEVARS savevars;
      int ctask;
      uint32_t   _pad_202    [ 3 ] ;
      LO *ploJobCur;
      LO *ploTaskCur;
      LO *ploTaskPending;
      LO *ploJobFailed;
      SVPATH *psvpDt;
      SVPATH *psvpCcoin;
      SVPATH *psvpWidCurrent;
      SVPATH *psvpEpisodeCurrent;
      SVPATH *psvpLang;
      SVPATH *psvpMixerMode;
      SVPATH *psvpMusicVolume;
      SVPATH *psvpEffectsVolume;
      SVPATH *psvpUseHeadset;
      SVPATH *psvpScreenCenterX;
      SVPATH *psvpScreenCenterY;
      SVPATH *psvpNoVibration;
      SVPATH *psvpReverseLookAround;
      SVPATH *psvpReverseAim;
      SVPATH *psvpAutosaveWarned;
      SVPATH *apsvpEpisodeDt[9];
      SVPATH *apsvpEpisodePercent[9];
      HUDSTACK ahudstack[4];
      uint32_t   _pad_228    [ 7 ] ;
      int fUnk3bec;
      int fUnk3bf0;
      uint grfUnk3bf4;
      FILEREC filerecDest;
      int nUnk3c38;
      int nUnk3c3c;
      uint32_t   _pad_235    [ 16 ] ;
      int nLoadFilter;
      int fUnk3c84;
      int nUnk3c88;
      int nUnk3c8c;
      SLOTHEAP slotheapCallback;
      SL slCallback;
      SLOTHEAP slotheapRefnode;
      int fAaoxUnsorted;
      int caaox;
      struct AAOX *aaaox[3];
      SLOTHEAP slotheapSotree;
      SLOTHEAP slotheapXplist;
      DL dlSoTreeAll;
      DL dlSoTree;
      DL dlSoTreeQueue1;
      DL dlSoTreeQueue2;
      SLOTHEAP slotheapXp;
      int nStampXf;
      int nUnk3d5c;
      int nStampLights;
      uint32_t   _pad_256    [ 1 ] ;
      int nStimulusSerial;
      int cEventWorld;
      int nUnk3d70;
      int nUnk3d74;
      int cHandsOff;
      int cNoPause;
      int cUnk3d80;
      int cUnk3d84;
      DL dlAsega;
      SLOTHEAP slotheapAsega;
      SLOTHEAP slotheapAct;
      SLOTHEAP slotheapOvra;
      DL dlSma;
      SLOTHEAP slotheapSma;
      RGBA rgbaClear;
      uint32_t   _pad_272    [ 3 ] ;
      VECTOR4 hsvLight;
      float gLightScale;
      float gMidtone;
      VECTOR vecLightRgb;
      float uMidtone;
      int fUnk3e38;
      uint32_t   _pad_279    [ 2 ] ;
      SLOTHEAP slotheapSobounds;
      void *pvUnk3e5c;
      CID116 *pcid116Fade;
      uint32_t   _pad_283    [ 18 ] ;
      SL slPoctl;
      uint cpo;
      PO *apo[16];
      PO *ppoHero;
      PO *ppoOverride;
      int cproxysrc;
      PROXYSRC *aproxysrc;
      PXR *ppxrRoot;
      int isrcLoading;
      int cloupd;
      int cloupdHole;
      int *piloupdCur;
      LOUPD *aloupd;
      DL dlTarget;
      DL dlCid11;
      DL dlCid13;
      DL dlCid39;
      DL dlCid88;
      DL dlCid12;
      DL dlCid34;
      DL dlCid38;
      DL dlCid35;
      DL dlCid40;
      DL dlCid9;
      DL dlCid27;
      DL dlStepguard;
      uint32_t   _pad_310    [ 6 ] ;
      DL dlCid47;
      uint32_t   _pad_312    [ 6 ] ;
      DL dlStimulus;
      SLOTHEAP slotheapStimulus;
      struct STIMK *apstimk[7];
      SLOTHEAP slotheapUnk403c;
      SLOTHEAP slotheapUnk4054;
      uint32_t   _pad_318    [ 1 ] ;
      SLOTHEAP slotheapUnk4070;
      SLOTHEAP slotheapUnk4088;
      uint32_t   _pad_321    [ 6 ] ;
      SLOTHEAP slotheapSsge;
      uint32_t   _pad_323    [ 6 ] ;
      SLOTHEAP slotheapXa;
      uint32_t   _pad_325    [ 6 ] ;
      SLOTHEAP slotheapZprHit;
      DL dlZprHit;
      uint32_t   _pad_328    [ 263 ] ;
      int ccell;
      CELL *acell;
      int cleaf;
      void *aleaf;
      uint32_t   _pad_333    [ 24 ] ;
      CID17 *pcid17Default;
      REGTREE *pregtree;
      CLQ clqGuardScale;
      LM lmGuardScale;
      uint32_t   _pad_338    [ 20 ] ;
      LO *ploUnk4638;
      int cCid37;
      int clangstr;
      LANGSTR *alangstr;
      uint32_t   _pad_343    [ 12 ] ;
      int cjtgrabcand;
      undefined4 unk_0x467c;
      JTGRABCAND ajtgrabcand[4];
      int idHintGrab;
      LO *ploUnk4744;
      int fGuardsFrozen;
      float tGuardsFrozen;
      float tLookLast;
      uint32_t   _pad_352    [ 2 ] ;
      float gUnk475c;
      uint32_t   _pad_354    [ 3 ] ;
      float tHudTimerEnd;
      int csecHudTimerShown;
      uint32_t   _pad_357    [ 1 ] ;
      int cpemitTracked;
      struct EMITTER *apemitTracked[4];
      uint32_t   _pad_360    [ 1 ] ;
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
struct PROXY : public XFM
{
      PROXYSRC *psrc;
      int nUnkE4;
      PROXYSPARE *pspareFirst;
      int cinit;
      PROXYINIT *ainit;
      undefined4 unk_0xf4[3];
};
int IdProxy(int isrc, int icopy);
int FindProxysrc(SW *psw, int idSource);
PXR *FindPxr(PXR *ppxrParent, int cid, int *aid);
void AddProxyCopy(SW *psw, int isrc, SO *psoParent, XF *pxfParent, OID oid, MATRIX4 *pmatProxy, CLoadCollector *pcollector);
typedef int GRFFSO;
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
/** @sly2-annot addr=0x1C9B08
 * Purpose: read one proxy record (source id/mode/placement) and instantiate copy or PROXY object
 * Info: proposed name LoadProxyFromBrx (T1); TU proxy
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r5_1AE130+parse-verified
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/engine/world-sw.md#loadswobjectsfrombrx-0x13fb40-matched,
 *   docs/formats/level-objects.md#3-object-list-and-object-record
 * @end-sly2-annot */
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
