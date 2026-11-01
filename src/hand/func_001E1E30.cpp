// func_001E1E30: ported from Lilly's personal Sly 2 decomp (src/P2/jsg.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A7AD0 (D_002A7AD0), D_002DE570 (D_002DE570), D_003DA530 (D_003DA530), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DEB30 (g_game), D_003DA0C0 (g_hintbox), D_002DFC00 (g_joy), D_002A2DC0 (g_normalZ), D_002A2DD0 (g_qwZero), D_003DD040 (g_sndmgr), D_004435F0 (g_subtitle), D_003DD490 (g_sw)
// 0x001E1E30 NextJsgCommand__FP3JSG  (T1200; was DispatchAsegEvent: it is Sly 1's NextJsgJsge)
// Steps the script to the next command of its block and executes it. First the command's actor is chosen (the oid the command carries, else the
// SetActor default, the block's object or the relay's firer), then the command runs by kind (the table in re/notes/relay-commands-t1289.md), and
// the time it started is noted (the relay's busy bit 0x100 picks the real-time clock). A reference is held on the script meanwhile: a command can
// end it (StopBlock on itself).
typedef unsigned  size_t;
typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
typedef uint16_t    ushort;
typedef uint32_t    uint;
typedef uint64_t    ulong;
typedef uchar   undefined1;
typedef uint    undefined4;
typedef unsigned long long qword __attribute__((mode(TI)));
enum CID
{
};
enum MSGID
{
};
struct LO;
struct ALO;
struct SO;
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
struct RAW8
{
    char ach[8];
};
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
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
    OID_Nil = -1,
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
      virtual void VUnk37(LO *plo);
      virtual void VUnk38();
      virtual void CloneHierarchy(LO *ploBase);
      virtual void Clone(LO *ploBase);
      virtual void VUnk41(float sBusy);
      virtual void *VUnk42(int id);
      virtual void VUnk43(int id);
      virtual void VUnk44(int id);
      virtual void SetVisMask(uint grfvis);
      virtual void VUnk46(int grf);
      virtual void VUnk47(int grf);
};
inline int GrfUpdateLo(LO *plo)
{
    int grf = plo->grfUnk40;
    if (!(grf & 1))
    {
        return *(int *)grf;
    }
    return grf;
}
qword PosLoWorld(LO *plo);
struct SMA;
struct ASEGA;
typedef int GRFAPL;
struct SMPA
{
    float svFast;
    float svSlow;
    float dtFast;
    float sdvMax;
};
SMA *PsmaFindOnAlo(ALO *paloRoot, OID oidSm);
int FIsSmaRetracted(SMA *psma);
OID GetSmaCur(SMA *psma);
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
enum SFXID
{
};
struct AMB
{
    SFXID sfxid;
    int iSerial;
    void *bnk;
    AMB **ppamb;
    float tStarted;
    float uVolAtSource;
    uint sfxh;
    undefined4 unk_1;
    VECTOR pos;
    ALO *palo;
    float sStart;
    float sFull;
    int fContinuous;
    int fOutOfRange;
    int fAttached;
    int fStopped;
    float volAttenuated;
    float pan;
    float frq;
    float rDoppler;
    int dnDoppler;
    int fPitchOrig;
    int ptchOrig;
    float tNext;
    LM lmRepeat;
    LM lmRepDist;
    undefined4 unk_2;
    undefined4 unk_3;
    undefined4 unk_4;
};
struct EXC
{
    int iexc;
    DLE dle;
};
struct SND
{
      undefined4 unk_0x00[3];
};
struct SNDMGR;
struct GAME
{
      float tIdleEvent;
      float tCmTurned;
      int nTaskStateDenied;
      int idGadgetLast;
      int oidJobCur;
      int oidTaskCur;
      int oidTaskPending;
};
extern GAME D_002DEB30;
struct TASKPAIR
{
    OID oidTask;
    int nState;
};
void SetTaskStates(GAME *pgame, int ctaskpair, TASKPAIR *ataskpair, LO *ploContext);
typedef ulong TICK;
enum CRK
{
    CRK_Max = 9
};
struct CLOCK
{
      float t;
      float dt;
      float rt;
      float rtCm;
      float dtCm;
      float tCm;
      float art[CRK_Max];
      float tReal;
      float dtReal;
      undefined4 unk_0x44;
      TICK tickFrame;
      TICK tickLastRaw;
      int cWrapAround;
      undefined4 unk_0x5c;
};
extern CLOCK D_002DDED0;
void SetClockRate(CLOCK *pclock, CRK crk, float rt);
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
struct GEOM
{
      int cpos;
      VECTOR *apos;
      int csurf;
      SURF *asurf;
      int cedge;
      EDGE *aedge;
};
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
struct BSPC
{
      int cbsp;
      int cbspFull;
      int nUnk08;
      BSP *absp;
};
struct BND;
struct WRBG;
struct GLOB
{
      OID oid;
      XF *pxf;
      MATRIX4 *pmatUnk08;
      union
    {
        uint grf;
        struct
        {
            uint grfglobType : 3;
            uint iUnk0c3 : 5;
            uint unk_0x0d : 2;
            uint fLimit : 1;
            uint fHide : 1;
            uint fBakedMat : 1;
            uint iUnkFirst : 9;
            uint iUnkEnd : 9;
            uint unk_0x1f : 1;
        };
    };
      void *pvUnk10;
      void *pvUnk14;
      BND *abnd;
      WRBG *pwrbg;
      struct SAA *psaa;
      struct POSE *ppose;
};
struct SUBGLOB
{
      char bUnk00;
      char cbnd;
      int bUnk02 : 8;
      char type;
      float gUnk04;
      void *pvLit;
      int iqwBone;
      int cqw;
};
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct POSE
{
      VECTOR pos;
      float gUnk0c;
      float gUnk10;
      long nKind : 4;
    ulong ucUnk : 8;
    ulong ibUnk : 6;
    ulong fHide : 1;
    long iglob : 13;
      void *pvUnk18;
      undefined4 unk_0x1c;
};
struct POSESET
{
    POSE apose[4];
};
struct GLOBSET
{
      SO *pso;
      float tUnk04;
      undefined4 unk_0x08[2];
      VECTOR posCenter;
      undefined4 unk_0x1c;
      float sRadius;
      int cpsaa;
      SAA **apsaa;
      int cnUnk2c;
      int *anUnk30;
      int nUnk34;
      int cglob;
      SUBGLOB *asubglob;
      GLOB *aglob;
      SUBGLOBX *asubglobx;
      POSESET *aposeset;
      int nUnk4c;
      union
    {
        float gUnk50;
        struct
        {
            ulong unk_0x50 : 32;
            ulong fBoundsUnk32 : 1;
            ulong fBoundsUnk33 : 1;
            ulong fUnk34 : 1;
            ulong unk_0x54 : 29;
        };
    };
      WRBG *pwrbgFirst;
      void *pbatchrecsFirst;
};
struct ACTSEG;
struct SGG;
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
struct KEY
{
    static void *operator new(uint cb)
    {
        SWALLOC swalloc;
        swalloc.cb = cb;
        SWALLOC *pswalloc = &swalloc;
        pswalloc->cbAlign = 0x10;
        return PvAllocSwClearImpl(pswalloc);
    }
};
struct CHN
{
      int oid : 20;
      int ixf : 10;
      uint fStripped : 1;
      uint fScalar : 1;
      KEY *pkey;
};
struct OVR
{
      int id;
      OID oid;
      KEY *pkey;
};
struct OVRA
{
      KEY *pkey;
      void *pv;
      LO *plo;
      int id;
      OVRA *povraNext;
};
enum EAK
{
};
struct EASND
{
      int sfxid;
      OID oid;
      int sNear;
      int sFar;
      float gPitch;
      float gUnk14;
      float gUnk18;
};
struct EA17
{
      int n0;
      int n4;
      int n8;
      float gC;
};
struct EA20
{
      int n0;
      int n4;
      float g8;
};
struct EA24
{
      RAW8 a[3];
};
struct EA
{
      float tLocal;
      EAK eak;
    union
    {
          int n8;
          float g8;
          OID oid;
          EASND *pesnd;
          EA17 *pea17;
          EA20 *pea20;
          EA24 *pea24;
    };
    union
    {
          int nC;
          float gC;
    };
};
struct ASEGSTART
{
      float tLocal;
      float svtLocal;
      GRFAPL grfapl;
};
struct ASEG : public LO
{
      DL dlAsega;
      int cchn;
      CHN *achn;
      int covr;
      OVR *aovr;
      int nPriority;
      int nWorldHold;
      float tMax;
      float svtMaster;
      uint32_t   _pad_263    [ 1 ] ;
      CLQ clqMasterSuck;
      LM lmMasterSuck;
      int ceaStart;
      EA *aeaStart;
      int ceaFrame;
      EA *aeaFrame;
      int ceaEnd;
      EA *aeaEnd;
      ASEGSTART *pasegstart;
      uint32_t   _pad_273    [ 3 ] ;
};
struct MRSG
{
    float t;
    float dt;
};
struct MRSGC
{
      int ib;
      int cmrsg;
      MRSG *amrsg;
};
struct ASEGBL : public ASEG
{
      int cbBl;
      int cbl;
      void *abl;
      int cmrsgc;
      MRSGC *amrsgc;
      int iblSolo;
      ASEG *pasegEvents;
      uint32_t   _pad_333    [ 1 ] ;
};
ASEGA *PasegaApplyAseg(ASEG *paseg, ALO *paloAsegRoot, float tLocal, float svtLocal, GRFAPL grfapl);
struct JOY;
enum RUMK
{
};
extern JOY D_002DFC00;
void TriggerJoyRumbleRumk(JOY *pjoy, RUMK rumk, float dt);
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
};
struct WARP : XFM
{
      undefined4 unk_0xe0[(0x16c - 0xe0) / 4];
      int coidAlias;
      OID aoidAlias[4];
};
struct TN;
enum CPP
{
};
struct CPDEFI
{
      int grfcpd;
      undefined4 field1_0x4;
      undefined4 field2_0x8;
      undefined4 field3_0xc;
      VECTOR4 posBase;
      VECTOR4 vBase;
      float radHome;
      float sAdjust;
      float tMoveLast;
      int nTurnAxis;
      VECTOR4 posAdjust;
};
struct CPLCYDATA
{
    void *pvHero;
    float gFovTargetSaved;
    float sNearClipSaved;
};
struct CPLCY : public CPLCYDATA
{
};
struct CPLOOK : public CPLCY
{
      float radPan;
      float radTilt;
      float swPan;
      float swTilt;
      float uZoom;
      float svuZoom;
      float rZoomMax;
      float radFovZoom;
      float radFovWide;
      CRefPtr<SND> psndZoom;
      CRefPtr<SND> psndUnk38;
      LM lmPan;
      LM lmTilt;
      float radPanStart;
      float radTiltStart;
      int cEnter;
};
struct CPZOOM : public CPLCY
{
      LO *ploOwner;
      LO *ploFocus;
      float uZoomTarget;
      uint32_t   _pad_224    [ 1 ] ;
      VECTOR4 posFocusSmooth;
      VECTOR4 posEyeSmooth;
      int fPose;
      uint32_t   _pad_228    [ 3 ] ;
      VECTOR4 pos;
      VECTOR4 eul;
      float radFov;
      uint32_t   _pad_232    [ 3 ] ;
      VECTOR4 posTarget;
      VECTOR4 eulTarget;
      float radFovTarget;
      float radPan;
      float radTilt;
      float uZoom;
      float svuZoom;
      float rZoomMax;
      float radFovZoom;
      float radFovWide;
      CRefPtr<SND> psndZoom;
      CRefPtr<SND> psndUnkc4;
      uint32_t   _pad_245    [ 2 ] ;
    void Reset();
    void SetOwner(LO *plo);
    void SetFocus(LO *plo);
    void SetPose(qword pos, qword eul, float radFov);
    void SetZoom(float u);
};
struct CPTURRET : public CPLCY
{
};
struct CID44;
struct CPCAMERA : public CPLCY
{
      CID44 *pcamera;
};
struct CPTWOSHOT : public CPLCY
{
      LO *ploOwner;
      LO *ploHero;
};
struct CAMEVENT
{
      uint32_t   _pad_376    [ 3 ] ;
};
struct CAMPOSE
{
      VECTOR4 pos;
      VECTOR4 eul;
      float radFov;
};
struct CAMMOVE
{
      CAMPOSE *pposeFrom;
      CAMPOSE *pposeTo;
      float dt;
      int fCutOnLeave;
      int iCurve;
      CAMEVENT *pevent;
      int nUnk18;
};
struct CPCAMBLEND : public CPLCY
{
      CAMMOVE *pcammove;
      uint32_t   _pad_510    [ 4 ] ;   // CM::cpzoom is at 0x520 (game: &g_cm + 0x520 = 0x2DE460)
};
struct ACT1;
struct CPHANG : public CPLCY
{
      XF *pxfCam;
      ACT1 *pact1;
      float tNext;
      uint32_t   _pad_434    [ 1 ] ;
      VECTOR4 posSway;
      VECTOR4 vecVel;
};
struct CPR
{
    CPP cpp;
    CPLCY *pcplcy;
    SO *psoFocus;
    void *pv;
};
struct CMPART1 : public CPLCY
{
};
struct CPD;
struct CMPART2 : public CPLCY
{
      TN *ptn;
      CPD *pcpd;
      float radPan;
      float radTilt;
      float sDist;
      float uFrameY;
      int fRecenter;
      float radPanRecenter;
      float radTiltRecenter;
      int nMode;
      int fEntered;
      int fArrived;
      float dradPan;
      float dradTilt;
      float tManual;
      float radPanCrv;
      float radPanFwd;
      float radTiltFwd;
      float radPanRev;
      float radTiltRev;
      float tMoveLast;
      float uTunnel;
      float dsDist;
      float duFrameY;
      qword posEye;
      float tLastSeen;
      float radPanLook;
      float radTiltLook;
      float dradPanLook;
      float dradTiltLook;
      uint32_t   _pad_510    [ 3 ] ;
};
struct CMBASE : public BASIC
{
};
struct CM : public CMBASE
{
      struct VISZONE *pzonVis;
      uint grfvis;
      float rFovCullRef;
      float rCullScale;
      float fov;
      float fovTarget;
      float dfov;
      float rUnk2c;
      float rUnk30;
      float rUnk34;
      float rUnk38;
      float rAspect;
      float gTanHalfX;
      float gTanHalfY;
      float sNearCorner;
      float sNearClip;
      float sFarClip;
      uint32_t   _pad_558    [ 3 ] ;
      union
    {
        MATRIX4 mat;
        struct
        {
            MATRIX3 matRot;
            VECTOR4 pos;
        };
    };
      float agFrustumXyz[12];
      float agFrustumWideXyz[12];
      MATRIX4 matProj;
      MATRIX4 matViewProj;
      MATRIX4 matViewProjInv;
      qword aplaneFrustum[4];
      qword aplaneFrustumWide[4];
      float sNearFog;
      float sFarFog;
      float uFogMax;
      RGBA rgbaFog;
      float agRecipFit[4];
      SO *psoAdjust;
      uint32_t   _pad_581    [ 3 ] ;
      CPDEFI cpdefi;
      qword dposAdjust;
      VECTOR vAdjust;
      uint32_t   _pad_585    [ 1 ] ;
      qword dposFocus;
      VECTOR vFocus;
      uint32_t   _pad_588    [ 1 ] ;
      int nCut;
      float radCutPan;
      float radCutTilt;
      int cmodeHero;
      CPR acpr[8];
      float tModeStart;
      uint32_t   _pad_595    [ 3 ] ;
      CMPART1 part3a0;
      CMPART2 part3b0;
      CPLOOK cplook;
      CPCAMERA part4a8;
      CPHANG part4c0;
      CPCAMBLEND part500;
      CPZOOM cpzoom;
      CPTURRET part5f0;
      CPTWOSHOT part600;
      float tJolt;
      int nUnk61c;
      float rUnk620;
      float rUnk624;
};
extern CM D_002DDF40;
void JoltCm(CM *pcm, int nUnk61c);
void CutCm(CM *pcm);
struct JT;
struct SW;
struct STEPPLAYER;
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
    virtual int FGetWrapRange(float *pasRange);
    virtual void GetClosestParam(JT *pjt, float *psParam);
};
struct PO;
extern SW D_003DD490;
struct XA
{
      uint32_t   _pad_409    [ 1 ] ;
      SO *pso;
      XA *pxaNextSource;
      XA *pxaNextTarget;
};
struct ASEGA : public BASIC
{
      int cref;
      ASEG *paseg;
      ALO *paloRoot;
      float tLocal;
      float tPrev;
      uint32_t   _pad_35    [ 2 ] ;
      DLE dleAseg;
      ACTSEG *pactsegFirst;
      OVRA *povraFirst;
      GRFAPL grfapl;
      float svt;
      float svtRate;
      float svtMaster;
      float svtSuck;
      int iea;
      int ieaEnd;
      int nWorldHold;
      CRefPtr<SND> psnd;
};
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XP;
struct DZ;
struct DZX;
struct FX;
struct SOTREE;
struct ZPR;
struct SURFX;
typedef int GRFFSO;
struct LVO
{
    VECTOR *mpiposvLocal;
    VECTOR *mpiposdvLocal;
    SURFX *mpisurfsurfx;
};
struct SOPART
{
      XF *pxf;
      SO *pso;
      float gUnk08;
      OID oid;
      LVO *plvo;
      short dhp;
      ulong fUnk48 : 1;
               ulong fUnk49 : 1;
               ulong fHidden : 1;
               ulong fUnk51 : 1;
               ulong unk_0x16 : 12;
      uint grfzpk;
      int nUnk1c;
      MATRIX4 matWorld;
      DLE dle;
      BSPC bspc;
      GEOM geomLocal;
      GEOM geomWorld;
      int nStampGeom;
      int nStampWorld;
};
struct SOPHYS
{
      MATRIX3 momintLocal;
      MATRIX3 momintInvLocal;
      qword qwVelocity;
      qword qwAngVelocity;
      VECTOR dvGravity;
      undefined4 unk_0x8c;
      CONSTR constrTorque;
      CONSTR constrForce;
      float m;
      undefined4 unk_0xd4[2];
      struct REGENT *pregent;
};
struct SO : public LO
{
      int cxf;
      XF *axf;
      XF *pxfUnk58;
      XF *pxfUnk5c;
      XF *pxfRoot;
      DL dlChild;
      LO *ploRemovedFirst;
      undefined4 unk_0x74[3];
      GLOBSET globset;
      int cpart;
      SOPART *apart;
      SOPHYS *psophys;
      SOTREE *ptree;
      qword qwDvContact;
      qword qwDwContact;
      uchar agrfZpk[26];
      undefined1 unk_0x12a[2];
      float uAlpha;
      float duAlpha;
      struct SOBOUNDS *psobounds;
      XA *pxa;
      float gUnk13c;
      int nStampUnk140;
      undefined4 unk_0x144;
      char bUnk148;
      char bUnk149;
      ulong unk_0x14a : 4;
                long nUnk14a : 4;
                ulong nZapMode : 2;
                ulong fIgnoreFixed : 1;
                ulong unk_0x14c : 2;
                ulong fNoJtPlace : 1;
                ulong unk_0x14d : 3;
                ulong fHurtRespawns : 1;
                ulong fTreeRoot : 1;
                ulong fUnk35 : 1;
                ulong fHasIkLimbs : 1;
      virtual void VUnk48();
      virtual void GetCpdefi(CPDEFI *pcpdefi);
      virtual XF *PxfFromIxf(int ixf);
      virtual void CloneXfRef(LO *ploBase, XF **ppxf);
      virtual SOTREE *VUnk52();
      virtual void VUnk53(SO *pso);
      virtual void AdjustNewXp(XP *pxp, int ixpd);
      virtual void VUnk55();
      virtual void VUnk56(XP *pxp, int ixpd);
      virtual void VUnk57();
      virtual void VUnk58();
      virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst);
      virtual int FIgnoreIntersection(SO *psoOther);
      virtual int VUnk61();
      virtual int CpartExtra();
      virtual void InitPhys();
      virtual void VUnk64();
      virtual void VUnk65();
      virtual void VUnk66();
      virtual void VUnk67(DZ *pdz, DZX *pdzx, int grfsg, int ixpd);
      virtual void VUnk68(FX *pfx, qword qwPos, VECTOR4 *pvecDv);
      virtual void VUnk69();
      virtual void VUnk70();
      virtual void VUnk71();
      virtual void VUnk72();
      virtual void ProcessSoZapContacts();
      virtual int FTakeDamage(ZPR *pzpr);
};
void ClearSoXps(SO *pso);
enum ZPK
{
};
struct ZPR
{
      LO *ploSource;
      SOPART *ppart;
      LO *ploUnk08;
      int zpk;
      int dhp;
      undefined4 unk_0x14[3];
      qword qwPos;
      qword qwVel;
      undefined4 unk_0x40;
      uint grfzpr;
      SO *psoHit;
      float tHit;
      DLE dle;
      undefined4 unk_0x58[2];
};
struct POCTL : public BASIC
{
      float dxLeft;
      float dyLeft;
      float radLeft;
      float sLeft;
      float dxRight;
      float dyRight;
      float radRight;
      float sRight;
      uint grfbtn;
      uint32_t   _pad_67    [ 3 ] ;
      MATRIX3 matPanTilt;
      float sLeftHold;
      float sRightHold;
      int grfPressed;
      PO *ppo;
      int nState;
      void *pvUnk84;
      float atPressed[16];
      float atLeftStick[2];
      float atRightStick[2];
      uint32_t   _pad_78    [ 2 ] ;
};
struct PO : public SO
{
      POCTL *pctl;
      uint grfbtnForce;
      undefined4 unk_0x158;
      int nUnk15c;
      undefined4 unk_0x160[2];
      PO *ppoUnk168;
      TN *ptnCam;
      undefined4 unk_0x170;
      void *pvUnk174;
      undefined4 unk_0x178;
      int nUnk17c;
      virtual int FInvulnerable(ZPK zpk);
      virtual int JthsCurrent();
      virtual void OnActive(int fActive);
      virtual void SetCtlState(int nState);
};
struct STEP;
struct BHV : public BASIC
{
      STEP *pstep;
      BHV *pbhvPrev;
      BHV *pbhvNext;
      int fBusy;
      int cBusyAbove;
      undefined4 unk_0x24[3];
      virtual void OnPush();
      virtual void Start();
};
struct BHVSTK
{
      STEP *pstep;
      int cpbhv;
      BHV *apbhv[8];
};
struct BHV166 : public BHV
{
      void *pvUnk30;
      float gUnk34;
      int nUnk38;
      int fHasEnd;
      int nUnk40;
      undefined4 unk_0x44;
      float gUnk48;
      int nMode;
      float tMode;
      void *pshapeUnk54;
      int nUnk58;
      float uUnk5c;
      float uUnk60;
      float sUnk64;
      float sUnk68;
      float sUnk6c;
      undefined4 unk_0x70[4];
      qword posStart;
      undefined4 unk_0x90[4];
      int nUnkA0;
      undefined4 unk_0xa4[3];
      qword posEnd;
};
struct BHV168 : public BHV166
{
      struct POCTL *pctlLeader;
      int nUnkc4;
      float gUnkc8;
      int nUnkcc;
      float gUnkd0;
};
struct BHV193 : public BHV
{
      undefined4 nUnk30;
      float gUnk34;
      int nUnk38;
      float gUnk3c;
      qword qwUnk40;
};
void SetBhv193Goal(BHV193 *pbhv, qword qwPos);
void SetBhv166Path(BHV166 *pbhv, void *pshape, int nUnk38);
struct STEP : public PO
{
      float gUnk180;
      undefined4 unk_0x184[3];
      VECTOR vUnk190;
      undefined4 unk_0x19c;
      float uGait;
      int nUnk1a4;
      float radTarget;
      undefined4 unk_0x1ac;
      MATRIX3 matTarget;
      qword qwUnk1e0;
      float rSpin;
      VECTOR vSpin;
      float rUnk200;
      int nUnk204;
      XP *pxpFloor;
      float tUnk20c;
      float gUnk210;
      float gUnk214;
      qword *pqwUnk218;
      qword *pqwUnk21c;
      float gUnk220;
      undefined4 unk_0x224[3];
      qword qwUnk230;
      qword qwUnk240;
      undefined4 unk_0x250[2];
      SOPART *ppartUnk258;
      undefined4 unk_0x25c;
      float rUnk260;
      int idUnk264;
      SOPART *precUnk268;
      BHVSTK bhvstk;
      void *pvUnk294;
      int nUnk298;
      undefined4 unk_0x29c;
      int acUnk2a0[6];
};
void SetStepRadTarget(STEP *pstep, float radTarget);
void SetStepRadTowardDir(STEP *pstep, qword dpos);
struct BL
{
      float gWeight;
      ASEG *paseg;
};
struct BLGAIT : public BL
{
      char abUnk08[8];
      qword vRoot;
      MRSG amrsgFoot0[2];
      MRSG amrsgFoot1[2];
};
struct GAIT
{
      STEPPLAYER *pstepplayer;
      ASEGBL *pasegbl;
      char abUnk08[8];
      BLGAIT abl[2];
      int sfxidStep;
      float gVolWalk;
      float gVolRun;
      int afFootDown[2];
      char abUnkA4[0xc];
};
struct BLDIR : public BL
{
      char abUnk08[8];
      qword vRoot;
};
struct BLLEAN : public BL
{
      char abUnk08[4];
};
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
struct JTPLACE
{
      LO *plo;
      GRABSPOT *pspot;
      SOPART *ppart;
      float tRemembered;
      qword qwPos;
};
struct JTPLACES
{
      STEPPLAYER *pstepplayer;
      int cplace;
      undefined4 unk_0x08[2];
      JTPLACE aplace[32];
};
struct JTMODE;
struct JTMODEHUD
{
      JTMODE *pjtmode;
      int nState;
      struct WIDGET *pwidgetHud;
      struct WIDGET *pwidgetDefault;
      struct WIDGET *pwidgetCur;
      int *pnUnk14;
      float uZoom;
};
struct JTMODE
{
      STEPPLAYER *pstepplayer;
      int nState;
      float tEnter;
      struct ASEG *pasegStart;
      struct ASEG *pasegEnd;
      struct ASEGA *pasega;
      struct ASEGA *pasegaHold;
      undefined4 unk_0x1c;
      int nKind;
      JTMODEHUD hud;
      STEP *pstepOther;
      struct WIDGET *ploHudOverride;
      undefined4 unk_0x48[2];
};
void StartJtMode(JTMODE *pjtmode, int nKind, STEP *pstepOther, LO *ploHudOverride);
struct JTHANDBL
{
      float uWeight;
      struct ASEG *paseg;
      undefined4 unk_0x08;
      struct SHAPE *pshapeA;
      struct SHAPE *pshapeB;
};
struct JTHANDS
{
      SO *pso;
      struct ASEGA **ppasega;
      JTHANDBL abl[2];
      struct ASEGBL *pasegbl;
      LO *ploHand;
      struct XF *pxf;
      undefined4 unk_0x3c;
      MATRIX4 matChain;
      struct ACT *pact1Euler;
      LO *ploTarget;
      float sParam;
      LO *ploRef;
      int nKind;
      struct ACT *pactPos;
      struct ACT *pactHold;
      struct ACT *pactTurn;
      SO *psoHeld;
      int nMode;
      float tMode;
      undefined4 unk_0xac;
};
enum HFDK
{
};
enum JTFOCK
{
};
struct JTGRAB
{
      STEPPLAYER *pstepplayer;
      float tEnter;
      int nState;
      int nKind;
      undefined4 unk_0x10;
      LO *plo;
      GRABSPOT *pspot;
      float uParam;
      float sParam;
      int fFlip;
      undefined4 unk_0x28;
      HFDK hfdk;
      float tHfdk;
      JTFOCK jtfock;
      undefined4 unk_0x38[(0x40 - 0x38) / 4];
      qword qwPosFocus;
      float uFlattenClamp;
      SOPART *precUnk54;
      SOPART *precUnk58;
      undefined4 unk_0x5c;
      SOPART *precUnk60;
      SOPART *precUnk64;
      struct TN *ptn;
      undefined4 unk_0x6c;
      BLDIR abldirA[8];
      struct ASEGBL *pasegblA;
      undefined4 unk_0x174[(0x180 - 0x174) / 4];
      GAIT gaitA;
      undefined4 unk_0x230[(0x240 - 0x230) / 4];
      GAIT gaitB;
      BLLEAN ablC[3];
      struct ASEGBL *pasegblC;
      float gLean;
      float dgLean;
      undefined4 unk_0x320[(0x330 - 0x320) / 4];
      BLDIR abldirB[8];
      struct ASEGBL *pasegblB;
      undefined4 unk_0x434[(0x440 - 0x434) / 4];
      JTHANDS hands;
      undefined4 unk_0x4f0[(0x4fc - 0x4f0) / 4];
      int nUnk4fc;
      int fUnk500;
      struct PROXY *pproxyMarker;
      struct ASEG *pasegUnk508;
      struct ASEG *pasegPhase0xC;
      undefined4 unk_0x510[(0x520 - 0x510) / 4];
};
struct JTPIPEBL
{
      float gWeight;
      struct ASEG *paseg;
      undefined4 unk_0x08;
      float gSpeed;
};
struct JTPIPE
{
      struct STEPPLAYER *pstepplayer;
      int nState;
      float tState;
      SHAPE *pshape;
      float s;
      float gSpeed;
      int fReversed;
      float gVel;
      int nStick;
      undefined4 unk_0x24[(0x30 - 0x24) / 4];
      qword qwPosHand;
      qword qwUnk40;
      JTPIPEBL abl[2];
      struct ASEGBL *pasegbl;
      struct ASEG *pasegMount;
      struct ASEG *pasegHold;
      struct ASEG *pasegTurn;
};
struct JTAIRBL
{
      float gWeight;
      struct ASEG *paseg;
      undefined4 unk_0x08;
};
struct JTAIR
{
      STEPPLAYER *pstepplayer;
      int nKind;
      int n2;
      struct LO *plo;
      undefined4 unk_0x010[2];
      float tStart;
      int nState;
      float tState;
      int fUnk24;
      int fCanDoubleJump;
      int fUnk2c;
      int fUnk30;
      float gUnk34;
      float gSpeedRatio;
      float zStart;
      int nUnk40;
      float tUnk44;
      float tUnk48;
      float tUnk4c;
      undefined4 unk_0x050[(0xa0 - 0x50) / 4];
      qword qwUnkA0;
      float radSmash;
      float gUnkb4;
      float sSmash;
      struct LO *ploUnk37c;
      struct SO *psoUnkc0;
      struct SO *psoUnkc4;
      struct JTEDGE *pedge;
      undefined4 unk_0x0cc;
      qword posEdge;
      qword normalEdge;
      qword dposEdge;
      int nUnk100;
      int nUnk104;
      float gUnk108;
      float gUnk10c;
      float gUnk110;
      float gUnk114;
      float gUnk118;
      float gUnk11c;
      struct ASEG *apasegJump[5];
      struct ASEG *apasegDoubleJump[5];
      struct ASEG *pasegLaunch;
      struct ASEG *pasegLaunch10;
      undefined4 unk_0x150;
      struct ASEG *pasegEdge;
      struct ASEG *pasegKindE;
      struct ASEG *pasegKindEDouble;
      undefined4 unk_0x160;
      float gUnk164;
      float rGravitySaved;
      struct TN *ptnLaunch;
      undefined4 unk_0x170;
      int fGravityChanged;
      struct ASEG *pasegUnk178;
      JTAIRBL abl[5];
      struct ASEGBL *pasegbl;
      struct TN *ptnUnk1bc;
      float sUnk1c0;
      float sUnk1c4;
      struct ASEG *pasegUnk1c8;
      undefined4 unk_0x1cc;
      JTPLACE placeTarget;
      struct ACT8 *pact8;
      undefined4 unk_0x1f4[(0x200 - 0x1f4) / 4];
};
struct HURTEND
{
};
struct JTHURT
{
      struct STEPPLAYER *pstepplayer;
      int nState;
      undefined4 unk_0x008;
      int nHpMax;
      int *php;
      undefined4 unk_0x014[(0x1c - 0x14) / 4];
      HURTEND hurtend;
      undefined4 unk_0x024[(0x30 - 0x24) / 4];
      ZPR zpr;
      int nUnk90;
      undefined4 unk_0x094[2];
      float gUnk9c;
      undefined4 unk_0x0a0[(0x10c - 0xa0) / 4];
      struct ASEGA *pasegaUnk10c;
};
struct GADGETSLOT
{
      int idGadget;
      int nState;
      float tState;
};
struct JTGOAL
{
      STEPPLAYER *pstepplayer;
      undefined4 unk_0x04[3];
      qword qwGoal;
      float sDist;
      float tCheck;
      int fHold;
      undefined4 unk_0x2c;
};
struct JTTHROW
{
      struct STEPPLAYER *pstepplayer;
      int nState;
      float tState;
      undefined4 unk_0x0c;
      struct LO *ploCarried;
      float radTurnHold;
      float radTurnAim;
      LM lmAim;
      float gUnk24;
      float svAim;
      undefined4 unk_0x2c;
      VECTOR vUnk30;
      undefined4 unk_0x3c;
      JTAIRBL abl[2];
      struct ASEGBL *pasegbl;
      undefined4 unk_0x5c[(0x68 - 0x5c) / 4];
      JTAIRBL ablB[4];
      struct ASEGBL *pasegblB;
      undefined4 unk_0x9c[(0xa4 - 0x9c) / 4];
      JTAIRBL ablC[2];
      struct ASEGBL *pasegblC;
      VECTOR vUnkC0;
      undefined4 unk_0xcc;
      VECTOR vUnkD0;
      undefined4 unk_0xdc;
      qword posHold;
      undefined4 unk_0xf0[(0x100 - 0xf0) / 4];
      qword posBlendFrom;
      qword posBlendTo;
      float tBlend;
      float dtBlend;
      int fBlending;
      struct LO *ploMarker;
      int nUnk130;
      undefined4 unk_0x134[(0x140 - 0x134) / 4];
};
struct STEPPLAYER : public STEP
{
      JTAIR jtair;
      JTGRAB jtgrab;
      JTPLACES jtplaces;
      JTHURT jthurt;
      JTMODE jtmode;
      JTPIPE jtpipe;
      JTGOAL jtgoal;
      uint grfUnk1000;
      undefined4 unk_0x1004[(0x1010 - 0x1004) / 4];
      qword dposUnk1010;
      float gUnk1020;
      struct TN *ptnUnk1024;
      struct SM *psmUnk1028;
      struct SM *psmUnk102c;
      struct ASEGA *pasegaUnk1030;
      struct SMA *psmaUnk1034;
      int nUnk1038;
      float gUnk103c;
      int nUnk1040;
      int fUnk1044;
      int nUnk1048;
      float gUnk104c;
      float gUnk1050;
      undefined4 unk_0x1054[(0x1060 - 0x1054) / 4];
      qword qwUnk1060;
      qword qwUnk1070;
      undefined4 unk_0x1080;
      struct JSG *pjsgScript;
      void *pvUnk1088;
      int nUnk108c;
      LOREF *aplorefUnk1090;
      int cplorefUnk1094;
      float agUnk1098[4];
      SMPA smpaUnk10a8;
      void *precUnk10b8;
      undefined4 unk_0x10bc[(0x10c8 - 0x10bc) / 4];
      int nUnk10c8;
      undefined4 unk_0x10cc[(0x10d4 - 0x10cc) / 4];
      float gUnk10d4;
      float gUnk10d8;
      LO *ploUnk10dc;
      int grfUnk10e0;
      int nUnk10e4;
      LO *ploFightTarget;
      int nInteractMode;
      int nUnk10f0;
      undefined4 unk_0x10f4;
      LO *ploLean;
      int nLeanKind;
      undefined4 unk_0x1100[(0x1120 - 0x1100) / 4];
      RAW8 araw1120[3];
      float agdtIdle[4];
      int nIdleKind;
      float tIdle;
      undefined4 unk_0x1150[(0x1180 - 0x1150) / 4];
      GADGETSLOT agadgetslot[3];
      undefined4 unk_0x11a4[(0x11b0 - 0x11a4) / 4];
      JTTHROW jtthrow;
      int nUnk12f0;
      int cUnk12f4;
      struct EMITTER *pemitterGrab;
      int *pmpCur;
      int mpMax;
      float tGadgetUsed;
      float gMpFrac;
      int nUnk130c;
      struct ASEGA *pasegaUnk1310;
      int nUnk1314;
      int nUnk1318;
      float tUnk131c;
};
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo);
void StepCmdHoldOn(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdPutDown(STEPPLAYER *pstepplayer, LO *ploObj, LO *ploWhere);
void StepCmdHoldOn9(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdCarry(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdPlaceHeld(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdGrab0xC(STEPPLAYER *pstepplayer, ASEG *paseg, LO *plo, int n);
void PlayStepClip(STEPPLAYER *pstepplayer, OID oid, int grfapl, int nState);
void FindShapeClosestPointAll(SHAPE *pshape, qword posFrom, CONSTR *pconstr, qword *pposClosest, VECTOR4 *pnormTangentClosest, float *pu, float *ps);
struct CID100 : public SO
{
      int nState;
      undefined4 unk_0x154;
      int nUnk158;
      int idGadget;
      undefined4 unk_0x160[3];
      LO *ploUnk16c;
      undefined4 unk_0x170[16];
};
enum JTS
{
    JTS_Max = 25
};
enum JTBS
{
    JTBS_Nil = -1,
};
struct CID19;
struct ACT5;
struct XMG
{
      ASEG *paseg;
      int fValid;
      float sSphere;
      undefined4 unk_0x0c;
      qword posExtrem;
      qword posSphere;
      qword posUp;
      qword posDown;
      qword posSphereLow;
      undefined4 unk_0x60[(0x100 - 0x60) / 4];
};
struct JTANIM
{
      ASEGA *pasega;
      int fInputOpen;
      int fLandAlt;
      int nUnk0c;
      int nUnk10;
      float tHang;
      int nUnk18;
      int fHangBack;
      struct XA *pxaHang;
      undefined4 unk_0x24[(0x70 - 0x24) / 4];
      struct SM *psmHang;
      undefined4 unk_0x74[3];
      qword dposHang;
};
struct JTPOCKET
{
      int idGadget;
      PROXY *pproxy;
      CID100 *pcid100;
};
struct JT : public STEPPLAYER
{
      undefined4 unk_0x1320[0xb0 / 4];
      undefined4 unk_0x13d0[0x100 / 4];
      ASEGBL *pasegblUnk14d0;
      ASEG *pasegUnk14d4;
      ASEG *pasegUnk14d8;
      undefined4 unk_0x14dc[(0x1514 - 0x14dc) / 4];
      int nUnk1514;
      ASEG *pasegUnk1518;
      undefined4 unk_0x151c;
      XMG axmgUnk1520[2];
      SOPART *aprecJts[JTS_Max];
      undefined4 unk_0x1784[(0x17d0 - 0x1784) / 4];
      JTHANDS hands;
      int cqwUnk1880;
      undefined4 unk_0x1884[3];
      qword aqwUnk1890[3];
      LO *ploUnk18c0;
      CID19 *phndUnk18c4;
      JTS jts;
      float tUnk18cc;
      JTBS jtbs;
      float tUnk18d4;
      int jtas;
      float tUnk18dc;
      JTANIM janim;
      undefined4 unk_0x1970[0x24 / 4];
      ASEG *pasegHang;
      float gUnk1998;
      float gUnk199c;
      SO *psoUnk19a0;
      SO *psoUnk19a4;
      LO *ploUnk19a8;
      float gUnk19ac;
      float tUnk19b0;
      undefined4 unk_0x19b4[(0x19c0 - 0x19b4) / 4];
      void *precUnk19c0;
      void *precUnk19c4;
      void *precUnk19c8;
      void *precUnk19cc;
      void *precUnk19d0;
      void *precUnk19d4;
      void *precUnk19d8;
      void *precUnk19dc;
      void *precUnk19e0;
      ACT5 *pact5Unk19e4;
      ACT5 *pact5Unk19e8;
      ACT5 *pact5Unk19ec;
      ACT5 *pact5Unk19f0;
      ACT5 *pact5Unk19f4;
      ACT5 *pact5Unk19f8;
      ACT5 *pact5Unk19fc;
      int nUnk1a00;
      undefined4 unk_0x1a04[0x18 / 4];
      ASEGBL *pasegblUnk1a1c;
      undefined4 unk_0x1a20[0x18 / 4];
      ASEGBL *pasegblUnk1a38;
      undefined4 unk_0x1a3c[0x18 / 4];
      ASEGBL *pasegblUnk1a54;
      undefined4 unk_0x1a58[0x18 / 4];
      ASEGBL *pasegblUnk1a70;
      ASEG *pasegUnk1a74;
      ASEG *pasegUnk1a78;
      ASEG *pasegUnk1a7c;
      ASEG *pasegUnk1a80;
      ASEG *pasegUnk1a84;
      SOPART *precUnk1a88;
      SOPART *precUnk1a8c;
      SOPART *precUnk1a90;
      SOPART *precUnk1a94;
      SOPART *precUnk1a98;
      SOPART *precUnk1a9c;
      SO *psoUnk1aa0;
      float tUnk1aa4;
      struct EXPL *pexplDustLand;
      struct EXPL *pexplDustGrab;
      undefined4 unk_0x1ab0[(0x1ac0 - 0x1ab0) / 4];
      undefined4 unk_0x1ac0[0x100 / 4];
      ASEGBL *pasegblUnk1bc0;
      SM *psmUnk1bc4;
      ASEG *pasegPickpocket;
      LO *ploPickpocket;
      LO *ploUnk1bd0;
      int nUnk1bd4;
      undefined4 unk_0x1bd8[(0x1c30 - 0x1bd8) / 4];
      undefined4 unk_0x1c30[(0x1ce0 - 0x1c30) / 4];
      ASEG *apasegSwing[13];
      ASEG *pasegAttackA;
      ASEG *pasegAttackB;
      ASEG *pasegUnk1d1c;
      SO *psoUnk1d20;
      SM *psmUnk1d24;
      SMA *psmaUnk1d28;
      int nUnk1d2c;
      int nUnk1d30;
      ASEG *pasegUnk1d34;
      undefined4 unk_0x1d38[2];
      int nUnk1d40;
      undefined4 unk_0x1d44[(0x1e10 - 0x1d44) / 4];
      undefined4 unk_0x1e10[0x100 / 4];
      ASEGBL *pasegblUnk1f10;
      SM *psmUnk1f14;
      LO *ploUnk1f18;
      undefined4 unk_0x1f1c[(0x1f60 - 0x1f1c) / 4];
      undefined4 unk_0x1f60[0x30 / 4];
      ASEGBL *pasegblUnk1f90;
      float gUnk1f94;
      float gUnk1f98;
      undefined4 unk_0x1f9c[(0x1fb0 - 0x1f9c) / 4];
      JTPOCKET apocket[4];
      int nUnk1fe0;
      int nUnk1fe4;
      ASEG *pasegUnk1fe8;
      undefined4 unk_0x1fec;
      undefined4 unk_0x1ff0;
      undefined4 unk_0x1ff4[(0x2010 - 0x1ff4) / 4];
      PROXY *pproxyUnk2010;
      JTAIRBL ablLean[3];
      ASEGBL *pasegblUnk2038;
      undefined4 unk_0x203c[(0x2044 - 0x203c) / 4];
      CRefPtr<SND> psndGadget;
      undefined4 unk_0x2048;
      struct EXPL *pexplUnk204c;
      ASEG *pasegPouncePowerUp;
      undefined4 unk_0x2054[3];
};
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs);
struct JSG : public BASIC
{
      int cref;
      struct RELAY *prelay;
      LO *ploContext;
      LO *ploBlock;
      LO *ploActorDefault;
      LO *ploActor;
      struct RELAYCMD *prelaycmdFirst;
      struct RELAYCMD *prelaycmdLast;
      struct RELAYCMD *prelaycmdCur;
      float tCommand;
      int fHandsOff;
      int grfHandsOff;
      struct RELAYCMD *prelaycmdJt;
      LO *pjt;
      int fHoldBtn10;
      ASEGA *pasegaCur;
      struct RELAY *prelayWait;
      struct STEP *pstepAseg;
      struct SMA *psmaCur;
      int fSmaReached;
      LO *ploFade;
      LO *ploWidget;
      struct RELAYCMD *prelaycmdLine;
      TN *ptnCur;
      int fHideTn;
      undefined4 unk_0x74;
      float sGoTo;
      float sPipe;
      float tBlendEnd;
      undefined4 unk_0x84[(0x90 - 0x84) / 4];
      int nUnk90;
      undefined4 unk_0x94[(0x9c - 0x94) / 4];
      int fUnk9c;
      struct STEPPLAYER *pstepplayerMode;
      int cstepHold;
      struct STEP *apstepHold[4];
      int anHold5[4];
      int anHold6[4];
      struct STEP *pstepSsgGoal;
      struct SSG *pssgGoal;
      int fRun;
      DLE dleRelay;
      CRefPtr<SND> psnd;
};
struct JSGREF
{
    JSG *pjsg;
    JSGREF(JSG *pjsgHold)
    {
        pjsg = pjsgHold;
        pjsg->VUnk00();
    }
    ~JSGREF()
    {
        pjsg->VUnk01();
    }
};
void SetJsgHandsOff(JSG *pjsg, int fHandsOff, int grfHandsOff);
void GetJsgLocation(JSG *pjsg, LO *plo, qword *ppos, float *prad);
void SetJsgJt(JSG *pjsg, struct RELAYCMD *prelaycmdJt, LO *pjt);
void EndJsgJtMode(JSG *pjsg);
void SetJsgFocus(JSG *pjsg, LO *plo);
void SetJsgTn(JSG *pjsg, TN *ptn);
void UpdateJsg(JSG *pjsg);
LO *PloFindJsgObject(JSG *pjsg, OID oid);
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
struct SENSOR;
enum ALARMS
{
};
struct ALARM : public SO
{
      ALARMS alarms;
    float tAlarms;
    SM *psm;
    SMA *psma;
    float dtReset;
      int calbrks;
      OID aoidAlbrks[4];
      int coidSensors;
      OID aoidSensors[16];
      int cpsensors;
      SENSOR *apsensors[16];
      int coidStepguards;
      OID aoidStepguards[6];
      int calbrksDisabled;
    AMB *pambSiren;
    EXC *pexc;
    int fSilent;
      int crsmg;
      RSMG arsmg[8];
      int ichkDisabled;
};
enum RELAYCK
{
    RELAYCK_BlockBegin = 0x00,
    RELAYCK_BlockEnd = 0x01,
    RELAYCK_StopBlock = 0x02,
    RELAYCK_RaiseAlarm = 0x05,
    RELAYCK_StopRelay = 0x06,
    RELAYCK_SetCtlState = 0x07,
    RELAYCK_SetActor = 0x08,
    RELAYCK_Signal = 0x09,
    RELAYCK_PlayAseg = 0x0A,
    RELAYCK_FireRelay = 0x0B,
    RELAYCK_SetContext = 0x0C,
    RELAYCK_ApplySm = 0x0D,
    RELAYCK_WidgetShow = 0x0E,
    RELAYCK_WidgetHide = 0x0F,
    RELAYCK_SetClockRate = 0x10,
    RELAYCK_SetTaskState = 0x11,
    RELAYCK_HandsOff = 0x12,
    RELAYCK_Label = 0x13,
    RELAYCK_GotoLabel = 0x14,
    RELAYCK_GotoLabelIf = 0x15,
    RELAYCK_TeleportTo = 0x16,
    RELAYCK_CutCamera = 0x17,
    RELAYCK_SetTunnel = 0x18,
    RELAYCK_SetFocus = 0x19,
    RELAYCK_PipeTo = 0x1A,
    RELAYCK_Unk1B = 0x1B,
    RELAYCK_HeroState = 0x1C,
    RELAYCK_StepCmd1D = 0x1D,
    RELAYCK_StepCmd1E = 0x1E,
    RELAYCK_StepCmd1F = 0x1F,
    RELAYCK_StepCmd20 = 0x20,
    RELAYCK_StepCmd21 = 0x21,
    RELAYCK_StepCmd22 = 0x22,
    RELAYCK_Face = 0x23,
    RELAYCK_GoTo = 0x24,
    RELAYCK_RunOn = 0x25,
    RELAYCK_RunOff = 0x26,
    RELAYCK_FollowPath = 0x27,
    RELAYCK_WalkTo = 0x28,
    RELAYCK_StepCmd29 = 0x29,
    RELAYCK_Respawn = 0x2A,
    RELAYCK_LaunchTo = 0x2B,
    RELAYCK_AddObject = 0x2C,
    RELAYCK_RemoveObject = 0x2D,
    RELAYCK_SetPartFlag = 0x2E,
    RELAYCK_ClearPartFlag = 0x2F,
    RELAYCK_ShowPart = 0x30,
    RELAYCK_HidePart = 0x31,
    RELAYCK_Unk32 = 0x32,
    RELAYCK_Rumble = 0x33,
    RELAYCK_Explode = 0x34,
    RELAYCK_Eyes = 0x35,
    RELAYCK_Sound = 0x36,
    RELAYCK_StopSound = 0x37,
    RELAYCK_CamBlendFrom = 0x38,
    RELAYCK_CamBlendTo = 0x39,
    RELAYCK_Unk3A = 0x3A,
    RELAYCK_StartJtMode = 0x3B,
    RELAYCK_EndJtMode = 0x3C,
    RELAYCK_TweenPosObj = 0x3D,
    RELAYCK_TweenAimObj = 0x3E,
    RELAYCK_TweenTarget = 0x3F,
    RELAYCK_TweenTime = 0x40,
    RELAYCK_TweenStart = 0x41,
    RELAYCK_ButtonPrompt = 0x42,
    RELAYCK_Unk43 = 0x43,
    RELAYCK_StopLine = 0x44,
    RELAYCK_Line = 0x45,
    RELAYCK_ClearSubtitle = 0x46,
    RELAYCK_Unk47 = 0x47,
    RELAYCK_Unk48 = 0x48,
    RELAYCK_Unk49 = 0x49,
    RELAYCK_Unk4A = 0x4A,
    RELAYCK_PlayAsegDefault = 0x4B,
    RELAYCK_StartMusic = 0x4C,
    RELAYCK_StopMusic = 0x4D,
    RELAYCK_Unk4F = 0x4F,
    RELAYCK_Unk50 = 0x50,
    RELAYCK_Unk51 = 0x51,
    RELAYCK_HintBox = 0x53,
    RELAYCK_ClearHintBox = 0x54,
    RELAYCK_SetSsgGoal = 0x55,
    RELAYCK_ResetStepGoal = 0x56,
    RELAYCK_Unk57 = 0x57,
    RELAYCK_Unk58 = 0x58,
    RELAYCK_Unk59 = 0x59,
    RELAYCK_Unk5A = 0x5A,
};
struct RELAYCMD
{
      RELAYCK relayck;
      int fAsync;
      undefined4 unk_0x8[2];
      union
    {
        undefined4 an[16];
    struct
    {
        OID oidLabel;
    } blockBegin;
    struct
    {
        OID oidLabel;
    } stopBlock;
    struct
    {
        int coid;
        int aoid[4];
    } waitBlocks;
    struct
    {
        int idAlarm;
        int fForget;
    } waitAlarm;
    struct
    {
        int idAlarm;
        OID oidRelay;
    } raiseAlarm;
    struct
    {
        OID oidRelay;
    } stopRelay;
    struct
    {
        OID oidObj;
    } setCtlState;
    struct
    {
        OID oidActor;
    } setActor;
    struct
    {
        int idSignal;
    } signal;
    struct
    {
        OID oidAseg;
        OID oidRoot;
        int grfapl;
    } playAseg;
    struct
    {
        OID oidRelay;
        OID oidFirer;
    } fireRelay;
    struct
    {
        OID oidContext;
    } setContext;
    struct
    {
        OID oidSm;
        OID oidUnk5;
        OID oidState;
        int fUnk7;
    } applySm;
    struct
    {
        OID oidWidget;
        int idLabel;
    } widgetShow;
    struct
    {
        OID oidWidget;
    } widgetHide;
    struct
    {
        float rate;
    } setClockRate;
    struct
    {
        OID oidTask;
        int nState;
    } setTaskState;
    struct
    {
        int fHandsOff;
        int grfHandsOff;
    } handsOff;
    struct
    {
        OID oidLabel;
    } label;
    struct
    {
        OID oidLabel;
    } gotoLabel;
    struct
    {
        OID oidLabel;
        int nValue;
    } gotoLabelIf;
    struct
    {
        OID oidWhere;
        int fAir;
        OID oidActor;
    } teleportTo;
    struct
    {
        OID oidTunnel;
        OID oidActor;
    } setTunnel;
    struct
    {
        OID oidFocus;
        OID oidActor;
    } setFocus;
    struct
    {
        OID oidTarget;
        float gTolerance;
        OID oidActor;
    } pipeTo;
    struct
    {
        OID oidActor;
    } unk1B;
    struct
    {
        int nUnk4;
        int nState;
        OID oidActor;
    } heroState;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd1D;
    struct
    {
        OID oidA;
        OID oidB;
        OID oidActor;
    } stepCmd1E;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd1F;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd20;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd21;
    struct
    {
        OID oidTarget;
        int nMode;
        int nUnk6;
        OID oidActor;
    } stepCmd22;
    struct
    {
        float dtWait;
        OID oidFace;
        OID oidActor;
    } face;
    struct
    {
        OID oidTarget;
        float uRun;
        float gArrive;
        OID oidActor;
        int idClip;
        int fUnk9;
    } goTo;
    struct
    {
        OID oidPath;
        int nPathMode;
        int nUnk6;
        float gUnk7;
        OID oidActor;
        int idClip;
    } followPath;
    struct
    {
        OID oidTarget;
        float gUnk5;
        int nUnk6;
        float gUnk7;
        OID oidActor;
        int idClip;
    } walkTo;
    struct
    {
        OID oidTarget;
        float gUnk5;
        float gArrive;
        OID oidActor;
    } stepCmd29;
    struct
    {
        OID oidSpot;
        int nUnk5;
        int nUnk6;
        OID oidActor;
    } respawn;
    struct
    {
        OID oidTarget;
        float gUnk5;
        float gUnk6;
        OID oidActor;
    } launchTo;
    struct
    {
        OID oidObj;
        float dtFade;
    } addObject;
    struct
    {
        OID oidObj;
        float dtFade;
    } removeObject;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } setPartFlag;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } clearPartFlag;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } showPart;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } hidePart;
    struct
    {
        int nUnk4;
    } unk32;
    struct
    {
        float dtRumble;
        int rumk;
    } rumble;
    struct
    {
        OID oidA;
        OID oidB;
        OID oidC;
        float dtDelay;
    } explode;
    struct
    {
        OID oidA;
        OID oidB;
        float dtEyes;
    } eyes;
    struct
    {
        int idSnd;
        OID oidA;
        int nUnk6;
        int nUnk7;
        float gUnk8;
        float gUnk9;
        float gUnk10;
    } sound;
    struct
    {
        int idSnd;
    } stopSound;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
        undefined4 unk_0x34[3];
        float dtBlend;
        OID oidAseg;
    } camBlendFrom;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
        undefined4 unk_0x34[3];
        int nUnk16;
        int nUnk17;
    } camBlendTo;
    struct
    {
        int nUnk4;
    } unk3A;
    struct
    {
        OID oidActor;
        OID oidStep;
        OID oidUnk6;
    } startJtMode;
    struct
    {
        OID oidObj;
    } tweenPosObj;
    struct
    {
        OID oidObj;
    } tweenAimObj;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
    } tweenTarget;
    struct
    {
        float gUnk4;
    } tweenTime;
    struct
    {
        int grfbtn;
        OID oidWho;
    } buttonPrompt;
    struct
    {
        int nUnk4;
        OID oidActor;
    } unk43;
    struct
    {
        int idLine;
        int fWait;
    } line;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
    } unk47;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
    } unk48;
    struct
    {
        int nUnk4;
    } unk49;
    struct
    {
        float gUnk4;
        int nUnk5;
        int nUnk6;
        OID oidActor;
    } unk4A;
    struct
    {
        OID oidOther;
        OID oidActor;
    } playAsegDefault;
    struct
    {
        int idMusic;
    } startMusic;
    struct
    {
        int idMusic;
    } stopMusic;
    struct
    {
        float dtWait;
    } waitTimer;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
        int nUnk7;
        int nUnk8;
    } unk4F;
    struct
    {
        OID oidActor;
    } unk50;
    struct
    {
        OID oidActor;
    } unk51;
    struct
    {
        float dtWait;
    } unk52;
    struct
    {
        int idHint;
        float dtHint;
    } hintBox;
    struct
    {
        int idHint;
    } clearHintBox;
    struct
    {
        OID oidStep;
        OID oidSsg;
    } setSsgGoal;
    struct
    {
        OID oidStep;
        OID oidUnk5;
    } resetStepGoal;
    struct
    {
        OID oidActor;
    } unk57;
    struct
    {
        OID oidShip;
        OID oidActor;
    } unk58;
    struct
    {
        OID oidShip;
        OID oidActor;
    } unk59;
    struct
    {
        OID oidShip;
        OID oidActor;
        int nUnk6;
    } unk5A;
    };
};
struct RELAY : public LO
{
      int crelaycmd;
      RELAYCMD *arelaycmd;
      int crelaycmdMax;
      DL dlJsg;
      struct JSG *pjsgSignal;
      LO *ploFirer;
      float tFired;
      int fFiring;
      int fBusy;
      int fAppendEnd;
      int nUnk80;
      int calarm;
      ALARM *apalarm[8];
      undefined4 unk_0xa8[2];
};
LO *PloFindSwObject(SW *psw, GRFFSO grffso, OID oid, LO *ploContext);
void PlayAsegSoundEvent(SW *psw, int sfxid, OID oid, int sNear, int sFar, float gPitch, float gUnk14, float gVolume, LO *plo, LO *ploRoot,
                        ASEG *paseg, CRefPtr<SND> *ppsnd);
