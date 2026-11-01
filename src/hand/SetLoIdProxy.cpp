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
typedef unsigned char    uint8_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
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
struct LO;
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
class CBinaryInputStream;
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
      virtual void SetIdProxy(int idProxy, int fChildren) __asm__("SetLoIdProxy");
      virtual void VUnk25(LO *plo);
      virtual void VUnk26(LO *plo);
      virtual int CploChildren(LO ***paplo);
};
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
