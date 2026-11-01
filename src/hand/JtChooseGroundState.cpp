// JtChooseGroundState: ported from Lilly's personal Sly 2 decomp (src/P2/jtground.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DA0 (D_002A2DA0), D_002BF354 (D_002BF354), D_002BF360 (D_002BF360), D_002BF364 (D_002BF364), D_002BF370 (D_002BF370), D_002DDED0 (g_clock), D_002A2DC0 (g_normalZ)
// 0x0018A8C8 JtChooseGroundState (curated name)
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
struct ALO;
struct SO;
struct SAA;
struct GLOB;
struct SUBGLOB;
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
inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]),
              "j"(qwPos));
    return qwResult;
}
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
qword ConvertLoPos(LO *ploFrom, LO *ploTo, qword qwPos);
MATRIX4 *PmatLoWorld(LO *plo);
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
struct ACT5 : public ACT
{
      ACT *pactCur;
      qword qwOffset;
    ACT5() { ack = 5; }
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
    virtual void GetClosestParam(JT *pjt, float *psParam);
};
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
    void ClearJoyPressed(int grf);
    uint GrfPressedWithin(uint grf, float dt);
    uint GrfConsumePressed(uint grf, float dt);
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
enum SEEK
{
    SEEK_Start = 0,
};
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
float TFindAsegLabel(ASEG *paseg, OID oidLabel);
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
void JtInteractPre(JTREC *prec);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
struct STUNHOLD
{
      int fValid;
      float gSide;
      float gUp;
      undefined4 unk_0x0c;
      qword qwPos;
      qword qwVel;
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
void HeroHurtPre(JTHURT *pjthurt);
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
void UpdateJtThrow(JTTHROW *pjtthrow);
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
    virtual void VUnk87(int n);
    virtual int VUnk88();
    virtual void VUnk89();
    virtual void VUnk90();
    virtual int VUnk91();
      virtual void OnHeroDied();
    virtual int VUnk93();
      virtual void SetPlayerInteractMode(int nKind, LO *ploTarget, int nMode, LO *ploFight);
      virtual void GetStunHold(STUNHOLD *phold, float dt);
      virtual void GetCmFocusPos(VECTOR4 *ppos);
    virtual int VUnk97();
      virtual int FCanUseGadget(int idGadget);
};
void UpdateJtPipe(JTPIPE *pjtpipe);
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo);
struct XPD
{
      SOPART *ppart;
      XF *pxf;
      SO *psoRoot;
      SO *psoLeaf;
      qword pos;
      qword posLeaf;
      qword v;
};
struct XP
{
      XPD axpd[2];
      union { qword qwNormal; VECTOR4 normal; };
      float gDist;
      float mu;
      float elas;
      int xpk;
      union { void *pv0; float sfMin; };
      union { void *pv1; float sfMax; };
      undefined4 unk_0xa8;
      XP *pxpNext;
};
extern "C" {
float PeekLastB(SHAPE *pshape);
}
void GetShapePosDirAtS(SHAPE *pshape, float s, qword *ppos, qword *pdir);
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
    JTS_Unk0A = 10,
    JTS_Pickpocket = 11,
    JTS_Gait2 = 12,
    JTS_Unk0D = 13,
    JTS_Unk0E = 14,
    JTS_Pounce = 16,
    JTS_Unk11 = 17,
    JTS_Unk13 = 19,
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
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs);
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
void SeekAsega(ASEGA *pasega, SEEK seek, float dtLocal, float svtLocal);
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
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline float GXOfQw(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
extern "C" {
void UpdateJtAirInput(JTAIR *pjtair);
}
extern "C" {
void UpdateJtGrabPre(JTGRAB *pjtgrab);
}
extern "C" {
int GetHeroMp(STEPPLAYER *pstepplayer);
}
extern "C" {
uint GrfMatchingSlots(STEPPLAYER *pstepplayer, int n);
}
extern "C" {
int FFightStanceInRange(STEPPLAYER *pstepplayer, int fHysteresis);
}
extern "C" {
qword CalculateLaunchVelocity(float gArc, qword qwFrom, qword qwTo, qword dvGravity);
}
int ChooseJtGrabTarget(JTGRAB *pjtgrab, uint grf, int fUnk);
MATRIX3 LoadRotateMatrixRad(float rad, qword normal);
extern float D_002BF354;
extern float D_002BF360;
extern float D_002BF364;
extern qword D_002BF370;
extern qword D_002A2DA0;
extern qword D_002A2DC0;
static inline qword QwProjectOut(qword v, qword n)
{
    qword qwDot = QwDot(v, n);
    qword qwAlong;
    qword qwAcross;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qwAlong) : "j"(qwDot), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qwAcross) : "j"(v), "j"(qwAlong));
    return qwAcross;
}
static inline qword QwScaleX(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline qword QwRotateVecAt(MATRIX3 *pmat, qword *pqwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(*pqwVec), "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]));
    return qwResult;
}
extern "C" void JtChooseGroundState(JT *pjt) __asm__("JtChooseGroundState");
extern "C" void JtChooseGroundState(JT *pjt)
{
    JTS jtsOld;
    JTBS jtbsOld;
    do
    {
        jtsOld = pjt->jts;
        jtbsOld = pjt->jtbs;
        if (jtsOld >= 0 && jtsOld < JTS_Max)
        {
            switch (jtsOld)
            {
                case JTS_Grab:
                {
                    int nState = pjt->jtgrab.nState;
                    if (nState < 5)
                    {
                        if (nState >= 3 || nState == 1)
                            break;
                        goto fell;
                    }
                    if (nState != 0xC)
                        goto fell;
                    break;
                }
                case JTS_Stand:
                case JTS_Run:
                case JTS_Unk05:
                case JTS_Unk0A:
                case JTS_Pickpocket:
                case JTS_Gait2:
                case JTS_Throw:
                case JTS_Unk18:
                    if (pjt->jtas != 8 && pjt->jtas != 9 || pjt->janim.fInputOpen)
                    {
    fell:
                        if (D_002DDED0.t - pjt->tUnk20c > 0.1f)
                            SetJtAirJump(&pjt->jtair, 0, 0, 0);
                    }
                    break;
            }
            switch (pjt->jts)
            {
                case JTS_Air:
                    UpdateJtAirInput(&pjt->jtair);
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (!pjt->FCanUseGadget(2))
                        break;
                    if (pjt->pctl->GrfConsumePressed(0x80, 0.0f))
                        SetJtJts(pjt, JTS_Unk0D, JTBS_Nil);
                    else if (pjt->pctl->GrfConsumePressed(0x10, 0.0f))
                        SetJtJts(pjt, JTS_Unk0E, JTBS_Nil);
                    break;
                case JTS_Grab:
                    UpdateJtGrabPre(&pjt->jtgrab);
                    break;
                case JTS_Unk04:
                    HeroHurtPre(&pjt->jthurt);
                    break;
                case JTS_Throw:
                    UpdateJtThrow(&pjt->jtthrow);
                    break;
                case JTS_Interact:
                    JtInteractPre((JTREC *)&pjt->unk_0x1ff0);
                    break;
                case JTS_Unk08:
                    UpdateJtPipe(&pjt->jtpipe);
                    break;
                case JTS_Stand:
                case JTS_Run:
                case JTS_Unk0A:
                case JTS_Gait2:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (pjt->jtas == 8 || pjt->jtas == 9)
                    {
                        if (!pjt->janim.fInputOpen)
                            break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (!(pjt->pctl->grfbtn & 8))
                    {
                        if (FFightStanceInRange(pjt, 0))
                        {
                            SetJtJts(pjt, JTS_Unk05, JTBS_4);
                            break;
                        }
                    }
                    if (pjt->pctl->GrfPressedWithin(0x400, 0.15f) && pjt->FCanUseGadget(3))
                    {
                        pjt->pctl->ClearJoyPressed(0x400);
                        StartJtMode(&pjt->jtmode, 0, 0, 0);
                        break;
                    }
                    if (pjt->nUnk10e4 == 3)
                    {
                        if (pjt->pctl->GrfConsumePressed(0x20, 0.15f))
                        {
                            SetJtJts(pjt, JTS_Pickpocket, JTBS_Nil);
                            break;
                        }
                    }
                    if (pjt->pctl->GrfConsumePressed(0x20, 0.2f))
                    {
                        ChooseJtGrabTarget(&pjt->jtgrab, 0xFFFF, 0);
                        break;
                    }
                    {
                        float sLeft = pjt->pctl->sLeft;
                        if (pjt->jts == 0 ? sLeft > 0.1f : sLeft > 0.0f)
                        {
                            if (pjt->pctl->grfbtn & 8)
                                SetJtJts(pjt, JTS_Run, JTBS_Nil);
                            else if (pjt->nUnk10e4 == 3)
                                SetJtJts(pjt, JTS_Unk0A, JTBS_Nil);
                            else
                                SetJtJts(pjt, JTS_Gait2, JTBS_Nil);
                        }
                        else
                            SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    }
                    break;
                case JTS_Unk18:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (!(0.0f < GetHeroMp(pjt)))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (!(pjt->pctl->grfbtn & GrfMatchingSlots(pjt, 0x19)))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->jtbs == JTBS_13)
                    {
                        qword qwVel = pjt->GetVelocity();
                        int fStill = GXOfQw(QwDot(qwVel, *(qword *)PmatLoWorld(pjt)->mat[0])) < D_002BF354;
                        if (fStill)
                            SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    }
                    break;
                case JTS_Unk05:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if ((uint)(pjt->jtbs - 6) < 4)
                        break;
                    if (pjt->pctl->grfbtn & 8)
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (!FFightStanceInRange(pjt, 1))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (pjt->pctl->GrfPressedWithin(0x400, 0.15f) && pjt->FCanUseGadget(3))
                    {
                        pjt->pctl->ClearJoyPressed(0x400);
                        StartJtMode(&pjt->jtmode, 0, 0, 0);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x20, 0.2f))
                    {
                        ChooseJtGrabTarget(&pjt->jtgrab, 0xFFFF, 0);
                        break;
                    }
                    if (0.0f < pjt->pctl->sLeft)
                        SetJtJts(pjt, JTS_Unk05, JTBS_5);
                    else
                        SetJtJts(pjt, JTS_Unk05, JTBS_4);
                    break;
                case JTS_Hang:
                    if (D_002DDED0.t - pjt->tUnk18cc > 0.25f)
                    {
                        if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                            SetJtAirJump(&pjt->jtair, 2, 0, 0);
                    }
                    break;
                case JTS_Unk0D:
                    if (FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        SetJtAirJump(&pjt->jtair, 0xB, 0, 0);
                        break;
                    }
                    if (pjt->pxpFloor)
                    {
                        if (0.7f < pjt->pxpFloor->normal.z)
                        {
                            if (D_002DDED0.t - pjt->tUnk18cc > 0.4f)
                                SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        }
                    }
                    break;
                case JTS_Unk0E:
                    if (FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pxpFloor)
                    {
                        if (0.7f < pjt->pxpFloor->normal.z)
                        {
                            if (D_002DDED0.t - pjt->tUnk18cc > 0.4f)
                            {
                                float tLabel = TFindAsegLabel(pjt->pasegUnk1a84, (OID)0x16D);
                                ASEGA *pasega = pjt->pasegaUnk1030;
                                if (pasega->tLocal < tLabel)
                                    SeekAsega(pasega, SEEK_Start, tLabel, pasega->svt);
                            }
                        }
                    }
                    break;
                case JTS_Pounce:
                    if (!pjt->pasegaUnk1030 || FAsegaStopped(pjt->pasegaUnk1030))
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                case JTS_Unk13:
                {
                    if (!pjt->pasegaUnk1030 || !FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        if (!pjt->psmaUnk1034)
                            break;
                        if (!FIsSmaRetracted(pjt->psmaUnk1034))
                            break;
                    }
                    SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                }
                case JTS_Unk11:
                    if (!pjt->nUnk1d30 || D_002DDED0.t - pjt->tUnk18cc > D_002BF360)
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                case JTS_SurfaceClimb:
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                        SetJtAirJump(&pjt->jtair, 0x12, 0, 0);
                    break;
                case JTS_HangFollow:
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                    {
                        VECTOR4 vTarget;
                        LO *ploTarget = pjt->phndUnk18c4->ploUnk138;
                        if (ploTarget && FIsBasicDerivedFrom(ploTarget, (CID)10))
                        {
                            SHAPE *pshape = (SHAPE *)pjt->phndUnk18c4->ploUnk138;
                            GetShapePosDirAtS(pshape, PeekLastB(pshape) * (0.5f - pjt->gUnk1f94 * 0.5f), (qword *)&vTarget, 0);
                        }
                        else
                        {
                            VECTOR4 vLocal;
                            if (pjt->phndUnk18c4->ploUnk138)
                                *(qword *)&vLocal = ConvertLoPos(0, pjt->phndUnk18c4, PosLoWorld(pjt->phndUnk18c4->ploUnk138));
                            else
                                *(qword *)&vLocal = D_002BF370;
                            MATRIX3 mat;
                            mat = LoadRotateMatrixRad(pjt->gUnk1f94 * D_002BF364, D_002A2DA0);
                            *(qword *)&vTarget = QwTransformPos(pjt->phndUnk18c4->PmatWorld(), QwRotateVecAt(&mat, (qword *)&vLocal));
                        }
                        qword qwVel = CalculateLaunchVelocity(0.5f, PosLoWorld(pjt), QwSub(*(qword *)&vTarget, pjt->qwUnk240), *(qword *)&pjt->psophys->dvGravity);
                        VU_FLOAT gScale = VuFloatFromMemWord(*(int *)&pjt->gUnk1f98);
                        pjt->SetVelocity(QwScaleX(qwVel, gScale));
                        SetJtAirJump(&pjt->jtair, 0xC, 0, 0);
                        pjt->jtair.qwUnkA0 = QwProjectOut(qwVel, D_002A2DC0);
                    }
                    break;
            }
        }
    } while (pjt->jts != jtsOld || pjt->jtbs != jtbsOld);
}