void StopAsegSoundEvent(SW *psw, int sfxid, ASEG *paseg, CRefPtr<SND> *ppsnd);
LO *FadeAsegObjectEvent(SW *psw, int fIn, OID oid, float dtFade, LO *ploRoot, ASEG *paseg);
void SetAsegPartFlagEvent(SW *psw, int fClear, OID oidSo, OID oidPart, LO *paloRoot, LO *ploRoot, ASEG *paseg);
void ShowAsegPartEvent(SW *psw, int fShow, OID oidSo, OID oidPart, LO *paloRoot, LO *ploRoot, ASEG *paseg);
void ExplodeAsegEvent(SW *psw, OID oidExpl, OID oidRef, OID oidUnk30, LO *ploRoot, ASEG *paseg, float gScale);
void SetAsegEyesEvent(SW *psw, OID oidSo, OID oidEyes, LO *ploRoot, ASEG *paseg, float g);
struct WIDGETCLIP;
struct WIDGET : public LO
{
      undefined4 unk_0x50[2];
      int nState;
      float tState;
      int cframeState;
      int nStateStamped;
      float tStateStamped;
      undefined4 unk_0x6c;
      int idLabelHold;
      int idLabelIn;
      int idLabelOut;
      int idLabelPlaying;
      ushort cclip;
      WIDGETCLIP *aclip;
      undefined4 unk_0x88;
      undefined4 unk_0x8c[6];
      int nPriority;
      int fHidden;
      int unk_0xac;
      int fFreezeClock;
      DLE dleDraw;
      undefined4 unk_0xbc[0x15];
    virtual void Show(int idLabel);
    virtual void Hide();
};
struct SUBTITLE
{
      int idString;
      RGBA rgba;
      int grfbtnPrompt;
};
extern SUBTITLE D_004435F0;
void ClearSubtitle(SUBTITLE *psubtitle);
void SetStepSsgGoal(STEP *pstep, SSG *pssg);
void ResetStepGoal(STEP *pstep, SSG *pssg);
void EnsureSggAlarm(SGG *psgg, ALARM *palarm);
struct PARTLINK
{
      undefined4 unk_0x00;
      void *precA;
      void *precB;
      void *precC;
      undefined4 unk_0x10[0xD0 / 4];
};
struct MOVEREC
{
      int nUnk00;
      int nState;
      undefined4 unk_0x08[(0x28 - 0x08) / 4];
      float gScale;
      undefined4 unk_0x2c[(0x64 - 0x2c) / 4];
      struct ASEG *pasegUnk64;
      undefined4 unk_0x68[(0xe0 - 0x68) / 4];
      qword qwPosOld;
      undefined4 unk_0xf0[(0x100 - 0xf0) / 4];
      qword qwStart;
      qword qwPosNew;
      float tStart;
      float gRatio;
      int fStarted;
};
struct MURRAY : public STEPPLAYER
{
      undefined4 unk_0x1320[(0x13d0 - 0x1320) / 4];
      int nState;
      undefined4 unk_0x13d4;
      int nUnk13d8;
      undefined4 unk_0x13dc[(0x13e8 - 0x13dc) / 4];
      int nUnk13e8;
      undefined4 unk_0x13ec;
      LO *ploCarried;
      undefined4 unk_0x13f4[(0x1410 - 0x13f4) / 4];
      float gUnk1410;
      float gUnk1414;
      undefined4 unk_0x1418[(0x1430 - 0x1418) / 4];
      LO *ploUnk1430;
      float tUnk1434;
      undefined4 unk_0x1438;
      int nUnk143c;
      undefined4 unk_0x1440[(0x144c - 0x1440) / 4];
      int nUnk144c;
      int nUnk1450;
      undefined4 unk_0x1454;
      int iCarry;
      undefined4 unk_0x145c;
      undefined4 unk_0x1460[0xb0 / 4];
      undefined4 unk_0x1510[(0x1530 - 0x1510) / 4];
      struct SMA *psmaWork;
      int nWork;
      undefined4 unk_0x1538[(0x1540 - 0x1538) / 4];
      undefined4 unk_0x1540[(0x15f0 - 0x1540) / 4];
      void *precUnk15f0;
      void *precUnk15f4;
      void *precUnk15f8;
      void *precUnk15fc;
      undefined4 unk_0x1600[4];
      PARTLINK *ppartlinkUnk1610;
      PARTLINK *ppartlinkUnk1614;
      undefined4 unk_0x1618[(0x1630 - 0x1618) / 4];
      undefined4 unk_0x1630[0xb0 / 4];
      undefined4 unk_0x16e0[(0x16f0 - 0x16e0) / 4];
      BLDIR abldirUnk16f0[8];
      ASEGBL *pasegblUnk17f0;
      int nUnk17f4;
      undefined4 unk_0x17f8[(0x1810 - 0x17f8) / 4];
      int nUnk1810;
      undefined4 unk_0x1814[(0x1830 - 0x1814) / 4];
      qword qwUnk1830;
      int nUnk1840;
      undefined4 unk_0x1844[(0x1850 - 0x1844) / 4];
      undefined4 unk_0x1850[0x40 / 4];
      undefined4 unk_0x1890[(0x190c - 0x1890) / 4];
      struct ASEGA *pasegaUnk190c;
      MOVEREC moverec;
      undefined4 unk_0x1a40[(0x1a58 - 0x1a40) / 4];
      SOPART *aprecState[0x16];
      SOPART *aprecCarry[4];
      undefined4 unk_0x1ac0;
      int nUnk1ac4;
      float tUnk1ac8;
      struct ACT1 *pact1Unk1acc;
      undefined4 unk_0x1ad0[(0x1b00 - 0x1ad0) / 4];
};
struct BTBLEND
{
      JTAIRBL abl[2];
      struct ASEGBL *pasegbl;
};
struct BTBLENDS
{
      struct BENTLEY *pbentley;
      int nUnk04;
      undefined4 unk_0x08[(0x20 - 0x08) / 4];
      qword qwRest;
      float gUnk30;
      undefined4 unk_0x34;
      BTBLEND abtblend[6];
};
struct BENTLEY : public STEPPLAYER
{
      undefined4 unk_0x1320[(0x13d0 - 0x1320) / 4];
      int nState;
      undefined4 unk_0x13d4[(0x13e0 - 0x13d4) / 4];
      BLDIR abldirA[8];
      ASEGBL *pasegblA;
      undefined4 unk_0x14e4[(0x14f0 - 0x14e4) / 4];
      BLDIR abldirB[8];
      ASEGBL *pasegblB;
      undefined4 unk_0x15f4[(0x1600 - 0x15f4) / 4];
      undefined4 unk_0x1600[(0x16b0 - 0x1600) / 4];
      int nUnk16b0;
      JTPOCKET *ppocketCur;
      undefined4 unk_0x16b8;
      int nUnk16bc;
      undefined4 unk_0x16c0[(0x1710 - 0x16c0) / 4];
      BTBLENDS btblends;
      undefined4 unk_0x17f0[(0x1800 - 0x17f0) / 4];
      int nUnk1800;
      undefined4 unk_0x1804[(0x1818 - 0x1804) / 4];
    union
    {
          long lUnk1818;
        struct
        {
              int nUnk1818;
              int nUnk181c;
        };
    };
      undefined4 unk_0x1820;
      JTPOCKET apocket[6];
      undefined4 unk_0x186c;
      int nUnk1870;
      int nUnk1874;
      undefined4 unk_0x1878[(0x1880 - 0x1878) / 4];
};
JSG *PjsgStartBlock(RELAY *prelay, RELAYCMD *prelaycmdBegin, JSG *pjsgParent);
JSG *PjsgFindRelayBlock(RELAY *prelay, OID oidLabel, JSG *pjsgExcept);
void EndJsg(JSG *pjsg);
extern "C" void func_001E51A0(JSG *pjsg, LO *plo, int n5, int n6);
extern "C" void func_001E5280(JSG *pjsg, LO *plo, int n5, int n6);
void StartJsgCamBlend(JSG *pjsg, RELAYCMD *prelaycmdFrom, RELAYCMD *prelaycmdTo, float dt);
void PopJsgCamBlend(JSG *pjsg, int n);
float DtJsgCommand(JSG *pjsg, RELAYCMD *prelaycmd);
extern "C" void func_0020FFF0(SW *psw, OID *aoid, LO *ploContext, int grf, JSG *pjsg, SMA **ppsma);
extern "C" int func_00210B68();
extern "C" void func_001DD820(void *pv);
extern "C" void func_001531A8(LO *plo, int n, int cent, void *aent);
extern "C" void func_001532D8(LO *plo, LO *ploActor, int n5, int n6, float g);
extern "C" void func_001B9C00(MURRAY *pmurray, SMA *psma, int n7, int n6, int n8);
extern "C" void func_001B9880(MURRAY *pmurray);
void ReleaseJsgSma(JSG *pjsg);
extern "C" void func_00220508(MOVEREC *pmr, LO *plo);
void PutSwHeroAtWarp(WARP *pwarp);
MATRIX3 LoadRotateMatrixRad(float rad, qword normal);
GRABSPOT *PspotFromLo(LO *plo);
extern "C" int GuardSpawnerGroundSnap(qword *ppos, int n, void *pfnFilter, float gUp, float gDown);
struct HINTBOX;
void SetJtAnimState(JT *pjt, int jtas);
int ChooseJtGrabTarget(JTGRAB *pjtgrab, uint grf, int fUnk);
struct JSGENT
{
    float g;
    int n;
    undefined4 unk_0x08[(0x30 - 0x08) / 4];
};
extern "C" void RelayFire(RELAY *prelay, LO *ploFirer);
extern "C" void func_001DFEB8(RELAY *prelay);
extern "C" BHV *PbhvPushStep(STEP *pstep, int cid);
extern "C" void EnterJtGrabState(JTGRAB *pjtgrab, int nState, LO *plo, GRABSPOT *pspot);
extern "C" void SetJtGrabPhase(JTGRAB *pjtgrab, int nPhase);
extern "C" void LaunchJtToPos(STEP *pstep, qword posTarget, float radTilt, float uSteer);
extern "C" void RespawnAtSafeSpot(STEPPLAYER *pstepplayer, JTPLACE *pplace);
extern "C" void PlayJtAsegDefault(LO *plo, LO *ploOther, int n);
extern "C" int StartDialogueLine(SNDMGR *psndmgr, int idLine, int fWait);
extern "C" void StartMusic(SNDMGR *psndmgr, int idMusic);
extern "C" void StopMusic(SNDMGR *psndmgr, int idMusic);
extern "C" void SetHintBoxText(HINTBOX *phintbox, int idText, float dt);
extern "C" void ClearIfSlotId(HINTBOX *phintbox, int idText);
extern SNDMGR D_003DD040;
extern HINTBOX D_003DA0C0;
extern char D_003DA530[];
extern LO *D_002DE570;
extern float D_002A7AD0;
extern qword D_002A2DD0;
extern qword D_002A2DC0;
static inline LO *PloJsgActorDefault(JSG *pjsg)
{
    if (pjsg->ploActorDefault)
    {
        return pjsg->ploActorDefault;
    }
    if (pjsg->ploBlock)
    {
        return pjsg->ploBlock;
    }
    return pjsg->prelay->ploFirer;
}
static inline qword QwAddScaledX(qword qw, qword qwDir, VU_FLOAT vuScale)
{
    qword qwSum;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwSum) : "j"(qw), "j"(qwDir), "j"(vuScale.qw));
    return qwSum;
}
void NextJsgCommand(JSG *pjsg) __asm__("func_001E1E30");
/** @sly2-annot addr=0x1E1E30
 * Purpose: Advance the script to the next command and execute it by kind
 * Info: proposed name NextJsgCommand (T1); TU relay
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void NextJsgCommand(JSG *pjsg)
{
    pjsg->prelaycmdCur++;
    if (pjsg->prelaycmdCur > pjsg->prelaycmdLast)
    {
        return;
    }
    JSGREF jsgref(pjsg);
    RELAYCMD *prelaycmd = pjsg->prelaycmdCur;
    OID oidActor;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_TeleportTo:
    case RELAYCK_PipeTo:
    case RELAYCK_HeroState:
    case RELAYCK_StepCmd1E:
    case RELAYCK_Face:
        oidActor = (OID)prelaycmd->an[2];
        break;
    case RELAYCK_FollowPath:
    case RELAYCK_WalkTo:
        oidActor = (OID)prelaycmd->an[4];
        break;
    case RELAYCK_StepCmd22:
    case RELAYCK_GoTo:
    case RELAYCK_StepCmd29:
    case RELAYCK_Respawn:
    case RELAYCK_LaunchTo:
    case RELAYCK_Unk4A:
        oidActor = (OID)prelaycmd->an[3];
        break;
    case RELAYCK_Unk1B:
    case RELAYCK_Unk47:
    case RELAYCK_Unk48:
    case RELAYCK_Unk4F:
    case RELAYCK_Unk50:
    case RELAYCK_Unk51:
    case RELAYCK_Unk57:
        oidActor = (OID)prelaycmd->an[0];
        break;
    case RELAYCK_FireRelay:
    case RELAYCK_SetTunnel:
    case RELAYCK_SetFocus:
    case RELAYCK_StepCmd1D:
    case RELAYCK_StepCmd1F:
    case RELAYCK_StepCmd20:
    case RELAYCK_StepCmd21:
    case RELAYCK_Unk43:
    case RELAYCK_PlayAsegDefault:
    case RELAYCK_Unk58:
    case RELAYCK_Unk59:
    case RELAYCK_Unk5A:
        oidActor = (OID)prelaycmd->an[1];
        break;
    default:
        oidActor = OID_Nil;
        break;
    }
    LO *ploActor;
    if (oidActor == OID_Nil)
    {
        ploActor = PloJsgActorDefault(pjsg);
    }
    else
    {
        ploActor = PloFindJsgObject(pjsg, oidActor);
    }
    pjsg->ploActor = ploActor;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_BlockBegin:
    {
        JSG *pjsgBlock = PjsgStartBlock(pjsg->prelay, prelaycmd, pjsg);
        pjsg->prelaycmdCur = pjsgBlock->prelaycmdLast;
        UpdateJsg(pjsgBlock);
        break;
    }
    case RELAYCK_StopBlock:
    {
        JSG *pjsgStop;
        if (prelaycmd->stopBlock.oidLabel == OID_Nil)
        {
            pjsgStop = pjsg;
        }
        else
        {
            pjsgStop = PjsgFindRelayBlock(pjsg->prelay, prelaycmd->stopBlock.oidLabel, 0);
        }
        if (pjsgStop)
        {
            EndJsg(pjsgStop);
        }
        break;
    }
    case RELAYCK_RaiseAlarm:
    {
        RELAY *prelayAlarm;
        if (prelaycmd->raiseAlarm.oidRelay == OID_Nil)
        {
            prelayAlarm = pjsg->prelay;
        }
        else
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->raiseAlarm.oidRelay);
            prelayAlarm = (plo && FIsBasicDerivedFrom(plo, (CID)114)) ? (RELAY *)plo : 0;
        }
        if (prelayAlarm)
        {
            EnsureSggAlarm((SGG *)prelayAlarm, (ALARM *)prelaycmd->raiseAlarm.idAlarm);
        }
        break;
    }
    case RELAYCK_StopRelay:
    {
        RELAY *prelayStop;
        if (prelaycmd->stopRelay.oidRelay == OID_Nil)
        {
            prelayStop = pjsg->prelay;
        }
        else
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->stopRelay.oidRelay);
            prelayStop = (plo && FIsBasicDerivedFrom(plo, (CID)114)) ? (RELAY *)plo : 0;
        }
        if (prelayStop)
        {
            func_001DFEB8(prelayStop);
        }
        break;
    }
    case RELAYCK_SetCtlState:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->setCtlState.oidObj);
        if (plo && FIsBasicDerivedFrom(plo, (CID)50))
        {
            ((PO *)plo)->SetCtlState(1);
        }
        break;
    }
    case RELAYCK_SetActor:
        pjsg->ploActorDefault = PloFindJsgObject(pjsg, prelaycmd->setActor.oidActor);
        break;
    case RELAYCK_Signal:
    {
        void *apv[1];
        apv[0] = &prelaycmd->signal.idSignal;
        pjsg->prelay->pjsgSignal = pjsg;
        pjsg->prelay->HandleSpliceEvent(4, 0, 1, apv, 0);
        pjsg->prelay->pjsgSignal = 0;
        break;
    }
    case RELAYCK_PlayAseg:
    {
        LO *ploRoot = PloFindJsgObject(pjsg, prelaycmd->playAseg.oidRoot);
        if (ploRoot)
        {
            if (FIsBasicDerivedFrom(ploRoot, (CID)52))
            {
                PlayStepClip((STEPPLAYER *)ploRoot, prelaycmd->playAseg.oidAseg, prelaycmd->playAseg.grfapl | 1, 5);
                if (!prelaycmd->fAsync)
                {
                    pjsg->pstepAseg = (STEP *)ploRoot;
                }
                break;
            }
            if (!FIsBasicDerivedFrom(ploRoot, (CID)48))
            {
                ploRoot = 0;
            }
        }
        LO *ploAseg = PloFindJsgObject(pjsg, prelaycmd->playAseg.oidAseg);
        if (ploAseg && FIsBasicDerivedFrom(ploAseg, (CID)109))
        {
            ASEGA *pasega = PasegaApplyAseg((ASEG *)ploAseg, (ALO *)ploRoot, 0.0f, 1.0f, prelaycmd->playAseg.grfapl);
            if (!prelaycmd->fAsync)
            {
                pjsg->pasegaCur = pasega;
                pasega->VUnk00();
            }
        }
        break;
    }
    case RELAYCK_FireRelay:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->fireRelay.oidRelay);
        if (plo && FIsBasicDerivedFrom(plo, (CID)114))
        {
            RelayFire((RELAY *)plo, ploActor);
            if (!prelaycmd->fAsync)
            {
                pjsg->prelayWait = (RELAY *)plo;
            }
        }
        break;
    }
    case RELAYCK_SetContext:
        pjsg->ploContext = PloFindJsgObject(pjsg, prelaycmd->setContext.oidContext);
        if (!pjsg->ploContext)
        {
            pjsg->ploContext = pjsg->prelay;
        }
        break;
    case RELAYCK_ApplySm:
    {
        OID aoid[4];
        aoid[0] = prelaycmd->applySm.oidSm;
        aoid[1] = prelaycmd->applySm.oidUnk5;
        aoid[2] = prelaycmd->applySm.oidState;
        aoid[3] = OID_Nil;
        int grfSma = prelaycmd->applySm.fUnk7 ? 3 : 1;
        func_0020FFF0(&D_003DD490, aoid, pjsg->ploContext, grfSma, pjsg, &pjsg->psmaCur);
        if (prelaycmd->fAsync)
        {
            ReleaseJsgSma(pjsg);
        }
        else if (pjsg->psmaCur && !pjsg->fSmaReached && !FIsSmaRetracted(pjsg->psmaCur) && GetSmaCur(pjsg->psmaCur) == prelaycmd->applySm.oidState)
        {
            pjsg->fSmaReached = 1;
        }
        break;
    }
    case RELAYCK_WidgetShow:
    case RELAYCK_WidgetHide:
    {
        LO *plo = PloFindSwObject(&D_003DD490, (GRFFSO)0x405, prelaycmd->widgetShow.oidWidget, 0);
        if (plo && FIsBasicDerivedFrom(plo, (CID)126))
        {
            if (prelaycmd->relayck == RELAYCK_WidgetShow)
            {
                ((WIDGET *)plo)->Show(prelaycmd->widgetShow.idLabel);
                if (!prelaycmd->fAsync)
                {
                    pjsg->ploWidget = plo;
                }
            }
            else
            {
                ((WIDGET *)plo)->Hide();
            }
        }
        break;
    }
    case RELAYCK_SetClockRate:
        SetClockRate(&D_002DDED0, (CRK)6, prelaycmd->setClockRate.rate);
        if (prelaycmd->setClockRate.rate == 0.0f)
        {
            RELAY *prelay = pjsg->prelay;
            int grf = prelay->grfUnk40;
            if ((grf ^ 1) & 1)
            {
                *(int *)grf |= 0x100;
            }
            else
            {
                prelay->grfUnk40 = grf | 0x100;
            }
        }
        else
        {
            RELAY *prelay = pjsg->prelay;
            int grf = prelay->grfUnk40;
            if ((grf ^ 1) & 1)
            {
                *(int *)grf &= ~0x100;
            }
            else
            {
                prelay->grfUnk40 = grf & ~0x100;
            }
        }
        break;
    case RELAYCK_SetTaskState:
        SetTaskStates(&D_002DEB30, 1, (TASKPAIR *)&prelaycmd->setTaskState, pjsg->ploContext);
        break;
    case RELAYCK_HandsOff:
        SetJsgHandsOff(pjsg, prelaycmd->handsOff.fHandsOff, prelaycmd->handsOff.grfHandsOff);
        break;
    case RELAYCK_GotoLabel:
    case RELAYCK_GotoLabelIf:
    {
        if (prelaycmd->relayck == RELAYCK_GotoLabelIf && func_00210B68() != prelaycmd->gotoLabelIf.nValue)
        {
            break;
        }
        RELAYCMD *prelaycmdGoto = prelaycmd;
        RELAYCMD *prelaycmdLabel = 0;
        int cBlock = 0;
        for (prelaycmd = pjsg->prelaycmdFirst + 1; prelaycmd < pjsg->prelaycmdLast; prelaycmd++)
        {
            switch (prelaycmd->relayck)
            {
            case RELAYCK_BlockBegin:
                cBlock++;
                break;
            case RELAYCK_BlockEnd:
                cBlock--;
                break;
            case RELAYCK_Label:
                if (cBlock == 0 && prelaycmd->label.oidLabel == prelaycmdGoto->gotoLabel.oidLabel)
                {
                    prelaycmdLabel = prelaycmd;
                    prelaycmd = pjsg->prelaycmdLast;
                }
                break;
            }
        }
        if (prelaycmdLabel)
        {
            pjsg->prelaycmdCur = prelaycmdLabel;
        }
        break;
    }
    case RELAYCK_TeleportTo:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->teleportTo.oidWhere);
        if (plo && FIsBasicDerivedFrom(plo, (CID)41))
        {
            PutSwHeroAtWarp((WARP *)plo);
            break;
        }
        if (!pjsg->ploActor)
        {
            break;
        }
        if (plo && FIsBasicDerivedFrom(plo, (CID)27))
        {
            if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
            {
                STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
                EnterJtGrabState(&pstepplayer->jtgrab, 8, plo, 0);
                SetJtGrabPhase(&pstepplayer->jtgrab, 9);
                SetJsgJt(pjsg, 0, 0);
            }
            break;
        }
        VECTOR4 pos;
        MATRIX3 mat;
        float rad;
        GetJsgLocation(pjsg, plo, (qword *)&pos, &rad);
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)51))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            pstep->TranslateToPos(QwSub(*(qword *)&pos, pstep->qwUnk240));
            SetStepRadTarget(pstep, rad);
        }
        else
        {
            pjsg->ploActor->TranslateToPos(*(qword *)&pos);
        }
        mat = LoadRotateMatrixRad(rad, D_002A2DC0);
        pjsg->ploActor->RotateToMat((MATRIX4 *)&mat);
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            pstepplayer->qwUnk1060 = QwAddScaledX(*(qword *)&pos, D_002A2DC0, VuFloatFromMemWord(pstepplayer->nUnk1048));
            if (!prelaycmd->teleportTo.fAir && pstepplayer->cUnk12f4 == 0)
            {
                SetJtAirJump(&pstepplayer->jtair, 0xB, 0, 0);
                ClearSoXps(pstepplayer);
            }
            SetJsgJt(pjsg, 0, 0);
        }
        pjsg->ploActor->SetVelocity(D_002A2DD0);
        pjsg->ploActor->SetAngularVelocity(D_002A2DD0);
        break;
    }
    case RELAYCK_CutCamera:
        CutCm(&D_002DDF40);
        break;
    case RELAYCK_SetTunnel:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->setTunnel.oidTunnel);
        if (plo && FIsBasicDerivedFrom(plo, (CID)15))
        {
            SetJsgTn(pjsg, (TN *)plo);
        }
        break;
    }
    case RELAYCK_SetFocus:
        SetJsgFocus(pjsg, PloFindJsgObject(pjsg, prelaycmd->setFocus.oidFocus));
        break;
    case RELAYCK_PipeTo:
    {
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtpipe.nState == -1)
            {
                break;
            }
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->pipeTo.oidTarget);
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            FindShapeClosestPointAll(pstepplayer->jtpipe.pshape, *(qword *)&pos, 0, 0, 0, 0, &pjsg->sPipe);
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    }
    case RELAYCK_HeroState:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)53))
        {
            JT *pjt = (JT *)pjsg->ploActor;
            switch (prelaycmd->heroState.nState)
            {
            case 2:
                SetJtAnimState(pjt, 3);
                break;
            case 4:
                SetJtJts(pjt, (JTS)0xD, (JTBS)-1);
                break;
            case 8:
                SetJtJts(pjt, (JTS)9, (JTBS)-1);
                break;
            }
        }
        break;
    case RELAYCK_StepCmd1D:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdHoldOn((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd1D.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd1E:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            LO *ploA = PloFindJsgObject(pjsg, prelaycmd->stepCmd1E.oidA);
            LO *ploB = PloFindJsgObject(pjsg, prelaycmd->stepCmd1E.oidB);
            StepCmdPutDown((STEPPLAYER *)pstep, ploA, ploB);
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd1F:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdHoldOn9((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd1F.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd20:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdCarry((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd20.oidGoal));
        }
        break;
    case RELAYCK_StepCmd21:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdPlaceHeld((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd21.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd22:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            LO *ploTarget = PloFindSwObject(&D_003DD490, (GRFFSO)1, prelaycmd->stepCmd22.oidTarget, pstep);
            LO *ploDir;
            if (prelaycmd->stepCmd22.nMode == 3)
            {
                CM *pcm = &D_002DDF40;
                qword qwCm = *(qword *)&pcm->pos;
                SetStepRadTowardDir(pstep, QwSub(qwCm, PosLoWorld(pstep)));
                ploDir = 0;
            }
            else
            {
                ploDir = PloFindJsgObject(pjsg, (OID)prelaycmd->stepCmd22.nMode);
            }
            StepCmdGrab0xC((STEPPLAYER *)pstep, (ASEG *)ploTarget, ploDir, prelaycmd->stepCmd22.nUnk6);
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_Face:
        if (!PloFindJsgObject(pjsg, prelaycmd->face.oidFace))
        {
            break;
        }
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            SetJsgJt(pjsg, prelaycmd, pjsg->ploActor);
        }
        break;
    case RELAYCK_GoTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (prelaycmd->goTo.idClip != -1)
            {
                PlayStepClip(pstepplayer, (OID)prelaycmd->goTo.idClip, 9, 6);
            }
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->goTo.oidTarget);
            if (plo)
            {
                if (FIsBasicDerivedFrom(plo, (CID)34) || FIsBasicDerivedFrom(plo, (CID)12) || FIsBasicDerivedFrom(plo, (CID)38) ||
                    FIsBasicDerivedFrom(plo, (CID)88) || FIsBasicDerivedFrom(plo, (CID)11) || FIsBasicDerivedFrom(plo, (CID)19))
                {
                    uint grf;
                    if (FIsBasicDerivedFrom(pstepplayer, (CID)53))
                    {
                        grf = 0xFFFF;
                    }
                    else if (FIsBasicDerivedFrom(pstepplayer, (CID)55))
                    {
                        grf = 0xF8FE;
                    }
                    else
                    {
                        grf = 0xF8FA;
                    }
                    ChooseJtGrabTarget(&pstepplayer->jtgrab, grf, (int)plo);
                }
                if (pstepplayer->jtgrab.nKind != -1 && pstepplayer->jtgrab.nState == 3)
                {
                    VECTOR4 pos;
                    SHAPE *pshape = (SHAPE *)pstepplayer->jtgrab.plo;
                    GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
                    FindShapeClosestPointAll(pshape, *(qword *)&pos, 0, 0, 0, &pjsg->sGoTo, 0);
                }
            }
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    case RELAYCK_RunOn:
        pjsg->fRun = 1;
        break;
    case RELAYCK_RunOff:
        pjsg->fRun = 0;
        break;
    case RELAYCK_FollowPath:
    {
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            break;
        }
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->followPath.oidPath);
        if (plo && FIsBasicDerivedFrom(plo, (CID)10))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            if (prelaycmd->followPath.idClip != -1)
            {
                PlayStepClip((STEPPLAYER *)pstep, (OID)prelaycmd->followPath.idClip, 9, 6);
            }
            BHV168 *pbhv = (BHV168 *)PbhvPushStep(pstep, 0xA8);
            pbhv->nUnk40 = pjsg->fRun;
            pbhv->nUnkcc = prelaycmd->followPath.nUnk6;
            pbhv->gUnkd0 = prelaycmd->followPath.gUnk7;
            SetBhv166Path(pbhv, plo, prelaycmd->followPath.nPathMode == 0);
            pbhv->Start();
        }
        break;
    }
    case RELAYCK_WalkTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->walkTo.oidTarget);
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            if (prelaycmd->walkTo.idClip != -1)
            {
                PlayStepClip((STEPPLAYER *)pstep, (OID)prelaycmd->walkTo.idClip, 9, 6);
            }
            BHV193 *pbhv = (BHV193 *)PbhvPushStep(pstep, 0xC1);
            pbhv->nUnk30 = pjsg->fRun;
            pbhv->gUnk34 = prelaycmd->walkTo.gUnk5;
            pbhv->nUnk38 = prelaycmd->walkTo.nUnk6;
            pbhv->gUnk3c = prelaycmd->walkTo.gUnk7;
            SetBhv193Goal(pbhv, *(qword *)&pos);
            pbhv->Start();
        }
        break;
    case RELAYCK_StepCmd29:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtgrab.nKind != -1)
            {
                if (pstepplayer->jtgrab.nState != 8)
                {
                    break;
                }
                SetJsgJt(pjsg, prelaycmd, pstepplayer);
            }
        }
        break;
    case RELAYCK_Respawn:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->respawn.oidSpot);
            JTPLACE place;
            memset(&place, 0, sizeof(JTPLACE));
            if (!plo)
            {
                place.qwPos = QwAdd(PosLoWorld(pstepplayer), pstepplayer->qwUnk240);
            }
            else
            {
                GRABSPOT *pspot = PspotFromLo(plo);
                place.pspot = pspot;
                if (pspot)
                {
                    if (pstepplayer->jtpipe.nState != -1 && (LO *)pstepplayer->jtpipe.pshape == plo)
                    {
                        break;
                    }
                    pspot->GetClosestParam((JT *)pstepplayer, (float *)&place.qwPos);
                    place.plo = plo;
                }
                else
                {
                    GetJsgLocation(pjsg, plo, &place.qwPos, 0);
                    GuardSpawnerGroundSnap(&place.qwPos, 0, 0, 100.0f, -150.0f);
                }
            }
            RespawnAtSafeSpot(pstepplayer, &place);
            if (prelaycmd->respawn.nUnk5 == 1)
            {
                pstepplayer->jtair.fCanDoubleJump = prelaycmd->respawn.nUnk5;
            }
            if (FIsBasicDerivedFrom(pstepplayer, (CID)53))
            {
                ((JT *)pstepplayer)->janim.nUnk10 = prelaycmd->respawn.nUnk6;
            }
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    case RELAYCK_LaunchTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->launchTo.oidTarget);
            STEP *pstep = (STEP *)pjsg->ploActor;
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            LaunchJtToPos(pstep, *(qword *)&pos, prelaycmd->launchTo.gUnk5, prelaycmd->launchTo.gUnk6);
        }
        break;
    case RELAYCK_AddObject:
    case RELAYCK_RemoveObject:
    {
        LO *plo = FadeAsegObjectEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_AddObject, prelaycmd->addObject.oidObj, prelaycmd->addObject.dtFade, pjsg->ploContext,
                                      (ASEG *)pjsg->prelay);
        if (plo && !prelaycmd->fAsync && FIsBasicDerivedFrom(plo, (CID)48))
        {
            pjsg->ploFade = plo;
        }
        break;
    }
    case RELAYCK_SetPartFlag:
    case RELAYCK_ClearPartFlag:
        SetAsegPartFlagEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_SetPartFlag, prelaycmd->setPartFlag.oidAseg, prelaycmd->setPartFlag.oidPart, 0, pjsg->ploContext,
                             (ASEG *)pjsg->prelay);
        break;
    case RELAYCK_ShowPart:
    case RELAYCK_HidePart:
        ShowAsegPartEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_ShowPart, prelaycmd->showPart.oidAseg, prelaycmd->showPart.oidPart, 0, pjsg->ploContext,
                          (ASEG *)pjsg->prelay);
        break;
    case RELAYCK_Unk32:
        JoltCm(&D_002DDF40, prelaycmd->unk32.nUnk4);
        break;
    case RELAYCK_Rumble:
        TriggerJoyRumbleRumk(&D_002DFC00, (RUMK)prelaycmd->rumble.rumk, prelaycmd->rumble.dtRumble);
        break;
    case RELAYCK_Explode:
        ExplodeAsegEvent(&D_003DD490, prelaycmd->explode.oidA, prelaycmd->explode.oidB, prelaycmd->explode.oidC, pjsg->ploContext, (ASEG *)pjsg->prelay,
                         prelaycmd->explode.dtDelay);
        break;
    case RELAYCK_Eyes:
        SetAsegEyesEvent(&D_003DD490, prelaycmd->eyes.oidA, prelaycmd->eyes.oidB, pjsg->ploContext, (ASEG *)pjsg->prelay, prelaycmd->eyes.dtEyes);
        break;
    case RELAYCK_Sound:
        PlayAsegSoundEvent(&D_003DD490, prelaycmd->sound.idSnd, prelaycmd->sound.oidA, prelaycmd->sound.nUnk6, prelaycmd->sound.nUnk7, prelaycmd->sound.gUnk8,
                           prelaycmd->sound.gUnk9, prelaycmd->sound.gUnk10, 0, pjsg->ploContext, (ASEG *)pjsg->prelay, &pjsg->psnd);
        break;
    case RELAYCK_StopSound:
        StopAsegSoundEvent(&D_003DD490, prelaycmd->stopSound.idSnd, (ASEG *)pjsg->prelay, &pjsg->psnd);
        break;
    case RELAYCK_CamBlendFrom:
    {
        float dtBlend;
        int fSum;
        if (prelaycmd->camBlendFrom.dtBlend == 0.0f)
        {
            dtBlend = 0.0f;
            fSum = 1;
        }
        else
        {
            dtBlend = prelaycmd->camBlendFrom.dtBlend;
            fSum = 0;
        }
        RELAYCMD *prelaycmdTo = 0;
        int cBlock = 0;
        for (RELAYCMD *prelaycmdT = prelaycmd + 1; prelaycmdT < pjsg->prelaycmdLast && !prelaycmdTo; prelaycmdT++)
        {
            switch (prelaycmdT->relayck)
            {
            case RELAYCK_BlockBegin:
                cBlock++;
                break;
            case RELAYCK_BlockEnd:
                cBlock--;
                break;
            case RELAYCK_CamBlendTo:
                if (cBlock == 0)
                {
                    prelaycmdTo = prelaycmdT;
                }
                break;
            default:
                if (cBlock == 0 && fSum)
                {
                    dtBlend += DtJsgCommand(pjsg, prelaycmdT);
                }
                break;
            }
        }
        if (prelaycmdTo)
        {
            StartJsgCamBlend(pjsg, prelaycmd, prelaycmdTo, dtBlend);
        }
        break;
    }
    case RELAYCK_CamBlendTo:
        if (pjsg->tBlendEnd != D_002A7AD0)
        {
            break;
        }
        if (prelaycmd->camBlendTo.nUnk16 != 0)
        {
            PopJsgCamBlend(pjsg, prelaycmd->camBlendTo.nUnk17);
        }
        else
        {
            pjsg->fUnk9c = 0;
        }
        break;
    case RELAYCK_Unk3A:
        PopJsgCamBlend(pjsg, prelaycmd->unk3A.nUnk4);
        break;
    case RELAYCK_StartJtMode:
    {
        LO *ploStep = PloFindJsgObject(pjsg, prelaycmd->startJtMode.oidStep);
        if (ploStep)
        {
            FIsBasicDerivedFrom(ploStep, (CID)51);
        }
        LO *ploHud = PloFindSwObject(&D_003DD490, (GRFFSO)0x405, prelaycmd->startJtMode.oidUnk6, 0);
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->startJtMode.oidActor);
        if (plo && FIsBasicDerivedFrom(plo, (CID)52))
        {
            pjsg->pstepplayerMode = (STEPPLAYER *)plo;
            StartJtMode(&((STEPPLAYER *)plo)->jtmode, 1, (STEP *)ploStep, ploHud);
        }
        break;
    }
    case RELAYCK_EndJtMode:
        EndJsgJtMode(pjsg);
        break;
    case RELAYCK_TweenPosObj:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->tweenPosObj.oidObj);
        D_002DDF40.cpzoom.SetOwner(plo);
        break;
    }
    case RELAYCK_TweenAimObj:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->tweenAimObj.oidObj);
        D_002DDF40.cpzoom.SetFocus(plo);
        break;
    }
    case RELAYCK_TweenTarget:
        D_002DDF40.cpzoom.SetPose(prelaycmd->tweenTarget.qwPos, *(qword *)&prelaycmd->tweenTarget.radX, prelaycmd->tweenTarget.gFov);
        break;
    case RELAYCK_TweenTime:
        D_002DDF40.cpzoom.SetZoom(prelaycmd->tweenTime.gUnk4);
        break;
    case RELAYCK_TweenStart:
        D_002DDF40.cpzoom.Reset();
        break;
    case RELAYCK_ButtonPrompt:
        D_004435F0.grfbtnPrompt = prelaycmd->buttonPrompt.grfbtn & 0xF0;
        break;
    case RELAYCK_StopLine:
        func_001DD820(D_003DA530);
        break;
    case RELAYCK_Line:
        StartDialogueLine(&D_003DD040, prelaycmd->line.idLine, prelaycmd->line.fWait);
        pjsg->prelaycmdLine = prelaycmd;
        break;
    case RELAYCK_ClearSubtitle:
        ClearSubtitle(&D_004435F0);
        break;
    case RELAYCK_Unk47:
        if (ploActor)
        {
            func_001E51A0(pjsg, ploActor, prelaycmd->unk47.nUnk5, prelaycmd->unk47.nUnk6);
        }
        break;
    case RELAYCK_Unk48:
        if (ploActor)
        {
            func_001E5280(pjsg, ploActor, prelaycmd->unk48.nUnk5, prelaycmd->unk48.nUnk6);
        }
        break;
    case RELAYCK_Unk49:
        if (D_002DE570)
        {
            JSGENT aent[6];
            uint cent = 0;
            int cBlock = 0;
            for (RELAYCMD *prelaycmdT = prelaycmd + 1; prelaycmdT < pjsg->prelaycmdLast; prelaycmdT++)
            {
                switch (prelaycmdT->relayck)
                {
                case RELAYCK_BlockBegin:
                    cBlock++;
                    break;
                case RELAYCK_BlockEnd:
                    cBlock--;
                    break;
                case RELAYCK_Unk4A:
                    if (cBlock == 0 && cent < 6)
                    {
                        JSGENT *pent = &aent[cent++];
                        pent->g = prelaycmdT->unk4A.gUnk4;
                        pent->n = prelaycmdT->unk4A.nUnk5;
                    }
                    break;
                case RELAYCK_Unk49:
                    if (cBlock == 0)
                    {
                        prelaycmdT = pjsg->prelaycmdLast;
                    }
                    break;
                }
            }
            func_001531A8(D_002DE570, prelaycmd->unk49.nUnk4, cent, aent);
        }
        break;
    case RELAYCK_Unk4A:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)68))
        {
            if (D_002DE570)
            {
                func_001532D8(D_002DE570, pjsg->ploActor, prelaycmd->unk4A.nUnk5, prelaycmd->unk4A.nUnk6, prelaycmd->unk4A.gUnk4);
            }
        }
        break;
    case RELAYCK_PlayAsegDefault:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)68))
        {
            LO *plo = pjsg->ploActor;
            LO *ploOther;
            if (prelaycmd->playAsegDefault.oidOther == OID_Nil)
            {
                ploOther = 0;
            }
            else
            {
                ploOther = PloFindSwObject(&D_003DD490, (GRFFSO)4, prelaycmd->playAsegDefault.oidOther, plo);
            }
            PlayJtAsegDefault(plo, ploOther, 0);
        }
        break;
    case RELAYCK_StartMusic:
        StartMusic(&D_003DD040, prelaycmd->startMusic.idMusic);
        break;
    case RELAYCK_StopMusic:
        StopMusic(&D_003DD040, prelaycmd->stopMusic.idMusic);
        break;
    case RELAYCK_Unk4F:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, (OID)prelaycmd->unk4F.nUnk5);
            if (plo)
            {
                SMA *psma = PsmaFindOnAlo((ALO *)plo, OID_Nil);
                if (psma)
                {
                    func_001B9C00(pmurray, psma, prelaycmd->unk4F.nUnk7, prelaycmd->unk4F.nUnk6, prelaycmd->unk4F.nUnk8);
                }
            }
        }
        break;
    case RELAYCK_Unk50:
    case RELAYCK_Unk51:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)55))
        {
            SetJsgJt(pjsg, prelaycmd, pjsg->ploActor);
        }
        break;
    case RELAYCK_HintBox:
        SetHintBoxText(&D_003DA0C0, prelaycmd->hintBox.idHint, prelaycmd->hintBox.dtHint);
        break;
    case RELAYCK_ClearHintBox:
        ClearIfSlotId(&D_003DA0C0, prelaycmd->clearHintBox.idHint);
        break;
    case RELAYCK_SetSsgGoal:
    {
        STEP *pstep = (STEP *)PloFindJsgObject(pjsg, prelaycmd->setSsgGoal.oidStep);
        SSG *pssg = (SSG *)PloFindJsgObject(pjsg, prelaycmd->setSsgGoal.oidSsg);
        SetStepSsgGoal(pstep, pssg);
        if (!prelaycmd->fAsync)
        {
            pjsg->pstepSsgGoal = pstep;
            pjsg->pssgGoal = pssg;
        }
        break;
    }
    case RELAYCK_ResetStepGoal:
    {
        STEP *pstep = (STEP *)PloFindJsgObject(pjsg, prelaycmd->resetStepGoal.oidStep);
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->resetStepGoal.oidUnk5);
        ResetStepGoal(pstep, (SSG *)plo);
        break;
    }
    case RELAYCK_Unk57:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            func_001B9880((MURRAY *)pjsg->ploActor);
        }
        break;
    case RELAYCK_Unk58:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)55))
        {
            BENTLEY *pbentley = (BENTLEY *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk58.oidShip);
            if (plo && FIsBasicDerivedFrom(plo, (CID)54))
            {
                SetJtAirJump(&pbentley->jtair, (pbentley->jtair.nState == 0x11) ? 3 : 1, 0, 0);
                SetJsgJt(pjsg, prelaycmd, pbentley);
            }
        }
        break;
    case RELAYCK_Unk59:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            break;
        }
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            if (pmurray->moverec.nState < 7)
            {
                if (pmurray->moverec.nState < 4)
                {
                    break;
                }
                LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk59.oidShip);
                if (plo)
                {
                    func_00220508(&pmurray->moverec, plo);
                }
            }
        }
        break;
    case RELAYCK_Unk5A:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            break;
        }
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            if (pmurray->moverec.nState < 7)
            {
                if (pmurray->moverec.nState < 4)
                {
                    break;
                }
                prelaycmd->unk5A.nUnk6 = pmurray->moverec.nUnk00;
                LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk5A.oidShip);
                if (plo)
                {
                    func_00220508(&pmurray->moverec, plo);
                    SetJsgJt(pjsg, prelaycmd, pmurray);
                }
            }
        }
        break;
    }
    if (GrfUpdateLo(pjsg->prelay) & 0x100)
    {
        pjsg->tCommand = D_002DDED0.tReal;
    }
    else
    {
        pjsg->tCommand = D_002DDED0.t;
    }
}
