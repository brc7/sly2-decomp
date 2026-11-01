// func_00195A00: ported from Lilly's personal Sly 2 decomp (src/P2/jtair.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF750 (D_002BF750), D_002BF754 (D_002BF754), D_002BF758 (D_002BF758), D_002BF75C (D_002BF75C), D_002BF760 (D_002BF760), D_002BF764 (D_002BF764), D_002BF76C (D_002BF76C), D_002BF770 (D_002BF770), D_002BF730 (g_SheetMovementMiscJumpHorizontalSpeed), D_002BF768 (g_SheetMovementMiscTurnbuckleLaunchHorizontalSpeed), D_002DDED0 (g_clock), D_002A2DC0 (g_normalZ)
// 0x00195A00 SetJtAirJump (curated name): starts a flight of the given kind. n2 picks the directional clip; plo is the object the sub-state works on.
// Every kind but the double jump (6) restarts the record (its timers, zStart, the JT's landing clue). The launch velocity comes from JtPlanLaunchVz; kinds
// 3 / 0xE add a push along the facing when the stick is at rest, 0x11 (wall run) flattens it and adds 400 up. Then the sub-state (SetJtAirState) and the
// per-kind ratios.
typedef unsigned char    uint8_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
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
struct CBinaryInputStream;
struct LO;
struct SO;
struct GLOB;
struct SUBGLOB;
struct RAW8
{
    char ach[8];
};
enum OID
{
};
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
extern "C" qword MakeVecFromAngle(float rad, float sXY, float sZ);
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct MATRIX3
{
	float mat[3][4];
} __attribute__((aligned(16)));
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
qword PosLoWorld(LO *plo);
typedef int GRFAPL;
struct SMPA
{
    float svFast;
    float svSlow;
    float dtFast;
    float sdvMax;
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
struct SND
{
      undefined4 unk_0x00[3];
};
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
struct SAA;
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
struct TN;
struct STEPPLAYER;
struct PO;
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
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
      virtual void VUnk81(int grf, LO *plo, qword qw1, qword qw2, qword qw3);
      virtual void VUnk82(uint grf, int fCount);
      virtual void VUnk83(uint grf, int fCount);
      virtual int VUnk84();
      virtual int VUnk85(ZPR *pzpr);
      virtual void VUnk86(int fOn);
};
void SetStepGravityRatio(STEP *pstep, float rGravity);
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
struct GRABSPOT;
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
      virtual void OnActive(int fActive);
      virtual int FInvulnerable(ZPK zpk);
      virtual int JthsCurrent();
    virtual void VUnk87(int n);
};
void SetJtAirState(JTAIR *pjtair, int nState, LO *plo);
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
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind);
void SetPlayerTn(STEPPLAYER *pstepplayer, TN *ptn);
void DecomposeCylind(qword v, float *prad, float *psXY, float *pz);
extern qword D_002A2DC0;
extern qword D_002BF770;
extern float D_002BF750;
extern float D_002BF754;
extern float D_002BF758;
extern float D_002BF75C;
extern float D_002BF760;
extern float D_002BF764;
extern float D_002BF76C;
extern float D_002BF730;
extern float D_002BF768;
static inline qword QwProjectOut(qword v, qword n)
{
    qword qwDot = QwDot(v, n);
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(qwDot), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(v), "j"(qw));
    return qw;
}
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo) __asm__("func_00195A00");
/** @sly2-annot addr=0x195A00
 * Purpose: Start a flight of kind nKind (n2 = directional clip, plo = object): reset record unless double
 *   jump (6), launch velocity from JtPlanLaunchVz, push along facing for 3/0xE, wall run 0x11 adds 400 up,
 *   then SetJtAirState
 * Info: proposed name SetJtAirJump (T1); TU air
 * Notes: runtime2: kind 1 jump, double jump keeps kind 1
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo)
{
    qword vel;
    qword velWall;
    float t;
    if (nKind != 6)
        pjtair->nKind = nKind;
    pjtair->n2 = n2;
    pjtair->pstepplayer->VUnk87(1);
    if (nKind != 6)
    {
        pjtair->nUnk40 = 0;
        t = D_002DDED0.t;
        pjtair->tUnk4c = t;
        pjtair->tUnk48 = t;
        pjtair->tUnk44 = t;
    }
    if (nKind == 9)
    {
        pjtair->rGravitySaved = pjtair->pstepplayer->rUnk260;
        SetStepGravityRatio(pjtair->pstepplayer, D_002BF750);
    }
    vel = JtPlanLaunchVz(pjtair, nKind);
    pjtair->pstepplayer->SetVelocity(vel);
    t = D_002DDED0.t;
    pjtair->pstepplayer->tUnk131c = t;
    if (nKind != 6)
        pjtair->tStart = D_002DDED0.t;
    switch (nKind)
    {
        case 0:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0xB:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fUnk24 = 1;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 3:
            if (pjtair->pstepplayer->pctl->sLeft < 0.2f)
            {
                qword dvel;
                pjtair->nUnk40 = 3;
                dvel = MakeVecFromAngle(pjtair->pstepplayer->radTarget, D_002BF754, 0.0f);
                pjtair->pstepplayer->SetVelocity(QwAdd(vel, dvel));
            }
            SetJtAirState(pjtair, 0, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 0.6f;
            pjtair->fUnk24 = 0;
            break;
        case 1:
        case 2:
        case 0x12:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 9:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 1;
            SetPlayerTn(pjtair->pstepplayer, pjtair->ptnLaunch);
            pjtair->gSpeedRatio = D_002BF758;
            pjtair->gUnk34 = D_002BF75C;
            pjtair->fGravityChanged = 1;
            break;
        case 7:
            SetJtAirState(pjtair, 7, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gSpeedRatio = 1.0f;
            pjtair->gUnk34 = 1.0f;
            break;
        case 8:
            SetJtAirState(pjtair, 5, plo);
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            pjtair->fCanDoubleJump = 1;
            DecomposeCylind(vel, &pjtair->radSmash, &pjtair->sSmash, 0);
            pjtair->gUnkb4 = 1.0f;
            break;
        case 6:
        {
            int nState = 2;
            if (pjtair->nKind < 0xF)
            {
                if (pjtair->nKind > 0xC)
                    nState = 3;
            }
            SetJtAirState(pjtair, nState, plo);
            break;
        }
        case 0xE:
        {
            float gRatio;
            SetJtAirState(pjtair, 1, plo);
            gRatio = D_002BF760;
            pjtair->fCanDoubleJump = 1;
            pjtair->gUnk34 = D_002BF764;
            pjtair->fUnk24 = 0;
            pjtair->gSpeedRatio = gRatio;
            if (pjtair->pstepplayer->pctl->sLeft < 0.2f)
            {
                qword dvel;
                pjtair->nUnk40 = 3;
                dvel = MakeVecFromAngle(pjtair->pstepplayer->pctl->radLeft, D_002BF730 * gRatio, 0.0f);
                pjtair->pstepplayer->SetVelocity(QwAdd(vel, dvel));
            }
            break;
        }
        case 0xD:
            SetJtAirState(pjtair, 1, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->gSpeedRatio = D_002BF768;
            pjtair->gUnk34 = D_002BF76C;
            pjtair->fUnk24 = 0;
            break;
        case 0xA:
            pjtair->nUnk40 = -1;
            SetJtAirState(pjtair, 0x15, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 0.0f;
            pjtair->gSpeedRatio = 0.0f;
            break;
        case 0xC:
            SetJtAirState(pjtair, 1, plo);
            pjtair->nUnk40 = 3;
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0x10:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0xF:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0x11:
            if (pjtair->nState == -1)
            {
                pjtair->pstepplayer->SetVelocity(QwAdd(QwProjectOut(velWall, D_002A2DC0), D_002BF770));
                pjtair->fCanDoubleJump = 1;
            }
            SetJtAirState(pjtair, 0x16, 0);
            pjtair->fUnk24 = 0;
            pjtair->fUnk2c = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
    }
    if (nKind != 6)
    {
        if (nKind != 0x11)
            pjtair->fUnk2c = pjtair->fCanDoubleJump;
        if (FIsBasicDerivedFrom(pjtair->pstepplayer, (CID)0x35))
            ((JT *)pjtair->pstepplayer)->janim.nUnk10 = -1;
        pjtair->zStart = GZOfQw(PosLoWorld(pjtair->pstepplayer));
    }
}
