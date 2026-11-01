// func_0018C250: ported from Lilly's personal Sly 2 decomp (src/P2/jtstate.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF358 (D_002BF358), D_002BF35C (D_002BF35C), D_002BF3E0 (D_002BF3E0), D_002BF3E4 (D_002BF3E4), D_002BF3E8 (D_002BF3E8), D_002DDED0 (g_clock), D_002DEB30 (g_game), D_002A2DD0 (g_qwZero), D_003DD490 (g_sw)
// 0x0018C250 SetJtJts
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
struct CBinaryInputStream;
struct LO;
struct ALO;
struct SO;
struct GAME;
struct SAA;
struct GLOB;
struct SUBGLOB;
enum OID
{
    OID_Nil = -1,
};
extern GAME D_002DEB30;
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
struct SM;
struct SMT;
struct ASEGA;
typedef int GRFAPL;
struct SMA : public BASIC
{
      int cref;
      DLE dleSm;
      DLE dleSw;
      SM *psm;
      ALO *paloRoot;
      int grfapl;
      ASEGA *pasegaCur;
      int ismsCur;
      int ismsNext;
      int ismsGoal;
      SMT *psmtCur;
      float svtLocal;
      int grfRoute;
};
struct SMPA
{
    float svFast;
    float svSlow;
    float dtFast;
    float sdvMax;
};
SMA *PsmaApplySm(SM *psm, ALO *paloRoot, OID oidInitialState, GRFAPL grfapl, int grfRoute);
int IsmsFindSmOptional(SM *psm, OID oid);
void RetractSma(SMA *psma);
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
float RadNormalize(float rad);
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
struct ACT1 : public ACT
{
      int nPhase;
      qword qwRest;
    ACT1() { ack = 1; nPhase = 2; }
};
struct ACT5 : public ACT
{
      ACT *pactCur;
      qword qwOffset;
    ACT5() { ack = 5; }
};
void InsertNodeBySlot(SO *pso, XF *pxf, ACT *pact);
void RemoveNodeBySlot(SO *pso, XF *pxf, ACT *pact);
extern qword D_002A2DD0;
struct SND
{
      undefined4 unk_0x00[3];
};
SND *MakeSnd(int sfxid);
void SetSndOwner(SND *psnd, LO *ploOwner);
void SetSndVolume(SND *psnd, float gVolume);
void SetSndNearFar(SND *psnd, float sNear, float sFar);
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
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
};
struct EXPL : public XFM
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
struct JT;
struct TN;
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
void FreeSwXaList(SW *psw, XA *pxaFirst);
struct XP;
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct DZ;
struct DZX;
struct FX;
struct SOTREE;
struct SURFX;
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
};
void ShowSoPartRec(SO *pso, SOPART *ppart);
void RemoveSoXa(SO *pso, XA *pxaRemove);
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
      virtual void VUnk86(int fOn);
};
void SetStepRadTarget(STEP *pstep, float radTarget);
void SetStepRadTowardDir(STEP *pstep, qword dpos);
struct RAW8
{
    char ach[8];
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
void ApplyAseg(ASEG *paseg, ALO *paloAsegRoot, float tLocal, float svtLocal, GRFAPL grfapl, ASEGA **ppasega);
float TFindAsegLabel(ASEG *paseg, OID oidLabel);
void ReblendAsegbl(ASEGBL *pasegbl);
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
struct JTREC
{
      JT *pjt;
      int nState;
      float tStart;
      int nSide;
      ASEG *apaseg[2];
};
int NJtRecSide(JTREC *prec);
void SetJtRecState(JTREC *prec, int nState);
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
void SetJtModeState(JTMODE *pjtmode, int nState);
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
      virtual void SetCtlState(int nState);
      virtual void OnActive(int fActive);
      virtual int FTakeDamage(ZPR *pzpr);
      virtual int FInvulnerable(ZPK zpk);
      virtual int JthsCurrent();
      virtual void VUnk81(int grf, LO *plo, qword qw1, qword qw2, qword qw3);
      virtual void VUnk82(uint grf, int fCount);
      virtual void VUnk83(uint grf, int fCount);
      virtual int VUnk84();
      virtual int FCanBeHealed();
      virtual void AddHpMp(int dhp, int dmp);
      virtual int VUnk85(ZPR *pzpr);
    virtual void VUnk87(int n);
    virtual int VUnk88();
    virtual void VUnk89();
};
void SetJtPipeState(JTPIPE *pjtpipe, int nState);
void SetJtAirState(JTAIR *pjtair, int nState, LO *plo);
struct TRI3
{
    int a, b, c;
};
struct SURFLOC
{
    int itri;
    float u;
    float v;
};
struct CLIMBSURF : public XFM
{
      int cvtx;
      VECTOR4 *avtx;
      int ctri;
      TRI3 *atri;
      TRI3 *atriB;
      undefined4 unk_0xf4[3];
    void FindClosestPoint(qword pos, SURFLOC *plocHint, SURFLOC *ploc, qword *ppos, qword *pnormal);
};
struct TARGET : public XFM
{
      DLE dleTarget;
      int grftak;
      float sRadiusTarget;
      int fHitTest;
      int nUnkF4;
      uint32_t   _pad_22    [ 2 ] ;
};
struct CID19 : public TARGET, public GRABSPOT
{
      int nHangKind;
      undefined4 unk_0x128;
      int sfxidHangStart;
      int sfxidHangEnd;
      float gUnk134;
      LO *ploUnk138;
      undefined4 unk_0x13c;
};
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
    JTS_Stand = 0,
    JTS_Run = 1,
    JTS_Air = 2,
    JTS_Hang = 3,
    JTS_Unk04 = 4,
    JTS_Unk05 = 5,
    JTS_Interact = 6,
    JTS_Grab = 7,
    JTS_Unk08 = 8,
    JTS_Unk09 = 9,
    JTS_Unk0A = 10,
    JTS_Pickpocket = 11,
    JTS_Gait2 = 12,
    JTS_Unk0D = 13,
    JTS_Unk0E = 14,
    JTS_Pounce = 16,
    JTS_Unk11 = 17,
    JTS_Unk12 = 18,
    JTS_Unk13 = 19,
    JTS_Unk14 = 20,
    JTS_SurfaceClimb = 21,
    JTS_HangFollow = 22,
    JTS_Throw = 23,
    JTS_Unk18 = 24,
    JTS_Max = 25
};
enum JTBS
{
    JTBS_Nil = -1,
    JTBS_0 = 0, JTBS_1 = 1, JTBS_2 = 2, JTBS_3 = 3, JTBS_4 = 4, JTBS_5 = 5, JTBS_6 = 6,
    JTBS_7 = 7, JTBS_8 = 8, JTBS_9 = 9, JTBS_10 = 10, JTBS_11 = 11, JTBS_12 = 12, JTBS_13 = 13
};
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
void RetractAsega(ASEGA *pasega);
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
void ExplodeExpl(EXPL *pexpl);
void OnGameEvent(GAME *pgame, int nEvent, LO *plo);
void CalculateJtHangAccel(JT *pjt);
struct STEPGUARD;
extern "C" {
void SetJtAnimState(JT *pjt, int jtas);
}
extern "C" {
void SetHeroHurtState(JTHURT *pjthurt, int nState);
}
extern "C" {
void SetJtThrowState(JTTHROW *pjtthrow, int nState);
}
extern "C" {
void SetJtGrabPhase(JTGRAB *pjtgrab, int nPhase);
}
extern "C" {
void UpdateJtPounceArc(JT *pjt);
}
extern "C" {
void JtHangFrame(JT *pjt, float *prad);
}
extern "C" {
int GrfJtLeanMask(STEPPLAYER *pstepplayer);
}
extern "C" {
void RollHeroIdleTimer(STEPPLAYER *pstepplayer);
}
extern "C" {
int FPowerUpAcquired(int ipowerup);
}
extern "C" {
void ReleaseRefNode(void *pvTarget, void *pvRef);
}
extern "C" {
void AddRefOidRef(void *pvTarget, void *pvRef);
}
extern "C" {
void UpdateJtHangPlace(JT *pjt);
}
extern "C" {
qword func_00190C58(JT *pjt);
}
extern "C" {
void func_001FB3F0(SND *psnd, qword qwPos);
}
extern "C" {
void GuardCapVelocityForPounce(STEPGUARD *pguard);
}
extern "C" {
void ClearHeroIdleTimer(STEPPLAYER *pstepplayer, int nIdleKind);
}
extern "C" {
void SetJtUnk1d30(JT *pjt, int n);
}
extern "C" {
void ClearGaitFootDown(GAIT *pgait);
}
extern "C" {
void PredictAsegEffect(float t, float svt, ASEG *paseg, ALO *palo, XF *pxfPart, XF *pxfRoot, MATRIX4 *pmat, int n);
}
extern "C" {
void SetSoCnstrForce(SO *pso, int fOn);
}
extern "C" {
void UpdateHeadingSmooth(STEP *pstep);
}
void SlotSetRole(SO *pso, XF *pxf, int nRole);
void SetPlayerTn(STEPPLAYER *pstepplayer, TN *ptn);
extern float D_002BF3E0;
extern float D_002BF3E4;
extern float D_002BF3E8;
extern float D_002BF358;
extern float D_002BF35C;
static inline void ClearXyz(VECTOR *pvec)
{
    pvec->z = 0.0f;
    pvec->y = 0.0f;
    pvec->x = 0.0f;
}
static inline qword QwScale(qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(gScale.qw));
    return qw;
}
static inline qword QwDivide(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline VU_FLOAT VuFloatBits(float g)
{
    VU_FLOAT v;
    __asm__("mfc1 %0, %1" : "=r"(v.qw) : "f"(g));
    return v;
}
static inline qword QwAddVelocity(VU_FLOAT t, qword pos, qword vel)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(qw) : "j"(pos), "j"(t.qw), "j"(vel));
    return qw;
}
static inline qword QwRotateVecRows(qword qwRow0, qword qwRow1, qword qwRow2, qword qwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(qwResult)
            : "j"(qwRow0), "j"(qwRow1), "j"(qwRow2), "j"(qwVec));
    return qwResult;
}
static inline qword QwRotateVecV(qword qwVec, MATRIX3 *pmat)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(qwVec), "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]));
    return qwResult;
}
void ChooseJtRushTarget(JT *pjt);
void ChooseJtSmashTarget(JT *pjt);
void SetJtChannelMask(JT *pjt, uint grf);
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs) __asm__("func_0018C250");
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs)
{
    ASEG *paseg = 0;
    uint grf = 0;
    int sfxid = -1;
    OID oid = OID_Nil;
    int grfapl = 1;
    float tStart = 0.0f;
    float svt = 1.0f;
    SM *psm = 0;
    int grfRoute = 0xFFFC;
    int fKeep = 0;
    if (pjt->jts == jts && pjt->jtbs == jtbs)
        return;
    if (pjt->jts != jts)
    {
        if (jts == JTS_Air)
        {
            switch (pjt->jts)
            {
                case JTS_Hang:
                case JTS_HangFollow:
                    pjt->jtair.ploUnk37c = (LO *)pjt->phndUnk18c4;
                    break;
                case JTS_Unk08:
                    pjt->jtair.ploUnk37c = (LO *)pjt->jtpipe.pshape;
                    break;
                case JTS_SurfaceClimb:
                    pjt->jtair.ploUnk37c = pjt->ploUnk1f18;
                    break;
                default:
                    pjt->jtair.ploUnk37c = 0;
                    break;
            }
        }
        switch (pjt->jts)
        {
            case JTS_Air:
                switch (jts)
                {
                    case JTS_Hang:
                    case JTS_Unk08:
                    case JTS_Unk12:
                        break;
                    case JTS_Grab:
                        if (pjt->jtgrab.nState == 1)
                            break;
                    default:
                    {
                        SND *psnd = MakeSnd(0x3D);
                        SetSndOwner(psnd, pjt);
                        SetSndVolume(psnd, 0.7f);
                        SetSndNearFar(psnd, 500.0f, 1000.0f);
                    }
                    break;
                }
                if (pjt->jtas == 0)
                {
                    if (jts != JTS_Unk08 && jts != JTS_Hang && jts != JTS_HangFollow)
                        SetJtAnimState(pjt, -1);
                }
                if (pjt->nUnk108c == 0)
                {
                    EXPL *pexpl = 0;
                    switch (jts)
                    {
                        case JTS_Grab:
                            switch (pjt->jtgrab.nState)
                            {
                                case 1:
                                    break;
                                case 3:
                                case 4:
                                    pexpl = pjt->pexplDustGrab;
                                    break;
                                default:
                                    pexpl = pjt->pexplDustLand;
                                    break;
                            }
                            break;
                        case JTS_Stand:
                        case JTS_Run:
                        case JTS_Unk05:
                        case JTS_Unk09:
                        case JTS_Unk18:
                            pexpl = pjt->pexplDustLand;
                            break;
                    }
                    if (pexpl != 0)
                    {
                        pexpl->Add();
                        ExplodeExpl(pexpl);
                        pexpl->Remove();
                    }
                }
                SetJtAirState(&pjt->jtair, -1, 0);
                break;
            case JTS_Hang:
                pjt->phndUnk18c4->HandleSpliceEvent(6, 0, 0, 0, 0);
                ReleaseRefNode(pjt->phndUnk18c4, pjt);
                SetJtAnimState(pjt, -1);
                RemoveNodeBySlot(pjt, (XF *)pjt->precUnk19c0, (ACT *)pjt->nUnk1a00);
                if (pjt->janim.pxaHang != 0)
                {
                    RemoveSoXa(pjt->janim.pxaHang->pso, pjt->janim.pxaHang);
                    FreeSwXaList(&D_003DD490, pjt->janim.pxaHang);
                    pjt->janim.pxaHang = 0;
                }
                SetPlayerTn(pjt, 0);
                break;
            case JTS_Unk08:
            {
                SetJtPipeState(&pjt->jtpipe, -1);
                int jtas = -1;
                if (jts == JTS_Air && pjt->pasegUnk14d8 != 0)
                    jtas = 0xB;
                SetJtAnimState(pjt, jtas);
            }
            break;
            case JTS_Grab:
                SetJtGrabPhase(&pjt->jtgrab, -1);
                break;
            case JTS_Unk04:
                SetHeroHurtState(&pjt->jthurt, -1);
                break;
            case JTS_Throw:
                SetJtThrowState(&pjt->jtthrow, -1);
                break;
            case JTS_Interact:
                SetJtRecState((JTREC *)&pjt->unk_0x1ff0, -1);
                break;
            case JTS_Unk09:
                pjt->ploUnk18c0 = 0;
                ShowSoPartRec(pjt, pjt->precUnk1a90);
                break;
            case JTS_Stand:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                switch (pjt->jtas)
                {
                    case 6:
                        SetJtAnimState(pjt, 4);
                        break;
                    case 7:
                        SetJtAnimState(pjt, 5);
                        break;
                    case 8:
                    case 9:
                        SetJtAnimState(pjt, -1);
                        break;
                    case 0xC:
                        if (jtbs != JTBS_6)
                            SetJtAnimState(pjt, 0xE);
                        break;
                    case 0xD:
                        if (jtbs != JTBS_7)
                            SetJtAnimState(pjt, 0xF);
                        break;
                }
                RollHeroIdleTimer(pjt);
                break;
            case JTS_Pickpocket:
                pjt->grfUnk10e0 &= ~2;
                break;
            case JTS_Run:
            case JTS_Unk0A:
            case JTS_Gait2:
                pjt->uGait = pjt->pasegaUnk1030->tLocal / pjt->pasegaUnk1030->paseg->tMax;
                break;
            case JTS_Pounce:
                if (pjt->pasegaUnk1030 != 0 && !FAsegaStopped(pjt->pasegaUnk1030))
                    GuardCapVelocityForPounce((STEPGUARD *)pjt->ploUnk10dc);
                pjt->nUnk1d2c = 0;
                pjt->grfUnk10e0 &= ~4;
                break;
            case JTS_Unk12:
            case JTS_Unk13:
            case JTS_Unk14:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                break;
            case JTS_SurfaceClimb:
                SetSoCnstrForce(pjt, 0);
                pjt->ploUnk1f18->HandleSpliceEvent(6, 0, 0, 0, 0);
                if (pjt->ptnUnk1024 == *(TN **)((char *)pjt->ploUnk1f18 + 0x110))
                    SetPlayerTn(pjt, 0);
                pjt->ploUnk1f18 = 0;
                break;
            case JTS_HangFollow:
                SetSoCnstrForce(pjt, 0);
                pjt->phndUnk18c4->HandleSpliceEvent(6, 0, 0, 0, 0);
                ReleaseRefNode(pjt->phndUnk18c4, pjt);
                pjt->phndUnk18c4 = 0;
                SetPlayerTn(pjt, 0);
                break;
            case JTS_Unk11:
                pjt->nUnk1d30 = 0;
                OnGameEvent(&D_002DEB30, 0x38, pjt);
                break;
            case JTS_Unk18:
                pjt->rUnk200 = 1.0f;
                pjt->pctl->radLeft = pjt->radTarget;
                break;
            case JTS_Unk0D:
                pjt->ploUnk18c0 = 0;
                break;
        }
        switch (jts)
        {
            case JTS_Stand:
            {
                psm = pjt->psmUnk1028;
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                pjt->nUnk1514 = 0;
                switch (pjt->jts)
                {
                    case JTS_Unk0A:
                        oid = (OID)0x177;
                        break;
                    case JTS_Gait2:
                        oid = (OID)0x176;
                        break;
                    case JTS_Air:
                    {
                        int fHigh = pjt->janim.nUnk10;
                        if (fHigh == -1)
                            fHigh = D_002BF3E0 < pjt->jtair.zStart - GZOfQw(PosLoWorld(pjt));
                        oid = (fHigh == 1) ? (OID)0x179 : (OID)0x178;
                    }
                    break;
                    default:
                        oid = (OID)0x175;
                        break;
                }
                if (IsmsFindSmOptional(psm, oid) < 0)
                    oid = OID_Nil;
                grfRoute = GrfJtLeanMask(pjt);
                ClearXyz(&pjt->vUnk190);
                if (pjt->pasegblUnk1a54 != 0 && pjt->pasegblUnk1a70 != 0)
                {
                    switch (pjt->jtas)
                    {
                        case 4:
                            SetJtAnimState(pjt, 6);
                            break;
                        case 5:
                            SetJtAnimState(pjt, 7);
                            break;
                    }
                }
                grf = 0x1F;
                ClearHeroIdleTimer(pjt, 1);
            }
            break;
            case JTS_Run:
                paseg = ((GAIT *)pjt->unk_0x1320)->pasegbl;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                ClearGaitFootDown((GAIT *)pjt->unk_0x1320);
                break;
            case JTS_Interact:
                fKeep = 1;
                SetJtRecState((JTREC *)&pjt->unk_0x1ff0, NJtRecSide((JTREC *)&pjt->unk_0x1ff0));
                break;
            case JTS_Hang:
            {
                pjt->janim.nUnk18 = -1;
                pjt->gUnk199c = 1.0f;
                pjt->gUnk1998 = 1.0f;
                ACT1 *pact1 = (ACT1 *)pjt->nUnk1a00;
                svt = 0.0f;
                pjt->janim.tHang = D_002DDED0.t;
                paseg = pjt->pasegHang;
                ClearXyz((VECTOR *)&pact1->qwRest);
                tStart = 0.5f;
                InsertNodeBySlot(pjt, (XF *)pjt->precUnk19c0, (ACT *)pjt->nUnk1a00);
                qword posHand = PosLoWorld(pjt->psoUnk19a4);
                pjt->janim.dposHang = QwSub(posHand, func_00190C58(pjt));
                UpdateJtHangPlace(pjt);
                pjt->phndUnk18c4->HandleSpliceEvent(5, 0, 0, 0, 0);
                AddRefOidRef(pjt->phndUnk18c4, pjt);
                float radFrame;
                JtHangFrame(pjt, &radFrame);
                float radBack = RadNormalize(radFrame + 3.1415927f);
                int nKind = 1;
                if (pjt->phndUnk18c4 != 0)
                    nKind = pjt->phndUnk18c4->nHangKind;
                switch (nKind)
                {
                    case 1:
                        SetStepRadTarget(pjt, radFrame);
                        pjt->janim.fHangBack = 0;
                        break;
                    case 2:
                        SetStepRadTarget(pjt, radBack);
                        pjt->janim.fHangBack = 1;
                        break;
                    case 0:
                        if (fabsf(RadNormalize(radFrame - pjt->pctl->radLeft)) < 1.5707964f)
                        {
                            SetStepRadTarget(pjt, radFrame);
                            pjt->janim.fHangBack = 0;
                        }
                        else
                        {
                            SetStepRadTarget(pjt, radBack);
                            pjt->janim.fHangBack = 1;
                        }
                        break;
                }
                sfxid = pjt->phndUnk18c4->sfxidHangStart;
                SetJtAnimState(pjt, 1);
                ClearXyz(&pjt->vUnk190);
                CalculateJtHangAccel(pjt);
                SetPlayerTn(pjt, pjt->phndUnk18c4->ptn);
            }
            break;
            case JTS_Unk08:
                SetJtPipeState(&pjt->jtpipe, 0);
                fKeep = 1;
                break;
            case JTS_Unk04:
                SetJtModeState(&pjt->jtmode, -1);
                SetJtUnk1d30(pjt, 0);
                break;
            case JTS_Grab:
                if (pjt->jtgrab.nState < 5)
                    grf = (pjt->jtgrab.nState < 3) ? 0 : 7;
                break;
            case JTS_Throw:
                fKeep = 1;
                break;
            case JTS_Unk09:
                SetJtAnimState(pjt, -1);
                ChooseJtRushTarget(pjt);
                paseg = pjt->pasegUnk1a7c;
                break;
            case JTS_Unk0D:
                SetJtAnimState(pjt, -1);
                ClearXyz(&pjt->vUnk190);
                ChooseJtSmashTarget(pjt);
                paseg = pjt->pasegUnk1a80;
                pjt->tUnk19b0 = D_002DDED0.t + D_002BF3E4;
                break;
            case JTS_Unk0E:
                SetJtAnimState(pjt, -1);
                ClearXyz(&pjt->vUnk190);
                paseg = pjt->pasegUnk1a84;
                break;
            case JTS_Unk0A:
                paseg = pjt->pasegblUnk1bc0;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                pjt->nUnk1038 = -1;
                break;
            case JTS_Pickpocket:
                paseg = pjt->pasegPickpocket;
                *(qword *)&pjt->vUnk190 = D_002A2DD0;
                pjt->grfUnk10e0 |= 2;
                grf = 100;
                pjt->nUnk1bd4 = 0;
                break;
            case JTS_Gait2:
                paseg = ((GAIT *)pjt->unk_0x1c30)->pasegbl;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                ClearGaitFootDown((GAIT *)pjt->unk_0x1c30);
                break;
            case JTS_Pounce:
                pjt->grfUnk10e0 |= 4;
                if (FPowerUpAcquired(0x1C))
                {
                    paseg = pjt->pasegPouncePowerUp;
                    UpdateJtPounceArc(pjt);
                    MATRIX3 *pmat = &pjt->matTarget;
                    qword qwVel = QwRotateVecRows(*(qword *)pmat->mat[0], *(qword *)pmat->mat[1], *(qword *)pmat->mat[2], QwScale(*(qword *)&pjt->vUnk190, VuFloat(-1.0f)));
                    pjt->SetVelocity(qwVel);
                    UpdateHeadingSmooth(pjt);
                }
                else
                {
                    paseg = pjt->pasegUnk1d1c;
                    qword posTarget = PosLoWorld(pjt->ploUnk10dc);
                    float gZero = 0.0f;
                    qword dpos = QwSub(posTarget, PosLoWorld(pjt));
                    SetStepRadTowardDir(pjt, QwScale(dpos, VuFloat(-1.0f)));
                    UpdateHeadingSmooth(pjt);
                    float tLabel = TFindAsegLabel(paseg, (OID)0x173);
                    MATRIX4 matHand;
                    PredictAsegEffect(tLabel, gZero, paseg, (ALO *)pjt, pjt->psoUnk19a4->pxfRoot, pjt->pxfRoot, &matHand, 0);
                    tLabel /= paseg->svtMaster;
                    qword posHold = PosLoWorld(pjt->ploUnk10dc);
                    qword posAim = QwSub(QwAddVelocity(VuFloatBits(tLabel), posHold, pjt->ploUnk10dc->GetVelocity()), QwRotateVecV(*(qword *)&matHand.mat[3], &pjt->matTarget));
                    *(qword *)&pjt->vUnk190 = QwDivide(QwSub(posAim, PosLoWorld(pjt)), VuFloatBits(tLabel));
                    pjt->vUnk190.z = gZero;
                    pjt->SetVelocity(*(qword *)&pjt->vUnk190);
                }
                break;
            case JTS_Unk12:
                psm = pjt->psmUnk102c;
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
            case JTS_Unk13:
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
                break;
            case JTS_Unk14:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
                break;
            case JTS_Unk11:
                paseg = pjt->pasegUnk1d34;
                grfapl = 9;
                OnGameEvent(&D_002DEB30, 0x37, pjt);
                break;
            case JTS_SurfaceClimb:
                ((CLIMBSURF *)pjt->ploUnk1f18)->FindClosestPoint(PosLoWorld(pjt), 0, (SURFLOC *)&pjt->unk_0x1f1c, 0, 0);
                SetSoCnstrForce(pjt, 1);
                pjt->SetVelocity(D_002A2DD0);
                pjt->SetAngularVelocity(D_002A2DD0);
                ClearXyz(&pjt->vUnk190);
                pjt->ploUnk1f18->HandleSpliceEvent(5, 0, 0, 0, 0);
                if (*(TN **)((char *)pjt->ploUnk1f18 + 0x110) != 0)
                    SetPlayerTn(pjt, *(TN **)((char *)pjt->ploUnk1f18 + 0x110));
                break;
            case JTS_HangFollow:
                paseg = pjt->pasegblUnk1f90;
                grfapl = 9;
                SetJtAnimState(pjt, -1);
                SetSoCnstrForce(pjt, 1);
                pjt->SetVelocity(D_002A2DD0);
                pjt->SetAngularVelocity(D_002A2DD0);
                ClearXyz(&pjt->vUnk190);
                pjt->phndUnk18c4->HandleSpliceEvent(5, 0, 0, 0, 0);
                AddRefOidRef(pjt->phndUnk18c4, pjt);
                SetPlayerTn(pjt, pjt->phndUnk18c4->ptn);
                break;
            case JTS_Unk18:
            {
                float gZero = 0.0f;
                grfapl = 9;
                grf = 0x18;
                pjt->ablLean[2].gWeight = gZero;
                pjt->ablLean[0].gWeight = gZero;
                pjt->ablLean[1].gWeight = 1.0f;
                ReblendAsegbl(pjt->pasegblUnk2038);
                paseg = pjt->pasegblUnk2038;
                SetStepRadTarget(pjt, pjt->pctl->radLeft);
                qword qwVel = MakeVecFromAngle(pjt->radTarget, D_002BF3E8, gZero);
                pjt->SetVelocity(qwVel);
                ClearXyz(&pjt->vUnk190);
            }
            break;
        }
    }
    if (pjt->jtbs != jtbs)
    {
        switch (jtbs)
        {
            case JTBS_Nil:
                break;
            case JTBS_0:
            case JTBS_1:
            case JTBS_2:
            {
                ASEG *pasegHang = pjt->pasegHang;
                if (pjt->pasegaUnk1030 != 0 && pjt->pasegaUnk1030->paseg == pasegHang)
                {
                    fKeep = 1;
                }
                else
                {
                    paseg = pasegHang;
                    svt = 0.0f;
                    tStart = paseg->tMax * 0.5f;
                }
            }
            break;
            case JTBS_3:
                psm = pjt->janim.psmHang;
                break;
            case JTBS_4:
                psm = pjt->psmUnk1bc4;
                ClearXyz(&pjt->vUnk190);
                break;
            case JTBS_5:
                pjt->nUnk1038 = -1;
                paseg = pjt->pasegblUnk14d0;
                ClearXyz(&pjt->vUnk190);
                grfapl = 9;
                break;
            case JTBS_6:
                SetJtAnimState(pjt, -1);
                paseg = pjt->apasegSwing[10];
                ClearXyz(&pjt->vUnk190);
                pjt->janim.fLandAlt = 0;
                pjt->janim.fInputOpen = 0;
                break;
            case JTBS_7:
                SetJtAnimState(pjt, -1);
                paseg = pjt->apasegSwing[11];
                ClearXyz(&pjt->vUnk190);
                pjt->janim.fLandAlt = 0;
                pjt->janim.fInputOpen = 0;
                break;
            case JTBS_8:
            case JTBS_9:
                if (jtbs == JTBS_8)
                    paseg = pjt->apasegSwing[12];
                else
                    paseg = pjt->pasegUnk1fe8;
                pjt->janim.fInputOpen = 0;
                ClearXyz(&pjt->vUnk190);
                break;
            case JTBS_10:
                psm = pjt->psmUnk1f14;
                break;
            case JTBS_11:
                paseg = pjt->pasegblUnk1f10;
                grfapl = 9;
                break;
            case JTBS_12:
                pjt->rUnk200 = D_002BF358;
                grf = 0x18;
                break;
            case JTBS_13:
                fKeep = 1;
                pjt->rUnk200 = D_002BF35C;
                grf = 0x18;
                break;
        }
    }
    if (jts != pjt->jts)
    {
        pjt->jts = jts;
        pjt->tUnk18cc = D_002DDED0.t;
    }
    if (jtbs != pjt->jtbs)
    {
        pjt->jtbs = jtbs;
        pjt->tUnk18d4 = D_002DDED0.t;
    }
    pjt->VUnk89();
    SetJtChannelMask(pjt, grf);
    if (sfxid != -1)
    {
        SND *psnd = MakeSnd(sfxid);
        SetSndOwner(psnd, 0);
        func_001FB3F0(psnd, PosLoWorld(pjt));
    }
    if (!fKeep)
    {
        if (pjt->pasegaUnk1030 != 0)
        {
            RetractAsega(pjt->pasegaUnk1030);
            pjt->pasegaUnk1030->VUnk01();
            pjt->pasegaUnk1030 = 0;
        }
        else if (pjt->psmaUnk1034 != 0)
        {
            RetractSma(pjt->psmaUnk1034);
            pjt->psmaUnk1034->VUnk01();
            pjt->psmaUnk1034 = 0;
        }
        if (paseg != 0)
        {
            ApplyAseg(paseg, (ALO *)pjt, tStart, svt, grfapl, &pjt->pasegaUnk1030);
            pjt->pasegaUnk1030->VUnk00();
        }
        else if (psm != 0)
        {
            pjt->psmaUnk1034 = PsmaApplySm(psm, (ALO *)pjt, oid, grfapl, grfRoute);
            pjt->psmaUnk1034->VUnk00();
        }
    }
}
