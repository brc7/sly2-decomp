// func_001CC5E8: ported from Lilly's personal Sly 2 decomp (src/P2/jthands.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C1ED0 (D_002C1ED0)
// 0x001CC5E8 InitJtHands__FP7JTHANDSP2SOPP5ASEGA
// Builds the hands record (the hero's VUnk14 and InitJtGrabRecord call it once ploHand is set): the matrix of the hand object's XF chain up to the first
// rotation XF (xfk 6) - the hand's frame in the hero's - and, for each hand that has a hold clip, two curves (class 10 SHAPEs of D_002C1ED0 points, removed
// from the world again): the path the hand object's XF (A) and the chain's top XF (B) take through the clip, sampled with PredictAsegEffect and carried to
// the hand's frame. With both clips the blend of the two (weights 1 and 0 to start with) is made.
typedef unsigned  size_t;
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
inline void SetMatrix4Identity(MATRIX4 *pmat)
{
    memset(pmat, 0, sizeof(MATRIX4));
    pmat->mat[3][3] = 1.0f;
    pmat->mat[2][2] = 1.0f;
    pmat->mat[1][1] = 1.0f;
    pmat->mat[0][0] = 1.0f;
}
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR);
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
};
typedef int GRFAPL;
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
struct ACT;
struct XF
{
      MATRIX4 mat;
      MATRIX4 matWorld;
      float gUnk80;
      float gUnk84;
      float *pgUnk88;
      float *pgUnk8c;
      ACT *pactFirst;
      int xfk;
      short nRole;
      short nPhase;
      OID oid;
      XF *pxfParent;
      int nStampWorld;
      XF *pxfRef;
      short nUnkAc;
      short nUnkAe;
      VECTOR4 vUnkB0;
      qword qwRest;
      qword qwUnkD0;
      qword qwRestUnkE0;
      qword qwUnkF0;
};
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
};
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
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc);
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
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
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
LO *PloNew(CID cid, SO *psoParent, OID oid, int isplice);
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
extern "C" void PredictAsegEffect(float tLocal, ASEG *paseg, SO *psoRoot, float svt, XF *pxfEffect, XF *pxfRef, MATRIX4 *pmatOut, int f);
extern "C" void func_001E6180(SHAPE *pshape);
extern int D_002C1ED0;
inline void *PvAllocSw(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
void InitJtHands(JTHANDS *phands, SO *pso, ASEGA **ppasega) __asm__("func_001CC5E8");
/** @sly2-annot addr=0x1CC5E8
 * Purpose: Build hands record: hand-frame matrix, sampled A/B curves per hold clip (PredictAsegEffect), blend
 * Info: proposed name InitJtHands (T1); TU jthands
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void InitJtHands(JTHANDS *phands, SO *pso, ASEGA **ppasega)
{
    phands->pso = pso;
    phands->ppasega = ppasega;
    if (!phands->ploHand)
        return;
    MATRIX4 matHand;
    XF *pxfHand = phands->ploHand->PxfLocal(&matHand);
    phands->matChain = matHand;
    phands->pxf = pxfHand;
    while (phands->pxf->xfk != 6)
    {
        phands->matChain = phands->pxf->mat * phands->matChain;
        phands->pxf = phands->pxf->pxfParent;
    }
    int cHands = 0;
    for (int i = 0; i < 2; i++)
    {
        ASEG *paseg = phands->abl[i].paseg;
        if (!paseg)
            continue;
        cHands++;
        for (int j = 0; j < 2; j++)
        {
            SHAPE *pshape = (SHAPE *)PloNew((CID)10, phands->pso, (OID)0xB2, -1);
            pshape->crvk = (CRVK)0;
            pshape->ccv = D_002C1ED0;
            pshape->mpicvu = (float *)PvAllocSw(D_002C1ED0 << 2, 16);
            pshape->mpicvs = (float *)PvAllocSw(D_002C1ED0 << 2, 16);
            pshape->mpicvpos = (VECTOR4 *)PvAllocSw(D_002C1ED0 << 4, 16);
            MATRIX4 matSample;
            XF *pxfEffect;
            if (j == 0)
            {
                matSample = matHand;
                pxfEffect = pxfHand;
            }
            else
            {
                pxfEffect = phands->pxf;
                SetMatrix4Identity(&matSample);
            }
            for (int k = 0; k < D_002C1ED0; k++)
            {
                MATRIX4 matOut;
                float u = (float)k / (float)(D_002C1ED0 - 1);
                PredictAsegEffect(u * paseg->tMax, paseg, phands->pso, 0.0f, pxfEffect, phands->pso->pxfRoot, &matOut, 0);
                pshape->mpicvu[k] = u;
                qword qwPos = QwTransformPos(&matOut, *(qword *)matSample.mat[3]);
                pshape->mpicvpos[k] = *(VECTOR4 *)&qwPos;
            }
            pshape->Remove();
            func_001E6180(pshape);
            if (j == 0)
                phands->abl[i].pshapeA = pshape;
            else
                phands->abl[i].pshapeB = pshape;
        }
    }
    if (cHands == 2)
    {
        phands->abl[0].uWeight = 1.0f;
        EnsureAsegBlendDynamic((ALO *)phands->pso, 0x14, 2, phands->abl, 0, 0, 0, &phands->pasegbl);
    }
}
