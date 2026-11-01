// func_0019A750: ported from Lilly's personal Sly 2 decomp (src/P2/jtair.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF8B0 (D_002BF8B0), D_002BF8C0 (D_002BF8C0), D_002BF8C8 (D_002BF8C8), D_002BF8CC (D_002BF8CC), D_002BF8D0 (D_002BF8D0), D_002BF8D4 (D_002BF8D4), D_002BF8D8 (D_002BF8D8), D_002BF8DC (D_002BF8DC), D_002BF73C (g_SheetMovementMiscDoubleJumpHeight), D_002BF738 (g_SheetMovementMiscJumpHeight)
// 0x0019A750 JtPlanLaunchVz (MATCH, T1312): the velocity the hero gets when a flight of this kind starts. It is the current velocity with its z raised:
//   z = max(z, vz of the kind), vz from g = the gravity (negative, SOPHYS::dvGravity.z, -1750) and, per kind
//   1, 7, 0xF   sqrt(2 g (zLand - zFoot - 215))      the X jump: the apex is 215 above the take-off (zLand - zFoot = *pqwUnk218.z - qwUnk230.z is 0 on flat ground, the radicand is floored at 0)
//   9, 0xE, 0xD the same with the apex 1200 / 250 / 500
//   2, 0x12, 3  the constants 700 / 900 / 800 (a launch is never slower than that upwards)
//   4           700 + 0.5 z limited to 700 (a bounce: a fall is turned into a launch of 700 at the most)
//   6           the double jump: it aims for the same landing time as a jump followed by a jump from the apex (CSolveQuadratic: the flight from the first
//               apex 215 with the speed that climbs 100 more) and gets the vz that does it from the current height; still rising: the vz that reaches 315
//   0, 5, 8, 0xA - 0xC, 0x10, 0x11: nothing (z stays)
// Returns the velocity (the callers SetVelocity it).
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
struct SAA;
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
static inline float GEvaluateClq(CLQ *pclq, float x)
{
    return pclq->g0 + x * (pclq->g1 + x * pclq->g2);
}
struct LM
{
	float gMin, gMax;
};
int CSolveQuadratic(float a, float b, float c, float *ax);
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
struct TN;
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
struct XP;
struct SOTREE;
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
};
struct PO;
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
};
struct STEP;
struct BHV;
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
};
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
struct STEPPLAYER;
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
};
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
static inline float GMaxOf(float a, float b)
{
    return (a > b) ? a : b;
}
static inline float GMinOf(float a, float b)
{
    return (a < b) ? a : b;
}
static inline float GLimitRange(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = GEvaluateClq(pclq, x);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    return GLimitRange(g, gMin, gMax);
}
extern float D_002BF8C8;
extern float D_002BF8CC;
extern float D_002BF8D0;
extern float D_002BF8D4;
extern float D_002BF8D8;
extern float D_002BF8DC;
extern float D_002BF738;
extern float D_002BF73C;
extern CLQ D_002BF8B0;
extern LM D_002BF8C0;
static inline float SqrtfEarly(float g)
{
    float r;
    __asm__ volatile("sqrt.s %0, %1" : "=&f"(r) : "f"(g));
    return r;
}
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind) __asm__("func_0019A750");
/** @sly2-annot addr=0x19A750
 * Purpose: Launch velocity for a flight kind: z raised to the per-kind value (215/1200/250/500 apex,
 *   700/900/800 constants, bounce, double jump solved with CSolveQuadratic)
 * Info: proposed name JtPlanLaunchVz (T1); TU air
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind)
{
    VECTOR4 pos;
    VECTOR4 vel;
    float g;
    float vzNew;
    *(qword *)&pos = PosLoWorld(pjtair->pstepplayer);
    *(qword *)&vel = pjtair->pstepplayer->GetVelocity();
    g = pjtair->pstepplayer->psophys->dvGravity.z;
    switch (nKind)
    {
        case 2:
            vzNew = D_002BF8C8;
            break;
        case 0x12:
            vzNew = D_002BF8CC;
            break;
        case 3:
            vzNew = D_002BF8D0;
            break;
        case 1:
        case 7:
        case 9:
        case 0xD:
        case 0xE:
        case 0xF:
        {
            float sApex;
            switch (nKind)
            {
                case 9:
                    sApex = D_002BF8D4;
                    break;
                case 0xE:
                    sApex = D_002BF8D8;
                    break;
                case 0xD:
                    sApex = D_002BF8DC;
                    break;
                default:
                    sApex = D_002BF738;
                    break;
            }
            vzNew = SqrtfEarly(GMaxOf((g + g) * (((VECTOR4 *)pjtair->pstepplayer->pqwUnk218)->z - ((VECTOR4 *)&pjtair->pstepplayer->qwUnk230)->z - sApex), 0.0f));
            break;
        }
        case 4:
            vzNew = GEvaluateClqLm(&D_002BF8B0, &D_002BF8C0, vel.z);
            break;
        case 6:
        {
            float vJump = SqrtfEarly(g * -2.0f * D_002BF738);
            float tApex = -vJump / g;
            float vzMin = GZOfQw(pjtair->pstepplayer->GetVelocity());
            vzMin = GMinOf(vzMin, vJump);
            float tElapsed = (vzMin - vJump) / g;
            float h = tElapsed * (vJump + g * 0.5f * tElapsed);
            float ax[4];
            float bq = SqrtfEarly(g * -2.0f * (D_002BF73C - D_002BF738));
            CSolveQuadratic(g * 0.5f, bq, D_002BF738, ax);
            float tFall = ax[1] - tApex;
            if (0.0f < vzMin)
                vzNew = SqrtfEarly(g * -2.0f * (D_002BF73C - h));
            else
            {
                float T;
                if (vzMin < -vJump)
                    h = 0.0f;
                T = tApex + tApex + tFall - tElapsed;
                vzNew = -h / T - g * 0.5f * T;
            }
            break;
        }
        default:
            vzNew = vel.z;
            break;
    }
    vel.z = GMaxOf(vel.z, vzNew);
    return *(qword *)&vel;
}
