// GatherJtGrabCandidates: ported from Lilly's personal Sly 2 decomp (src/P2/jtgrab.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF660 (D_002BF660), D_002BF664 (D_002BF664), D_002BF70C (D_002BF70C), D_003DA0C0 (g_hintbox), D_003DD490 (g_sw)
// 0x00194A40 GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi  (MATCHES Sly 2; our name for the record)
// Every frame (UpdateJtPre 0x18B7E8 asks with 0xFFFF; the others with a mask the current state allows): collects the hero's grab
// candidates. grf says which of the world's grab lists to walk: 1 the HNDs (found in the TARGET list), 2 PIPEs, 4 class 27, 8 HSHAPEs,
// 0x10 class 34, 0x20 class 38, 0x80 class 35, 0x200 RAILs, 0x400 class 39. 0x195368 and 0x194F70 then refine the four best, and the
// hint box (the popup at the top of the screen) shows the hint text of the best one's spot (nUnk14) while it is within 250 - or the
// text 0x125 for an object of oid 0x624 that has been the best for more than a second.
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
typedef uchar byte;
enum CID
{
};
struct LO;
struct SAA;
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
struct SO;
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
      virtual CLASSINFO *Pclassinfo();
};
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
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
};
qword PosLoWorld(LO *plo);
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
struct TASKPAIR
{
    OID oidTask;
    int nState;
};
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
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
struct ASEG;
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
};
struct FILEREC
{
      uint aw[16];
};
struct CID17;
struct CID124;
struct REGLINKS;
struct REGNODE
{
      VECTOR4 *pplane;
      unsigned short aiChild[2];
};
struct REGTREE
{
      int cnode;
      REGNODE *anode;
      int cplane;
      VECTOR4 *aplane;
      int ccid17;
      OID *aoid;
      CID17 **apcid17;
      REGLINKS *alinks;
      int ccid124;
      CID124 **apcid124;
      int nVersion;
      int fReady;
};
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
void UpdateJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand);
void EmitJtGrabSparkles(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand);
struct SPLCS;
struct PO;
struct PROXYSRC;
struct CID116;
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
struct XP;
enum CRVK
{
};
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
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
struct VOL : public XFM
{
      BSPC bspc;
      GEOM geom;
      undefined4 unk_0x108[2];
      qword posCenter;
      float sRadius2;
      undefined4 unk_0x124[3];
};
struct SHAPE : public XFM
{
      CRVK crvk;
      int fClosed;
      int ccv;
      float *mpicvu;
      float *mpicvs;
      VECTOR4 *mpicvpos;
      undefined4 unk_0xf8[2];
      MATRIX4 matWorldCache;
      VECTOR4 *mpicvposA;
      VECTOR4 *mpicvposB;
      int nUnk148;
      float agsSeg[11];
      int nUnk178;
      void *pvUnk17c;
      struct SHAPERANGES *pranges;
      undefined4 unk_0x184[3];
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
struct CID34 : public XFM, public GRABSPOT
{
      DLE dle;
      float sUnk10c;
};
struct CID38 : public XFM, public GRABSPOT
{
      DLE dle;
      int nGrabState;
};
struct CID11 : public SHAPE, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x1bc;
      qword qwUnk1c0;
      int nUnk1d0;
      int nUnk1d4;
      undefined4 unk_0x1d8[2];
};
struct CID12 : public SHAPE, public GRABSPOT
{
      DLE dle;
      int nUnk1bc;
};
struct CID13 : public SHAPE, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x1bc;
      float gUnk1c0;
      float gUnk1c4;
      undefined4 unk_0x1c8[2];
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
struct CID27 : public VOL, public GRABSPOT
{
      DLE dle;
      LO *ploUnk15c;
};
struct CID39 : public XFM, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x10c;
};
struct CID35 : public XFM, public GRABSPOT
{
      DLE dle;
      LO *ploUser;
      float sUnk110;
      LO *ploRelay;
      int crsmg;
      RSMG arsmg[4];
      int ctaskpair;
      TASKPAIR ataskpair[4];
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
struct RAW8
{
    char ach[8];
};
struct MRSG
{
    float t;
    float dt;
};
struct ASEGBL;
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
};
struct PROXYSPARE
{
      struct PROXY *pproxy;
      PROXYSPARE *pspareNext;
      int fLocked;
      undefined4 unk_0xc;
      MATRIX4 mat;
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
PROXY *PproxyFromLo(LO *plo);
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
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLength(qword v)
{
    return GFromVf(QwLength(v));
}
struct HINTBOX;
extern "C" {
int FIsSwHero(STEPPLAYER *pstepplayer);
}
extern "C" {
int GetHintBoxTextId(HINTBOX *phintbox);
}
extern "C" {
void SetHintBoxText(HINTBOX *phintbox, int idText, float dt);
}
extern "C" {
void ClearIfSlotId(HINTBOX *phintbox, int idText);
}
extern float D_002BF660;
extern float D_002BF664;
extern float D_002BF70C;
extern HINTBOX D_003DA0C0;
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand);
void GatherJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, uint grf) __asm__("GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi");
void GatherJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, uint grf)
{
    JTGRABCAND ajtgrabcand[4];
    if (!FIsSwHero(pstepplayer))
        return;
    memset(ajtgrabcand, 0, sizeof(ajtgrabcand));
    uint i = 0;
    do
    {
        ajtgrabcand[i].nPriority = -1;
        ajtgrabcand[i].sDist = D_002BF664;
        i++;
    } while (i < 4);
    qword qwPosJt = PosLoWorld(pstepplayer);
    if (grf & 8)
    {
        DL *pdlpcid12 = &D_003DD490.dlCid12;
        for (CID12 *pcid12 = (CID12 *)pdlpcid12->head; pcid12; pcid12 = (CID12 *)pcid12->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid12, pcid12, 4, ajtgrabcand);
    }
    if (grf & 0x10)
    {
        DL *pdlpcid34 = &D_003DD490.dlCid34;
        for (CID34 *pcid34 = (CID34 *)pdlpcid34->head; pcid34; pcid34 = (CID34 *)pcid34->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid34, pcid34, 4, ajtgrabcand);
    }
    if (grf & 0x80)
    {
        DL *pdlpcid35 = &D_003DD490.dlCid35;
        for (CID35 *pcid35 = (CID35 *)pdlpcid35->head; pcid35; pcid35 = (CID35 *)pcid35->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid35, pcid35, 4, ajtgrabcand);
    }
    if (grf & 4)
    {
        DL *pdlpcid27 = &D_003DD490.dlCid27;
        for (CID27 *pcid27 = (CID27 *)pdlpcid27->head; pcid27; pcid27 = (CID27 *)pcid27->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid27, pcid27, 4, ajtgrabcand);
    }
    if (grf & 0x400)
    {
        DL *pdlpcid39 = &D_003DD490.dlCid39;
        for (CID39 *pcid39 = (CID39 *)pdlpcid39->head; pcid39; pcid39 = (CID39 *)pcid39->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid39, pcid39, 4, ajtgrabcand);
    }
    if (grf & 0x200)
    {
        DL *pdlpcid13 = &D_003DD490.dlCid13;
        for (CID13 *pcid13 = (CID13 *)pdlpcid13->head; pcid13; pcid13 = (CID13 *)pcid13->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid13, pcid13, 4, ajtgrabcand);
    }
    if (grf & 2)
    {
        DL *pdlpcid11 = &D_003DD490.dlCid11;
        for (CID11 *pcid11 = (CID11 *)pdlpcid11->head; pcid11; pcid11 = (CID11 *)pcid11->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid11, pcid11, 4, ajtgrabcand);
    }
    if (grf & 1)
    {
        DL *pdlTarget = &D_003DD490.dlTarget;
        for (TARGET *ptarget = (TARGET *)pdlTarget->head; ptarget; ptarget = (TARGET *)ptarget->dleTarget.next)
        {
            if ((ptarget->grftak & 1) && FIsBasicDerivedFrom(ptarget, (CID)19))
                AddJtGrabCandidate(psw, pstepplayer, qwPosJt, ptarget, (CID19 *)ptarget, 4, ajtgrabcand);
        }
    }
    if (grf & 0x20)
    {
        DL *pdlpcid38 = &D_003DD490.dlCid38;
        for (CID38 *pcid38 = (CID38 *)pdlpcid38->head; pcid38; pcid38 = (CID38 *)pcid38->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid38, pcid38, 4, ajtgrabcand);
    }
    int ccand;
    for (ccand = 4; ccand > 0 && !ajtgrabcand[ccand - 1].plo; ccand--)
        ;
    SW *pswG = &D_003DD490;
    EmitJtGrabSparkles(pswG, pstepplayer, ccand, ajtgrabcand);
    UpdateJtGrabCandidates(pswG, pstepplayer, ccand, ajtgrabcand);
    int idHintOld = GetHintBoxTextId(&D_003DA0C0);
    if (idHintOld != -1 && idHintOld != pswG->idHintGrab)
        return;
    int idHint;
    int fFar;
    if (pswG->cjtgrabcand == 0)
        idHint = -1;
    else if (fFar = D_002BF660 < GLength(QwSub(qwPosJt, pswG->ajtgrabcand[0].qwPos)), fFar)
        idHint = -1;
    else
    {
        PROXY *pproxy = pswG->ajtgrabcand[0].ploUnk10 ? PproxyFromLo(pswG->ajtgrabcand[0].ploUnk10) : 0;
        if (pproxy && pproxy->oid == 0x624 && D_002BF70C < pswG->ajtgrabcand[0].tUnk14)
            idHint = 0x125;
        else
            idHint = D_003DD490.ajtgrabcand[0].pspot->nUnk14;
    }
    if (idHint != idHintOld)
    {
        if (idHint == -1)
            ClearIfSlotId(&D_003DA0C0, D_003DD490.idHintGrab);
        else
            SetHintBoxText(&D_003DA0C0, idHint, -1.0f);
        D_003DD490.idHintGrab = idHint;
    }
}
