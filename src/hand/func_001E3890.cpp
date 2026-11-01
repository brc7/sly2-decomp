// func_001E3890: ported from Lilly's personal Sly 2 decomp (src/P2/jsg.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BD850 (D_002BD850), D_002C3620 (D_002C3620), D_002C3624 (D_002C3624), D_002C3628 (D_002C3628), D_002C362C (D_002C362C), D_003DA530 (D_003DA530), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DFC00 (g_joy), D_003DD040 (g_sndmgr), D_004435F0 (g_subtitle), D_003DD490 (g_sw)
// 0x001E3890 FIsJsgCommandComplete__FP3JSGP8RELAYCMD  (T1306, our name; MATCH)
// Whether the command the script is on has finished, by kind; UpdateJsg asks it of the current command (a command that is fAsync is not asked) and
// ReleaseJsgJtIfComplete of the one that took the character. A kind with nothing to wait for (and every kind past the table) is complete at once.
// The waits (the table is in re/notes/relay-commands-t1289.md, "T1306"): WaitBlocks / WaitAlarm / FireRelay / ApplySm / WidgetShow / PlayAseg wait for the thing
// the runner started and let go of it when it is done; the character commands (PipeTo, GoTo, FollowPath, WalkTo, StepCmd1D..22 / 29, Respawn, LaunchTo, Unk1B,
// HeroState, Unk50 / 51 / 58 / 59 / 5A) wait for the character (pjt / ploActor) to get there or reach a state; Face / Unk32 / Rumble / HintBox wait their time
// since tCommand; Line / StopLine / Unk52 / WaitTimer / ButtonPrompt wait for the dialogue; the camera ones (CamBlendTo, Tween*) for the camera.
// The result is one variable (the game's s2) assigned in every arm; a command that has several "done at once" exits writes them as `else fComplete = 1`.
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
typedef uchar byte;
enum CID
{
};
enum MSGID
{
};
struct CBinaryInputStream;
struct LO;
struct SAA;
struct ALO;
struct GLOB;
struct SUBGLOB;
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
inline int FIsLoInWorld(LO *plo)
{
    return plo->fInWorld != 0;
}
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
typedef int GRFAPL;
struct SMPA
{
    float svFast;
    float svSlow;
    float dtFast;
    float sdvMax;
};
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
int FFloatsNear(float g1, float g2, float gEpsilon);
float GLimitLm(LM *plm, float g);
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
struct SOPART;
struct WRBG;
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
enum RUMS
{
};
struct RUMINS
{
      byte fHighSpeedMotor;
      byte bLowSpeedMotor;
      byte unk1;
      byte unk2;
      float dt;
};
struct RUMPAT
{
      int crumins;
      RUMINS arumins[32];
};
struct RUMBLE
{
      int nPort;
      int nSlot;
      RUMS rums;
      RUMPAT *prumpat;
      int irumins;
      float dtRumble;
      float dtRumins;
};
enum JOYS
{
};
enum JOYK
{
};
struct JOY
{
      byte abPadDma[0x1000];
      int nPort;
      int nSlot;
      int term;
      JOYS joys;
      JOYK joyk;
      float tJoys;
      float tRead;
      float tActive;
      int dtIdle;
      undefined4 unk_0x1024;
      undefined4 unk_0x1028;
      undefined4 unk_0x102c[4];
      float dxLeft;
      float dyLeft;
      float uDeflectLeft;
      byte bXLeft;
      byte bYLeft;
      ushort unk_0x104a;
      int fStickMovedLeft;
      LM almDeflect[4];
      float dxRight;
      float dyRight;
      float sRight;
      byte bXRight;
      byte bYRight;
      ushort unk_0x107e;
      int fStickMovedRight;
      LM almDeflect2[4];
      float dxdtLeft;
      float dydtLeft;
      float dxdtRight;
      float dydtRight;
      uint grfbtn;
      uint grfbtnPressed;
      uint grfbtnLatched;
      byte mpbtnpb[12];
      RUMBLE rumble;
      int fRumbleEnabled;
      float uDpad;
      undefined4 unk_0x10f0[4];
};
extern JOY D_002DFC00;
void LatchButtonEdges(JOY *pjoy, uint grfbtn);
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
struct FILEREC
{
      uint aw[16];
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
    int FAtTargetFocus();
    int FAtTargetEye();
    int FAtTargetZoom();
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
int FAsegaStopped(ASEGA *pasega);
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
    int FConsumePressed(uint grf);
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
      virtual int FCanBeHealed();
      virtual void AddHpMp(int dhp, int dmp);
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
struct BHV193 : public BHV
{
      undefined4 nUnk30;
      float gUnk34;
      int nUnk38;
      float gUnk3c;
      qword qwUnk40;
};
int FBhv193AtGoal(BHV193 *pbhv);
int FBhv166NearEnd(BHV166 *pbhv);
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
      virtual void VUnk83(uint grf, int fCount);
      virtual int VUnk84();
      virtual int VUnk85(ZPR *pzpr);
      virtual void VUnk86(int fOn);
};
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
      virtual void VUnk81(int grf, LO *plo, qword qw1, qword qw2, qword qw3);
      virtual void VUnk82(uint grf, int fCount);
    virtual void VUnk87(int n);
    virtual int VUnk88();
};
void GetJtPipeLimits(JTPIPE *pjtpipe, float *pagLimit);
int FStepHoldArrived(STEPPLAYER *pstepplayer);
int FStepPutDownDone(STEPPLAYER *pstepplayer);
int FStepHoldOn9Done(STEPPLAYER *pstepplayer);
int FStepGrab0xCDone(STEPPLAYER *pstepplayer);
int FStepClipPlaying(STEPPLAYER *pstepplayer);
void StopStepClip(STEPPLAYER *pstepplayer);
void SetJtGoalPos(JTGOAL *pjtgoal, qword qwPos);
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
    JTS_Air = 2,
    JTS_Max = 25
};
enum JTBS
{
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
void GetJsgLocation(JSG *pjsg, LO *plo, qword *ppos, float *prad);
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLengthXY(qword v)
{
    return GFromVf(QwLengthXY(v));
}
static inline float GYOfQw(qword qw)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(qw));
    return y;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
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
    RELAYCK_WaitBlocks = 0x03,
    RELAYCK_WaitAlarm = 0x04,
    RELAYCK_PlayAseg = 0x0A,
    RELAYCK_FireRelay = 0x0B,
    RELAYCK_ApplySm = 0x0D,
    RELAYCK_WidgetShow = 0x0E,
    RELAYCK_PipeTo = 0x1A,
    RELAYCK_Unk1B = 0x1B,
    RELAYCK_HeroState = 0x1C,
    RELAYCK_StepCmd1D = 0x1D,
    RELAYCK_StepCmd1E = 0x1E,
    RELAYCK_StepCmd1F = 0x1F,
    RELAYCK_StepCmd21 = 0x21,
    RELAYCK_StepCmd22 = 0x22,
    RELAYCK_Face = 0x23,
    RELAYCK_GoTo = 0x24,
    RELAYCK_FollowPath = 0x27,
    RELAYCK_WalkTo = 0x28,
    RELAYCK_StepCmd29 = 0x29,
    RELAYCK_Respawn = 0x2A,
    RELAYCK_LaunchTo = 0x2B,
    RELAYCK_AddObject = 0x2C,
    RELAYCK_RemoveObject = 0x2D,
    RELAYCK_Unk32 = 0x32,
    RELAYCK_Rumble = 0x33,
    RELAYCK_CamBlendTo = 0x39,
    RELAYCK_StartJtMode = 0x3B,
    RELAYCK_EndJtMode = 0x3C,
    RELAYCK_TweenPosObj = 0x3D,
    RELAYCK_TweenAimObj = 0x3E,
    RELAYCK_TweenTime = 0x40,
    RELAYCK_ButtonPrompt = 0x42,
    RELAYCK_Unk43 = 0x43,
    RELAYCK_StopLine = 0x44,
    RELAYCK_Line = 0x45,
    RELAYCK_Unk4A = 0x4A,
    RELAYCK_WaitTimer = 0x4E,
    RELAYCK_Unk4F = 0x4F,
    RELAYCK_Unk50 = 0x50,
    RELAYCK_Unk51 = 0x51,
    RELAYCK_Unk52 = 0x52,
    RELAYCK_HintBox = 0x53,
    RELAYCK_SetSsgGoal = 0x55,
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
void RemoveRelayAlarm(RELAY *prelay, ALARM *palarm);
int FRelayHasAlarm(RELAY *prelay, ALARM *palarm);
struct SUBTITLE
{
      int idString;
      RGBA rgba;
      int grfbtnPrompt;
};
extern SUBTITLE D_004435F0;
int FStepGoalIs(STEP *pstep, SSG *pssg);
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
static inline float GXOfQwAct(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
JSG *PjsgFindRelayBlock(RELAY *prelay, OID oidLabel, JSG *pjsgExcept);
void PopJsgCamBlend(JSG *pjsg, int n);
void ReleaseJsgSma(JSG *pjsg);
extern SNDMGR D_003DD040;
extern char D_003DA530[];
LO *PloFindJsgObject(JSG *pjsg, OID oid);
extern "C" BHV *PbhvFindStep(STEP *pstep, int cid);
extern "C" int FObjHasTarget(LO *plo, OID oid);
extern "C" int func_002121C8(LO *plo);
extern "C" void func_001E5468(JSG *pjsg);
extern "C" int GrfHandleJoyBtn(JOY *pjoy, uint grfbtn);
extern "C" int func_001D9880(void *pv);
extern "C" int FCurrentIdMatches(SNDMGR *psndmgr, int idLine);
extern "C" float func_001FE888(SNDMGR *psndmgr);
extern "C" float GetFirstTimerSeconds(SNDMGR *psndmgr);
extern "C" void ClearBhvstk(BHVSTK *pbhvstk);
extern float D_002BD850;
extern float D_002C3620;
extern float D_002C3624;
extern float D_002C3628;
extern float D_002C362C;
static inline float TJsgNow(JSG *pjsg)
{
    if (GrfUpdateLo(pjsg->prelay) & 0x100)
    {
        return D_002DDED0.tReal;
    }
    return D_002DDED0.t;
}
int FIsJsgCommandComplete(JSG *pjsg, RELAYCMD *prelaycmd) __asm__("func_001E3890");
int FIsJsgCommandComplete(JSG *pjsg, RELAYCMD *prelaycmd)
{
    int fComplete;
    MURRAY *pmurray;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_WaitBlocks:
    {
        int i;
        fComplete = 1;
        for (i = 0; i < prelaycmd->waitBlocks.coid; i++)
        {
            if (PjsgFindRelayBlock(pjsg->prelay, (OID)prelaycmd->waitBlocks.aoid[i], pjsg))
            {
                fComplete = 0;
                break;
            }
        }
        break;
    }
    case RELAYCK_WaitAlarm:
        fComplete = FRelayHasAlarm(pjsg->prelay, (ALARM *)prelaycmd->waitAlarm.idAlarm);
        if (fComplete && prelaycmd->waitAlarm.fForget)
        {
            RemoveRelayAlarm(pjsg->prelay, (ALARM *)prelaycmd->waitAlarm.idAlarm);
        }
        break;
    case RELAYCK_PlayAseg:
        fComplete = 0;
        if (pjsg->pstepAseg)
        {
            if (!FStepClipPlaying((STEPPLAYER *)pjsg->pstepAseg))
            {
                pjsg->pstepAseg = 0;
                fComplete = 1;
            }
        }
        else
        {
            if (pjsg->pasegaCur && FAsegaStopped(pjsg->pasegaCur))
            {
                pjsg->pasegaCur->VUnk01();
                pjsg->pasegaCur = 0;
            }
            fComplete = pjsg->pasegaCur == 0;
        }
        break;
    case RELAYCK_FireRelay:
        if (pjsg->prelayWait && !pjsg->prelayWait->fFiring)
        {
            pjsg->prelayWait = 0;
        }
        fComplete = pjsg->prelayWait == 0;
        break;
    case RELAYCK_ApplySm:
        if (pjsg->psmaCur && (FIsSmaRetracted(pjsg->psmaCur) || pjsg->fSmaReached))
        {
            ReleaseJsgSma(pjsg);
        }
        fComplete = pjsg->psmaCur == 0;
        break;
    case RELAYCK_WidgetShow:
        if (pjsg->ploWidget && func_002121C8(pjsg->ploWidget))
        {
            func_001E5468(pjsg);
        }
        fComplete = pjsg->ploWidget == 0;
        break;
    case RELAYCK_PipeTo:
        if (pjsg->pjt)
        {
            VECTOR4 vLimits;
            GetJtPipeLimits(&((STEPPLAYER *)pjsg->pjt)->jtpipe, (float *)&vLimits);
            float s = GLimitLm((LM *)&vLimits, pjsg->sPipe);
            fComplete = fabsf(((STEPPLAYER *)pjsg->pjt)->jtpipe.s - s) < prelaycmd->pipeTo.gTolerance;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk1B:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtair.nState >= 0xB && pstepplayer->jtair.nState <= 0xD)
            {
                fComplete = 0;
            }
            else if (pstepplayer->jtgrab.nKind != -1 && (pstepplayer->jtgrab.nState == 0 || pstepplayer->jtgrab.nState == 0xD))
            {
                pstepplayer->VUnk87(0);
                fComplete = 1;
            }
            else
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_HeroState:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x35))
        {
            JT *pjt = (JT *)pjsg->ploActor;
            switch (prelaycmd->heroState.nState)
            {
            case 2:
                fComplete = pjt->jtas == -1;
                break;
            case 4:
                fComplete = pjt->jts != JTS_Air;
                break;
            case 8:
                fComplete = pjt->jts != 9;
                break;
            default:
                fComplete = 1;
                break;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1D:
    case RELAYCK_StepCmd21:
        if (pjsg->pjt)
        {
            fComplete = FStepHoldArrived((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1E:
        if (pjsg->pjt)
        {
            fComplete = FStepPutDownDone((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1F:
        if (pjsg->pjt)
        {
            fComplete = FStepHoldOn9Done((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd22:
        if (pjsg->pjt)
        {
            fComplete = FStepGrab0xCDone((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Face:
        fComplete = prelaycmd->face.dtWait < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_GoTo:
    {
        LO *pjt = pjsg->pjt;
        if (pjt)
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->goTo.oidTarget);
            if (plo)
            {
                qword pos;
                GetJsgLocation(pjsg, plo, &pos, 0);
                fComplete = GLengthXY(QwSub(PosLoWorld(pjt), pos)) < prelaycmd->goTo.gArrive;
                SetJtGoalPos(&((STEPPLAYER *)pjt)->jtgoal, pos);
            }
            else
            {
                fComplete = 1;
            }
            if (fComplete && prelaycmd->goTo.idClip != -1)
            {
                StopStepClip((STEPPLAYER *)pjt);
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    case RELAYCK_FollowPath:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            BHV166 *pbhv = (BHV166 *)PbhvFindStep(pstep, 0xA8);
            fComplete = 0;
            if (!pbhv || FBhv166NearEnd(pbhv))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                ClearBhvstk(&pstep->bhvstk);
                if (prelaycmd->followPath.idClip != -1)
                {
                    StopStepClip((STEPPLAYER *)pstep);
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_WalkTo:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            BHV *pbhv = PbhvFindStep(pstep, 0xC1);
            fComplete = 0;
            if (!pbhv || FBhv193AtGoal((BHV193 *)pbhv))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                ClearBhvstk(&pstep->bhvstk);
                if (prelaycmd->walkTo.idClip != -1)
                {
                    StopStepClip((STEPPLAYER *)pstep);
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd29:
    {
        STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->pjt;
        if (pstepplayer && pstepplayer->jtgrab.nKind != -1 && pstepplayer->jtgrab.nState == 8)
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->stepCmd29.oidTarget);
            if (plo)
            {
                VECTOR4 pos;
                GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
                fComplete = GLengthXY(QwSub(PosLoWorld(pstepplayer), *(qword *)&pos)) < prelaycmd->stepCmd29.gArrive;
            }
            else
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    case RELAYCK_Respawn:
        if (pjsg->pjt)
        {
            fComplete = ((STEPPLAYER *)pjsg->pjt)->jtair.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_LaunchTo:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            fComplete = ((STEPPLAYER *)pjsg->ploActor)->jtair.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_AddObject:
    case RELAYCK_RemoveObject:
        if (pjsg->ploFade)
        {
            if (prelaycmd->relayck == RELAYCK_AddObject)
            {
                if (FIsLoInWorld(pjsg->ploFade) && ((SO *)pjsg->ploFade)->uAlpha == 1.0f)
                {
                    pjsg->ploFade = 0;
                }
            }
            else if (!FIsLoInWorld(pjsg->ploFade))
            {
                pjsg->ploFade = 0;
            }
        }
        fComplete = pjsg->ploFade == 0;
        break;
    case RELAYCK_Unk32:
        fComplete = D_002BD850 < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_Rumble:
        fComplete = prelaycmd->rumble.dtRumble < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_CamBlendTo:
        fComplete = pjsg->tBlendEnd <= D_002DDED0.t;
        if (fComplete)
        {
            if (prelaycmd->camBlendTo.nUnk16 != 0)
            {
                PopJsgCamBlend(pjsg, prelaycmd->camBlendTo.nUnk17);
            }
            else
            {
                pjsg->fUnk9c = 0;
            }
        }
        break;
    case RELAYCK_StartJtMode:
        if (pjsg->pstepplayerMode)
        {
            fComplete = pjsg->pstepplayerMode->jtmode.hud.nState == 1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_EndJtMode:
        if (pjsg->pstepplayerMode)
        {
            fComplete = pjsg->pstepplayerMode->jtmode.hud.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_TweenPosObj:
        fComplete = D_002DDF40.cpzoom.FAtTargetFocus();
        break;
    case RELAYCK_TweenAimObj:
        fComplete = D_002DDF40.cpzoom.FAtTargetEye();
        break;
    case RELAYCK_TweenTime:
        fComplete = D_002DDF40.cpzoom.FAtTargetZoom();
        break;
    case RELAYCK_ButtonPrompt:
        if (prelaycmd->buttonPrompt.oidWho == -1)
        {
            fComplete = GrfHandleJoyBtn(&D_002DFC00, prelaycmd->buttonPrompt.grfbtn);
        }
        else
        {
            POCTL *ppoctl;
            for (ppoctl = (POCTL *)D_003DD490.slPoctl.head; ppoctl; ppoctl = (POCTL *)ppoctl->pvUnk84)
            {
                if (FObjHasTarget(ppoctl->ppo, prelaycmd->buttonPrompt.oidWho))
                {
                    break;
                }
            }
            fComplete = 0;
            if (!ppoctl || ppoctl->FConsumePressed(prelaycmd->buttonPrompt.grfbtn))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                LatchButtonEdges(&D_002DFC00, prelaycmd->buttonPrompt.grfbtn);
            }
        }
        if (fComplete)
        {
            D_004435F0.grfbtnPrompt = 0;
        }
        break;
    case RELAYCK_Unk43:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            fComplete = 0;
            if (((STEPPLAYER *)pjsg->ploActor)->VUnk88() == prelaycmd->unk43.nUnk4)
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StopLine:
        fComplete = func_001D9880(D_003DA530) == 0;
        break;
    case RELAYCK_Line:
        fComplete = FCurrentIdMatches(&D_003DD040, prelaycmd->line.idLine) == 0;
        break;
    case RELAYCK_Unk52:
    {
        int idLine = -1;
        if (pjsg->prelaycmdLine)
        {
            idLine = pjsg->prelaycmdLine->line.idLine;
        }
        if (!FCurrentIdMatches(&D_003DD040, idLine))
        {
            fComplete = 1;
        }
        else if (prelaycmd->unk52.dtWait < 0.0f)
        {
            fComplete = 0;
        }
        else
        {
            fComplete = 0.0f <= func_001FE888(&D_003DD040) - prelaycmd->unk52.dtWait;
        }
        break;
    }
    case RELAYCK_Unk4A:
        fComplete = prelaycmd->unk4A.gUnk4 <= GetFirstTimerSeconds(&D_003DD040);
        break;
    case RELAYCK_WaitTimer:
        fComplete = prelaycmd->waitTimer.dtWait <= GetFirstTimerSeconds(&D_003DD040);
        break;
    case RELAYCK_Unk4F:
        fComplete = 0;
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x36))
        {
            MURRAY *pmurrayActor = (MURRAY *)pjsg->ploActor;
            if (pmurrayActor->nState != 6)
            {
                fComplete = 1;
            }
            else if (prelaycmd->unk4F.nUnk7 != -1)
            {
                fComplete = GetSmaCur(pmurrayActor->psmaUnk1034) == prelaycmd->unk4F.nUnk7;
            }
        }
        break;
    case RELAYCK_Unk50:
        fComplete = 0;
        if (!pjsg->pjt || ((BENTLEY *)pjsg->pjt)->nState == 6)
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk51:
        fComplete = 0;
        if (!pjsg->pjt || ((BENTLEY *)pjsg->pjt)->nState != 6)
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_HintBox:
        fComplete = prelaycmd->hintBox.dtHint < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_SetSsgGoal:
        if (pjsg->pssgGoal && !FStepGoalIs(pjsg->pstepSsgGoal, pjsg->pssgGoal))
        {
            pjsg->pstepSsgGoal = 0;
            pjsg->pssgGoal = 0;
        }
        fComplete = pjsg->pssgGoal == 0;
        break;
    case RELAYCK_Unk58:
        if (pjsg->pjt)
        {
            MURRAY *pmurrayShip = (MURRAY *)PloFindJsgObject(pjsg, prelaycmd->unk58.oidShip);
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C3620)
            {
                fComplete = 1;
            }
            else
            {
                fComplete = 0;
                if (pmurrayShip->moverec.nState < 7)
                {
                    fComplete = pmurrayShip->moverec.nState >= 4;
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk59:
        pmurray = (MURRAY *)pjsg->pjt;
        if (pmurray)
        {
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C3624)
            {
                fComplete = 1;
            }
            else
            {
                qword qwNew = pmurray->moverec.qwPosNew;
                qword qwOld = pmurray->moverec.qwPosOld;
                float gEps = D_002C3628;
                fComplete = FFloatsNear(GXOfQwAct(qwNew), GXOfQwAct(qwOld), gEps) && FFloatsNear(GYOfQw(qwNew), GYOfQw(qwOld), gEps) &&
                            FFloatsNear(GZOfQw(qwNew), GZOfQw(qwOld), gEps);
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk5A:
    {
        pmurray = (MURRAY *)pjsg->pjt;
        if (pmurray)
        {
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C362C)
            {
                fComplete = 1;
            }
            else if (pmurray->moverec.nState == -1 || pmurray->moverec.nState == 0)
            {
                if (pmurray->moverec.nState == 0)
                {
                    pmurray->VUnk87(0);
                }
                fComplete = ((STEPPLAYER *)prelaycmd->unk5A.nUnk6)->jtair.nState == -1;
            }
            else
            {
                fComplete = 0;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    default:
        fComplete = 1;
        break;
    }
    return fComplete;
}
