// PostJtLoadGait: ported from Lilly's personal Sly 2 decomp (src/P2/jt.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C3DC0 (D_002C3DC0), D_002C3DC8 (D_002C3DC8), D_002C3DD0 (D_002C3DD0), D_002C3DD8 (D_002C3DD8)
// 0x0020B868 PostJtLoadGait__FP4GAIT  (our name)
// After the level load (JT::VUnk14, for the gaits at JT+0x1320 and +0x1C30): the foot markers of both clips (FillBlAmrsg), each clip's root movement per second
// ((position at the end - position at the start) / length), the blend (EnsureAsegBlendDynamic with the two marker arrays). Then every blended channel is told
// which marker array keeps it in step: XFs with oid 0x10 / 0x12 / 0x14 the second foot's (amrsgc[1]), 0x11 / 0x13 / 0x15 the first's (the default), any other none.
typedef unsigned char    uint8_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
typedef uint32_t    uint;
typedef uint64_t    ulong;
typedef uint    undefined4;
typedef unsigned long long qword __attribute__((mode(TI)));
struct CBinaryInputStream;
enum OID
{
};
struct SO;
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct MATRIX4
{
	float mat[4][4];
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
struct BASIC
{
      CRefPtr<CFrame> pframe;
      CRefPtr<CSidebag> psidebag;
      struct REFNODE *prefnodeFirst;
};
class LO;
struct ALO;
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
    virtual void Load(CBinaryInputStream *pbis);
    static void *operator new(uint cb)
    {
        SWALLOC swalloc;
        swalloc.cb = cb;
        SWALLOC *pswalloc = &swalloc;
        pswalloc->cbAlign = 0x10;
        return PvAllocSwClearImpl(pswalloc);
    }
};
struct VECKEY : public KEY
{
    virtual void CacheSeg(XF *pxf);
    virtual void GetValue(float t, float svt, qword *ppos, qword *pv);
};
struct SCALARKEY : public KEY
{
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
void FindAsegChannelTarget(ASEG *paseg, ALO *paloRoot, CHN *pchn, SO **ppso, XF **ppxf);
CHN *PchnFindAseg(ASEG *paseg, ALO *palo, int xfk, int fStripped);
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
void FillBlAmrsg(int cbBl, int cbl, void *abl, int ibMrsg, int coid, OID *aoid);
void ReblendAsegbl(ASEGBL *pasegbl);
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
struct VKBLEND : public VECKEY
{
      ASEGBL *pasegbl;
      MRSGC *amrsgc;
      KEY **apkey;
      qword qwRest;
    virtual int NKeyType() { return 5; }
};
struct SKBLEND : public SCALARKEY
{
      ASEGBL *pasegbl;
      MRSGC *amrsgc;
      KEY **apkey;
      float gRest;
};
struct VU_FLOAT
{
    qword qw;
};
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwDivVu(qword qw, VU_FLOAT g)
{
    qword qwRet;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwRet) : "j"(g.qw), "j"(qw));
    return qwRet;
}
extern OID D_002C3DC0[2];
extern OID D_002C3DC8[2];
extern int D_002C3DD0[2];
extern int D_002C3DD8[2];
void PostJtLoadGait(GAIT *pgait) __asm__("PostJtLoadGait");
void PostJtLoadGait(GAIT *pgait)
{
    if (pgait->abl[0].paseg == 0)
        return;
    FillBlAmrsg(0x40, 2, pgait->abl, 0x20, 2, D_002C3DC0);
    FillBlAmrsg(0x40, 2, pgait->abl, 0x30, 2, D_002C3DC8);
    for (uint ibl = 0; ibl < 2; ibl++)
    {
        BLGAIT *pblgait = &pgait->abl[ibl];
        ASEG *paseg = pblgait->paseg;
        if (paseg != 0)
        {
            CHN *pchn = PchnFindAseg(paseg, (ALO *)pgait->pstepplayer, 1, 1);
            VECTOR4 vStart, vEnd;
            ((VECKEY *)pchn->pkey)->GetValue(0.0f, 1.0f, (qword *)&vStart, 0);
            ((VECKEY *)pchn->pkey)->GetValue(paseg->tMax, 1.0f, (qword *)&vEnd, 0);
            qword qwDiff = QwSub(*(qword *)&vEnd, *(qword *)&vStart);
            VU_FLOAT vuT;
            __asm__("qmtc2 %1, %0" : "=j"( vuT.qw ) : "rV"(  *(int *)&paseg->tMax )) ;
            pblgait->vRoot = QwDivVu(qwDiff, vuT);
        }
    }
    EnsureAsegBlendDynamic((ALO *)pgait->pstepplayer, 0x40, 2, pgait->abl, 2, D_002C3DD8, D_002C3DD0, &pgait->pasegbl);
    for (int ichn = 0; ichn < pgait->pasegbl->cchn; ichn++)
    {
        CHN *pchn = &pgait->pasegbl->achn[ichn];
        VKBLEND *pvkblend;
        SKBLEND *pskblend;
        if (pchn->fScalar)
        {
            pvkblend = 0;
            pskblend = (SKBLEND *)pchn->pkey;
        }
        else
        {
            pvkblend = (VKBLEND *)pchn->pkey;
            pskblend = 0;
        }
        SO *pso;
        XF *pxf;
        FindAsegChannelTarget(pgait->pasegbl, (ALO *)pgait->pstepplayer, pchn, &pso, &pxf);
        switch (pxf->oid)
        {
            case 0x10:
            case 0x12:
            case 0x14:
                if (pvkblend != 0)
                    pvkblend->amrsgc = &pgait->pasegbl->amrsgc[1];
                if (pskblend != 0)
                    pskblend->amrsgc = &pgait->pasegbl->amrsgc[1];
                break;
            case 0x11:
            case 0x13:
            case 0x15:
                break;
            default:
                if (pvkblend != 0)
                    pvkblend->amrsgc = 0;
                if (pskblend != 0)
                    pskblend->amrsgc = 0;
                break;
        }
    }
    ReblendAsegbl(pgait->pasegbl);
}
