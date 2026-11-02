// func_0016A000 = LoadSubglobType0FromBrx (name from Lilly's personal decomp, decomp/src/P2/glob.c T1082 draft). MATCH (T1701).
// Built on lilly4 (T1697) / lilly3 (T1693); see notes/160000.md for the tricks that matched it, the last ones being:
//  - the header flags word (BATCHHDR0 +0x8) is a one-member struct, set from an aggregate initializer in the ibatch
//    switch: the clear-then-set keeps loop.c from hoisting the constant with the other 1s;
//  - the lit-batch count is Min(0x27, cvtx - iofs) through an inline Min (no REG_EQUAL on n's first set);
//  - ppart->iqwVtx is re-read for iqwEnd/iqwOwn; pg[0] = pg[1] = pg[2] = -1.0f.
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
struct SO;
struct SAA;
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
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
union QW
{
    VECTOR vec;
    VECTOR4 vec4;
    ulong ull;
    ulong aul[2];
    uint aun[4];
    ushort aus[8];
    uchar ab[16];
    long ll;
    long al[2];
    int an[4];
    short as[8];
    float ag[4];
};
class DMAS
{
public:
      uchar* m_pbMax;
      uchar* m_ab;
      QW* m_pqwCnt;
      uchar* m_pb;
      int m_fPad;
      int m_fEndPrim;
    void AllocStack(int cqw);
    void DetachCopySw(int *pcqw, QW **paqw, QW **paqwCopy);
    void AddDmaCnt();
    void AddDmaRefs(int cqw, QW *aqw);
    void AddDmaRet();
    void EndDmaCnt();
    virtual void EndPrim(int fEOP);
};
inline void AddDmaInt(DMAS *pdmas, int val)
{
    *(int *)pdmas->m_pb = val;
    pdmas->m_pb += 4;
}
enum UPK
{
    UPK_S16 = 1,
    UPK_S8 = 2,
    UPK_V3_32 = 8,
    UPK_V4_32 = 12,
    UPK_V4_8 = 14,
    UPK_V4_5 = 15,
};
class VIFS : public DMAS
{
public:
      uint m_unMask;
      int m_fCheckCnt;
    VIFS();
    void Align(int iun);
    void AddVifMscal(void *pv);
    void AddVifStcycl(int wl, int cl);
    void AddVifStrow(QW *pqw);
    void AddVifStmask(uint unMask);
    void UnpackHelper(UPK upk, int c, int iqw, int *pcb, uint **ppun);
    void AddVifUnpack(UPK upk, int c, void *pvSrc, int iqw);
};
struct BMP;
struct CLUT;
enum OID
{
};
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
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
void InitSl(SL *psl, int ibSle);
void AppendSlEntry(SL *psl, void *pv);
struct BASIC;
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
struct XF;
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    void Read(int cb, void *pv);
    uchar U8Read();
    char S8Read();
    short S16Read();
    int S32Read();
    float F32Read();
    void ReadVector(VECTOR *pvec);
    void ReadMatrix4(MATRIX4 *pmat);
};
typedef struct SHD;
typedef struct SHDP;
struct TEXF
{
    ushort OID;
    short grdtex;
    uchar cibmp;
    uchar ciclut;
};
struct TEX : public TEXF
{
    ushort unk_0;
    SHD *pshd;
    BMP **apbmp;
    CLUT **apclut;
};
struct SHDF
{
      int oid;
      uchar shdk;
      uchar grfshd;
      uchar unk_6;
      uchar unk_7;
      RGBA rgba;
      RGBA rgbaVolume;
      RGBA rgba2;
      ushort OidAltSat;
      uchar rp;
      uchar ctex;
};
struct SHD : public SHDF
{
    TEX *atex;
    int cshdp;
    SHDP *ashdp;
    int cframe;
    SAA *psaa;
};
struct WRBG;
struct BND
{
      OID oid;
      LO *plo;
      int ixf;
      XF *pxf;
      MATRIX4 matInv;
};
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
struct BATCH
{
      int cvtx;
      SHD *pshd;
      int grf;
      SAA *psaa;
      int ivtxOwn;
      int *aivtx;
      void *pvPos;
      void *pvUv;
      void *pvUvLink;
      int obColorA;
      int obColorB;
      uchar *pbAlpha;
      int obAlpha;
      uchar *pbRgb;
      uchar *pbNorm;
      uchar *pbWeights;
};
struct SPECPARAMS
{
    float agUnk00[3];
    float gPad0c;
    float agUnk10[2];
    int iSpec;
    int fMode;
    VECTOR vec;
    float gPad2c;
} __attribute__((aligned(16)));
struct BATCHFILE
{
    int cvtx;
    int ishd;
    uint grf;
};
struct BATCHPART0
{
      ushort iqwVtx;
      ushort iqwEnd;
      ushort iqwUv;
      ushort cvtxFlag;
      ushort aiqwShdp[4];
      ushort iqwOwn;
      ushort iqwSkin;
      ushort iqwSaa;
      ushort iqwSpec;
      ushort oidAltSat;
      ushort iqwUvLink;
      ushort iqwWeights;
      ushort cframe;
};
// BATCHHDR0 +0x8 flags word.
struct BATCHGRF
{
      ushort grf;
};
struct BATCHHDR0
{
      ushort unk_0x00;
      ushort oid;
      ushort unk_0x04;
      ushort iqwShadow;
      BATCHGRF grf;
      ushort iqwBase;
      ushort grfWarp;
      ushort iqwWarp;
      ushort aiqwBone[4];
      BATCHPART0 apart[2];
};
struct VERTREC
{
      int ivtxFile;
      BATCH *pbatchFirst;
      int ivtxFirst;
      BATCH *pbatch;
      int ivtx;
};
struct WRBGBATCH
{
    int cvtx;
    void *pvPos;
    void *apvWarp[4];
    WRBGBATCH *pwrbgbatchNext;
};
struct BATCHREC
{
    int cvtx;
    uint grf;
    void *pvUv;
    void *pvPos;
    void *pvUvLink;
    void *pbRgb;
    void *pbNorm;
    int unk_0x1c;
};
struct BATCHRECS
{
    SUBGLOB *psubglob;
    int cbatch;
    BATCHREC *abatchrec;
    BATCHRECS *pbatchrecsNext;
};
struct CLUTCOLOR
{
    ushort ucA;
    ushort ucB;
    uchar bAlpha;
};
struct CLUTPAL
{
    int unk_0x00;
    CLUTCOLOR *acolor;
};
struct CLUTSET
{
    int cpal;
    CLUTPAL *apal;
    int *aipal;
};
struct SUBGLOBLOAD
{
    float gUnk00;
    int nUnk04;
    int nUnk08;
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
struct CM;
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
extern CM D_002DDF40;
void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase);
struct SWALLOC
{
    int cb;
    int cbAlign;
    SWALLOC(int cbArg, int cbAlignArg)
    {
        cb = cbArg;
        cbAlign = cbAlignArg;
    }
};
void *PvAllocSwImpl(SWALLOC *pswalloc);
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
void *PvAllocStackClearImpl(SWALLOC *pswalloc);
void *PvAllocStackImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
void CopyAb(void *pvDst, void *pvSrc, uint cb);
struct UVF
{
    float u, v;
};
struct WR;
enum WEK
{
};
struct WEKI
{
    WEK wek;
    float sInner;
    float uInner;
    float sOuter;
    float uOuter;
    undefined1 unknown[12];
    MATRIX4 dmat;
};
struct WRBG
{
    SO *pso;
    GLOB *pglob;
    OID oid;
    WR *pwr;
    int cmat;
    int fDpos;
    int fDuv;
    ushort grf;
    int iqw;
    undefined1 unknown[12];
    WEKI weki;
    SL slBatch;
    WRBG *pwrbgNextGlobset;
    WRBG *pwrbgNextWr;
};
extern char D_00000000[];
extern "C" {
void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("func_0016A000");
}
static inline void *PvAllocSw(int cb)
{
    SWALLOC swalloc(cb, 0x10);
    return PvAllocSwImpl(&swalloc);
}
static inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOC swalloc(cb, 0x10);
    return PvAllocSwCopyImpl(&swalloc, pvBase);
}
SHD *PshdFromIshd(int ishd);
int AddVifUnpackShdp(SHD *pshd, int ishdp, int iqw, VIFS *pvifs);
extern uint D_002BE540[];
extern int D_002BE558[];
extern QW D_002DEB60;
void CalcRecipFitParams(float gScale, CM *pcm, float *pgOut);
CLUTSET *LoadClutSetFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis);
void LoadSubglobInstancesFromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBX *psubglobx, int cSubExtra, int cbatch, int *acbatch, BATCH **apbatch, CBinaryInputStream *pbis);
int CompareVertRec(const void *pv1, const void *pv2);
void CopyDmaRefsChainSw(SUBGLOB *psubglob, GLOB *pglob, int cqw, void *pvBase, int cqwMain, char *pbNew, char *pbOld);
extern "C" {
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
}
extern "C" void func_00117F90(CBinaryInputStream *pbis, void *pv);
extern "C" void func_00117FB0(CBinaryInputStream *pbis, void *pv);
void FixupRgba(RGBA *prgba);
extern int D_003E11EC;
static inline void *PvAllocStackP(int cb)
{
    SWALLOC swalloc(cb, 0x10);
    return PvAllocStackImpl(&swalloc);
}
static inline void *PvAllocStackClearP(int cb)
{
    SWALLOC swalloc(cb, 0x10);
    return PvAllocStackClearImpl(&swalloc);
}
static inline void *PvAllocSwClearP(int cb)
{
    SWALLOC swalloc(cb, 0x10);
    return PvAllocSwClearImpl(&swalloc);
}
extern "C" SHD *PshdFromOid(int oid);
extern "C" int func_001EC6D0(SHD *pshd, int ishdp, int iqw, VIFS *pvifs);
extern "C" int func_001ED960(SAA *psaa, int iqw, VIFS *pvifs);
extern "C" void func_001EC740(int iFirst, int iEnd, int cb, void *pvOld, void *pvNew);
extern int D_003DA790;
extern int D_003E12D0;
extern int D_002BE580;
extern int D_002DEB54;
extern float D_002A52A0;
extern char D_00003628[];
extern char D_000037A8[];
extern char D_00002EF8[];
extern char D_00002E20[];
extern char D_00002F80[];
extern char D_00002E68[];
extern char D_00002FC8[];
extern char D_00002EB0[];
extern char D_00003358[];
extern char D_00002EE8[];
struct SAAVIRT
{
    int unk_0x0[3];
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void V5();
    virtual void V6();
    virtual void V7();
    virtual void V8();
    virtual bool V9(SHD *pshd);
};
struct SAAFIELDS : public SAAVIRT
{
    int unk_0x10[4];
    uint grf;
    int unk_0x24[8];
    int cqwMain;
    void *pvMain;
};
static inline int CwtFromSubglob(SUBGLOB *psubglob)
{
    int cbnd = psubglob->cbnd;
    int cwt = 4;
    if (cbnd < 4)
        cwt = cbnd;
    return cwt;
}
static inline void RelocPpv(char *pbOld, int cb, char *pbNew, char **ppb)
{
    if (ppb != 0 && *ppb >= pbOld && *ppb < pbOld + cb)
        *ppb += pbNew - pbOld;
}
static inline char *PbReloc(char *pbOld, int cb, char *pbNew, char *pb)
{
    char *pbRet;
    if (pb != 0 && pb >= pbOld && pb < pbOld + cb)
        pbRet = pb + (pbNew - pbOld);
    else
        pbRet = pb;
    return pbRet;
}
struct RGBA5551 { ushort r : 5; ushort g : 5; ushort b : 5; ushort a : 1; };
struct UVLINK { short u, v; ushort link; } __attribute__((packed));
static inline int Min(int a, int b) { return a < b ? a : b; }
/** @sly2-annot addr=0x16A000
 * Purpose: Sub-glob type 0 loader: textured lit triangle strips (the visible mesh); builds VIF batches
 * Info: proposed name LoadSubglobType0FromBrx (T1); TU glob
 * Notes: sub-glob type->loader correction (CONFLICTS #16, MESH_FORMAT, level_parser type switch 0x167748): v1
 *   had the types permuted | level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of
 *   bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+meshes(MESH_FORMAT jal switch
 *   0x167750)+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#2-globset-loadglobsetfrombrx-0x167320-matched-cparse,
 *   docs/formats/meshes-scene.md#type-0-the-visible-textured-mesh-loadsubglobtype0frombrx-0x16a000-cparse
 * @end-sly2-annot */
void LoadSubglobType0FromBrx(GLOBSET *const pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis)
{
    psubglob->bUnk02 = pbis->U8Read();
    int isaa = pbis->S16Read();
    if (isaa >= 0)
        pglob->psaa = pglobset->apsaa[isaa];
    if (psubglob->bUnk02 & 1)
        pglob->grf |= 0x100;
    if (psubglob->bUnk02 & 2)
        pglob->grf |= 0x200;

    STACKSCOPE scopeOuter;
    VIFS vifs;
    VIFS vifsAux;
    vifs.AllocStack(0x2000);
    QW *pqwChain = (QW *)vifs.m_pb;
    pglob->iUnkFirst = D_003DA790;
    if (psubglob->bUnk02 & 1)
    {
        vifs.AddDmaCnt();
        pglobset->fBoundsUnk32 = 1;
    }
    else
    {
        vifsAux.AllocStack(0x800);
        if (FIsBasicDerivedFrom((BASIC *)pglobset->pso, (CID)0x31) && !(psubglob->bUnk02 & 4) && (uchar)(psubglob->bUnk00 - 7) >= 2 &&
            psubglob->gUnk04 == D_002A52A0 && D_003E12D0 == 0)
        {
            QW qwOne;
            qwOne.ag[0] = 1.0f;
            qwOne.ag[1] = 1.0f;
            qwOne.ag[2] = 0.0f;
            qwOne.ag[3] = 0.0f;
            pglob->grf |= 0x1000;
            vifsAux.AddDmaCnt();
            vifsAux.AddVifUnpack(UPK_V4_32, 4, &pglob->pxf->matWorld, 0);
            vifsAux.AddVifUnpack(UPK_V4_32, 1, &qwOne, 8);
            vifsAux.EndDmaCnt();
        }
    }
    float *pgFit;
    vifs.UnpackHelper(UPK_V4_32, 1, 9, 0, (uint **)&pgFit);
    CalcRecipFitParams(psgl->gUnk00, &D_002DDF40, pgFit);

    psubglob->cbnd = pbis->U8Read();
    pglob->abnd = (BND *)PvAllocSwClearP(psubglob->cbnd * sizeof(BND));
    for (int ibnd = 0; ibnd < psubglob->cbnd; ibnd++)
    {
        BND *pbnd = &pglob->abnd[ibnd];
        pbnd->oid = (OID)pbis->S32Read();
        pbnd->ixf = pbis->S16Read();
    }
    int iqwBase = (psubglob->bUnk02 & 1) ? 0x21 : 0x15;
    if (psubglob->cbnd != 0)
    {
        psubglob->iqwBone = iqwBase;
        iqwBase += psubglob->cbnd * 4;
    }
    SPECPARAMS spec;
    spec.iSpec = pbis->S8Read();
    if (spec.iSpec != -1)
    {
        func_00117F90(pbis, &spec.agUnk00);
        func_00117FB0(pbis, &spec.agUnk10);
        pbis->ReadVector(&spec.vec);
        spec.agUnk00[1] *= 0.007874016f;
        spec.agUnk00[2] *= 6.200012e-05f;
        spec.fMode = psubglob->bUnk02 & 1;
    }
    CLUTSET *pclutset = LoadClutSetFromBrx(pglobset, pbis);
    if (psubglob->bUnk02 & 4)
    {
        WRBG wrbg;
        memset(&wrbg, 0, sizeof(WRBG));
        wrbg.oid = (OID)pbis->S32Read();
        wrbg.weki.wek = (WEK)pbis->S8Read();
        if (wrbg.weki.wek != -1)
        {
            wrbg.weki.sInner = pbis->F32Read();
            wrbg.weki.uInner = pbis->F32Read();
            wrbg.weki.sOuter = pbis->F32Read();
            wrbg.weki.uOuter = pbis->F32Read();
            pbis->ReadMatrix4(&wrbg.weki.dmat);
        }
        wrbg.cmat = pbis->U8Read();
        wrbg.fDpos = pbis->U8Read();
        wrbg.fDuv = pbis->U8Read();
        WRBG *pwrbg = (WRBG *)PvAllocSwCopy(sizeof(WRBG), &wrbg);
        pwrbg->pso = pglobset->pso;
        pwrbg->pglob = pglob;
        pwrbg->grf = pwrbg->cmat;
        if (pwrbg->fDpos != 0)
            pwrbg->grf = pwrbg->cmat | 0x100;
        if (pwrbg->fDuv != 0)
            pwrbg->grf |= 0x200;
        InitSl(&pwrbg->slBatch, 0x18);
        pwrbg->iqw = iqwBase;
        if (pwrbg->fDpos != 0)
            iqwBase += pwrbg->cmat * 4;
        if (pwrbg->fDuv != 0)
            iqwBase += pwrbg->cmat * 4;
        pglob->pwrbg = pwrbg;
        pwrbg->pwrbgNextGlobset = pglobset->pwrbgFirst;
        pglobset->pwrbgFirst = pwrbg;
    }
    int cbatch = pbis->S16Read();
    int nLit = 0;
    VIFS vifsLit;
    if (!(psubglob->bUnk02 & 1))
    {
        vifsLit.AllocStack(0x2000);
        vifsLit.AddDmaCnt();
    }
    int *acbatch = (int *)PvAllocStackClearP(cbatch * 4);
    BATCH **apbatch = (BATCH **)PvAllocStackClearP(cbatch * 4);
    void *apvLit[1024];
    for (int ibatch = 0; ibatch < cbatch; ibatch++)
    {
        int cbatchThis = pbis->U8Read();
        BATCH *abatch = (BATCH *)PvAllocStackClearP(cbatchThis << 6);
        acbatch[ibatch] = cbatchThis;
        apbatch[ibatch] = abatch;
        BATCHHDR0 *phdr;
        vifs.UnpackHelper((UPK)0xd, 0xb, 10, 0, (uint **)&phdr);
        memset(phdr, 0, sizeof(BATCHHDR0));
        switch (ibatch)
        {
        case 0:
        {
            BATCHGRF grf = {1};
            phdr->grf = grf;
            switch (psgl->nUnk08)
            {
            case 0:
                phdr->grf.grf |= 0x100;
                break;
            case 1:
                phdr->grf.grf |= 0x200;
                break;
            case 2:
                phdr->grf.grf |= 0x400;
                break;
            }
            break;
        }
        case 1:
        {
            BATCHGRF grf = {4};
            phdr->grf = grf;
            break;
        }
        }
        phdr->oid = (ushort)pglob->oid;
        if (psubglob->bUnk02 & 1)
            phdr->grf.grf |= 2;
        if (psubglob->bUnk02 & 0x10)
            phdr->grf.grf |= 0x80;
        if (pglob->pwrbg != 0)
        {
            phdr->grfWarp = pglob->pwrbg->grf;
            phdr->iqwWarp = pglob->pwrbg->iqw;
        }
        if (psubglob->cbnd != 0)
            phdr->iqwWarp = psubglob->iqwBone;
        int cvtxTotal = 0;
        for (int ib = 0; ib < cbatchThis; ib++)
        {
            BATCH *pbatch = &abatch[ib];
            BATCHFILE bf;
            pbis->Read(sizeof(BATCHFILE), &bf);
            pbatch->cvtx = bf.cvtx;
            pbatch->pshd = PshdFromIshd(bf.ishd);
            pbatch->grf = bf.grf;
            SAAFIELDS *psaaGlob = (SAAFIELDS *)pglob->psaa;
            if (psaaGlob != 0)
            {
                if (psaaGlob->V9(pbatch->pshd))
                    pbatch->psaa = pglob->psaa;
            }
            if (pbatch->psaa == 0)
            {
                SAAFIELDS *psaaShd = (SAAFIELDS *)pbatch->pshd->psaa;
                if (psaaShd != 0)
                {
                    if (psaaShd->V9(pbatch->pshd))
                        pbatch->psaa = pbatch->pshd->psaa;
                }
            }
            if (pbatch->psaa != 0)
            {
                if (((SAAFIELDS *)pbatch->psaa)->grf & 1)
                    pbatch->grf |= 0x10;
                if (((SAAFIELDS *)pbatch->psaa)->grf & 2)
                    pbatch->grf |= 8;
            }
            if (pbatch->grf & 0x20)
                pbatch->aivtx = (int *)PvAllocStackClearP(pbatch->cvtx << 2);
            cvtxTotal += pbatch->cvtx;
            if (D_002DEB54 != 0)
                pbatch->grf &= ~2;
            switch (pbatch->pshd->shdk)
            {
            case 3:
                pbatch->grf |= 6;
                break;
            case 7:
                if (!(pbatch->pshd->grfshd & 0x20))
                    pbatch->grf |= 4;
                break;
            }
        }
        STACKSCOPE scope;
        int cvtxFile = pbis->U8Read();
        VECTOR *aposFile = (VECTOR *)PvAllocStackP(cvtxFile * sizeof(VECTOR));
        pbis->Read(cvtxFile * sizeof(VECTOR), aposFile);
        UVF *auvFile = (UVF *)PvAllocStackP(cvtxFile * sizeof(UVF));
        pbis->Read(cvtxFile * sizeof(UVF), auvFile);
        VECTOR *anormFile = (VECTOR *)PvAllocStackP(cvtxFile * sizeof(VECTOR));
        pbis->Read(cvtxFile * sizeof(VECTOR), anormFile);
        RGBA *argbaFile = (RGBA *)PvAllocStackP(cvtxFile * sizeof(RGBA));
        pbis->Read(cvtxFile * sizeof(RGBA), argbaFile);
        for (RGBA *prgba = argbaFile; prgba < argbaFile + cvtxFile; prgba++)
            FixupRgba(prgba);
        int fAlpha = pbis->S8Read();
        if (psubglob->bUnk00 < 9)
            fAlpha = (psubglob->bUnk00 < 3) ? fAlpha : 1;
        void *pvWeights;
        if (psubglob->cbnd != 0)
        {
            for (int iwt = 0; iwt < 4; iwt++)
                phdr->aiqwBone[iwt] = psubglob->iqwBone + pbis->U8Read() * 4;
            pvWeights = PvAllocStackP(CwtFromSubglob(psubglob) * cvtxFile);
            pbis->Read(CwtFromSubglob(psubglob) * cvtxFile, pvWeights);
        }
        else
        {
            pvWeights = 0;
        }
        uchar *pbVtx = (uchar *)PvAllocStackP(cvtxTotal);
        uint *punBits;
        uint un = 1;
        pbis->Read(cvtxTotal, pbVtx);
        int cbBits = ((cvtxTotal + 31) / 32) * 4;
        int fMulti = 0;
        uint *aunBits = (uint *)PvAllocStackP(cbBits);
        pbis->Read(cbBits, aunBits);
        punBits = aunBits;
        if ((psubglob->bUnk02 & 5) || psubglob->cbnd != 0 || spec.iSpec != -1)
            fMulti = 1;
        int fNoSaa = !fMulti && abatch[0].psaa == 0 && abatch[1].psaa == 0;
        int iqw = iqwBase;
        phdr->iqwBase = iqw;
        VERTREC *arec;
        if (psubglob->cbnd == 0 && !(psubglob->bUnk02 & 1) && spec.iSpec == -1)
        {
            arec = 0;
        }
        else
        {
            if (psubglob->bUnk02 & 8)
            {
                uchar *pbV = pbVtx;
                arec = (VERTREC *)PvAllocStackP(cvtxTotal * sizeof(VERTREC));
                VERTREC *prec = arec;
                for (int ib = 0; ib < cbatchThis; ib++)
                {
                    BATCH *pbatch = &abatch[ib];
                    pbatch->ivtxOwn = 0;
                    for (int iv = 0; iv < pbatch->cvtx; iv++)
                    {
                        prec->ivtxFile = *pbV++;
                        prec->ivtxFirst = iv;
                        prec->ivtx = iv;
                        prec->pbatch = pbatch;
                        prec->pbatchFirst = pbatch;
                        prec++;
                    }
                }
            }
            else
            {
                VERTREC **apvtx = (VERTREC **)PvAllocStackClearP(cvtxFile << 2);
                arec = (VERTREC *)PvAllocStackP(cvtxTotal * sizeof(VERTREC));
                VERTREC *prec = arec;
                for (int ib = 0; ib < cbatchThis; ib++)
                    abatch[ib].ivtxOwn = abatch[ib].cvtx;
                uchar *pb = pbVtx + cvtxTotal;
                BATCH *pbatchFirst = &abatch[cbatchThis - 1];
                for (int ib = cbatchThis - 1; ib >= 0; ib--)
                {
                    BATCH *pbatch = &abatch[ib];
                    int iv = pbatch->cvtx;
                    while (--iv >= 0)
                    {
                        int ivtxFile = *--pb;
                        if (apvtx[ivtxFile] != 0)
                        {
                            *prec = *apvtx[ivtxFile];
                        }
                        else
                        {
                            prec->pbatchFirst = pbatchFirst;
                            prec->ivtxFile = ivtxFile;
                            prec->ivtxFirst = --pbatchFirst->ivtxOwn;
                            apvtx[ivtxFile] = prec;
                            if (pbatchFirst->ivtxOwn == 0)
                                pbatchFirst--;
                        }
                        prec->ivtx = iv;
                        prec->pbatch = pbatch;
                        prec++;
                    }
                    if (!fMulti)
                    {
                        pbatchFirst = &abatch[ib - 1];
                        memset(apvtx, 0, cvtxFile << 2);
                    }
                }
                qsort(arec, cvtxTotal, sizeof(VERTREC), CompareVertRec);
            }
        }
        if (spec.iSpec != -1)
        {
            phdr->apart[0].iqwSpec = phdr->apart[1].iqwSpec = iqw;
            vifs.AddVifUnpack(UPK_V4_32, 3, &spec, phdr->apart[0].iqwSpec);
            iqw += 3;
        }
        int ib;
        uchar *pbVtxCur = pbVtx;
        for (ib = 0; ib < cbatchThis; ib++)
        {
            BATCH *pbatch = &abatch[ib];
            BATCHPART0 *ppart = &phdr->apart[ib];
            ppart->cvtxFlag = pbatch->cvtx | 0x8000;
            ppart->oidAltSat = pbatch->pshd->OidAltSat;
            ppart->cframe = pbatch->pshd->cframe;
            for (int ishdp = 0; ishdp < 3; ishdp++)
            {
                for (int ibPrev = 0; ibPrev < ib; ibPrev++)
                {
                    BATCH *pbatchPrev = &abatch[ibPrev];
                    BATCHPART0 *ppartPrev = &phdr->apart[ibPrev];
                    if (pbatch->pshd == pbatchPrev->pshd && ((pbatch->grf >> ishdp) & 1) && ((pbatchPrev->grf >> ishdp) & 1))
                    {
                        ppart->aiqwShdp[ishdp] = ppartPrev->aiqwShdp[ishdp];
                        break;
                    }
                }
            }
            for (int ishdp = 0; ishdp < 3; ishdp++)
            {
                if (ppart->aiqwShdp[ishdp] == 0 && ((pbatch->grf >> ishdp) & 1))
                {
                    if (psubglob->bUnk02 & 2)
                    {
                        int ioid = pglobset->cnUnk2c;
                        while (--ioid >= 0)
                            iqw += func_001EC6D0(PshdFromOid(pglobset->anUnk30[ioid]), ishdp, iqw, &vifs);
                    }
                    ppart->aiqwShdp[ishdp] = iqw;
                    iqw += AddVifUnpackShdp(pbatch->pshd, ishdp, iqw, &vifs);
                }
            }
            if (pbatch->psaa != 0)
            {
                for (int ibPrev = 0; ibPrev < ib; ibPrev++)
                {
                    BATCH *pbatchPrev = &abatch[ibPrev];
                    BATCHPART0 *ppartPrev = &phdr->apart[ibPrev];
                    if (pbatch->psaa == pbatchPrev->psaa)
                    {
                        ppart->iqwSaa = ppartPrev->iqwSaa;
                        break;
                    }
                }
                if (ppart->iqwSaa == 0)
                {
                    ppart->iqwSaa = iqw;
                    iqw += func_001ED960(pbatch->psaa, iqw, &vifs);
                }
            }
            iqw += 1;
            ppart->iqwVtx = iqw;
            iqw += pbatch->cvtx * 4;
            ppart->iqwUv = iqw;
            ppart->iqwEnd = ppart->iqwVtx + (((pbatch->cvtx / 2) & ~3) + 2) * 4;
            if (arec != 0)
                ppart->iqwOwn = ppart->iqwVtx + pbatch->ivtxOwn * 4;
            if (psubglob->cbnd != 0)
            {
                ppart->iqwWeights = iqw;
                iqw += pbatch->cvtx - pbatch->ivtxOwn;
                if (psubglob->bUnk02 & 1)
                    phdr->grf.grf |= 0x20;
                else
                    phdr->grf.grf |= 0x10;
            }
            if (pbatch->pshd->shdk == 3 && phdr->iqwShadow == 0)
            {
                phdr->apart[0].aiqwShdp[3] = phdr->apart[1].aiqwShdp[3] = iqw;
                iqw += AddVifUnpackShdp(pbatch->pshd, 3, iqw, &vifs);
                ushort *pus;
                vifs.UnpackHelper(UPK_S16, 1, iqw, 0, (uint **)&pus);
                *pus = iqw + 2;
                phdr->iqwShadow = iqw;
                iqw += 0x1a;
            }
            if (spec.iSpec != -1)
            {
                if (psubglob->bUnk02 & 1)
                {
                    ppart->iqwUvLink = ppart->iqwOwn + 1;
                }
                else
                {
                    ppart->iqwUvLink = iqw;
                    int c = pbatch->cvtx - pbatch->ivtxOwn;
                    iqw += c;
                    vifs.UnpackHelper((UPK)0x1e, c, ppart->iqwUvLink, 0, (uint **)&pbatch->pbNorm);
                }
            }
            if (pglob->pwrbg != 0)
            {
                ppart->iqwWeights = iqw;
                int cqwWarp = pglob->pwrbg->cmat * pbatch->cvtx;
                phdr->grf.grf |= 0x40;
                iqw += cqwWarp;
            }
            vifs.AddVifStcycl(1, 4);
            if ((psubglob->bUnk02 & 1) || psubglob->cbnd != 0 || spec.iSpec != -1)
                vifs.UnpackHelper((UPK)0x19, pbatch->cvtx, ppart->iqwVtx, 0, (uint **)&pbatch->pvUvLink);
            else
                vifs.UnpackHelper((UPK)0x15, pbatch->cvtx, ppart->iqwVtx, 0, (uint **)&pbatch->pvUv);
            if (psubglob->bUnk02 & 1)
            {
                if (pbatch->ivtxOwn < pbatch->cvtx)
                {
                    vifs.UnpackHelper((UPK)0x1e, pbatch->cvtx - pbatch->ivtxOwn, ppart->iqwOwn + 1, 0, (uint **)&pbatch->pbNorm);
                    vifs.UnpackHelper(UPK_V4_8, pbatch->cvtx - pbatch->ivtxOwn, ppart->iqwOwn + 2, 0, (uint **)&pbatch->pbRgb);
                }
            }
            else
            {
                vifs.Align(0);
                vifsAux.AddDmaRefs((vifs.m_pb - (uchar *)pqwChain) >> 4, pqwChain);
                pqwChain = (QW *)vifs.m_pb;
                vifsAux.AddDmaCnt();
                vifsAux.UnpackHelper(UPK_V4_5, pbatch->cvtx, ppart->iqwVtx + 1, 0, (uint **)&pbatch->obColorA);
                if ((pbatch->grf & 2) && pbatch->pshd->shdk != 3)
                    vifsAux.UnpackHelper(UPK_V4_5, pbatch->cvtx, ppart->iqwVtx + 2, 0, (uint **)&pbatch->obColorB);
                if (fAlpha != 0 || pbatch->obColorB != 0)
                {
                    vifsAux.AddVifStmask(0x3f3f3f3f);
                    if (fAlpha != 0)
                        vifsAux.UnpackHelper(UPK_S8, pbatch->cvtx, ppart->iqwVtx + 1, 0, (uint **)&pbatch->pbAlpha);
                    if (pbatch->obColorB != 0)
                        vifsAux.UnpackHelper(UPK_S8, pbatch->cvtx, ppart->iqwVtx + 2, 0, (uint **)&pbatch->obAlpha);
                    vifsAux.AddVifStmask(0);
                }
                vifsAux.EndDmaCnt();
            }
            vifs.UnpackHelper(UPK_V4_32, pbatch->cvtx, ppart->iqwVtx + 3, 0, (uint **)&pbatch->pvPos);
            vifs.AddVifStcycl(1, 1);
            if (psubglob->cbnd != 0 && pbatch->ivtxOwn < pbatch->cvtx)
            {
                if (psubglob->cbnd < 4)
                {
                    vifs.AddVifStrow(&D_002DEB60);
                    vifs.AddVifStmask(D_002BE540[CwtFromSubglob(psubglob)]);
                }
                vifs.UnpackHelper((UPK)D_002BE558[CwtFromSubglob(psubglob)], pbatch->cvtx - pbatch->ivtxOwn, ppart->iqwWeights,
                                  0, (uint **)&pbatch->pbWeights);
                if (psubglob->cbnd < 4)
                    vifs.AddVifStmask(0);
            }
            if (pglob->pwrbg != 0)
            {
                int iqwW = ppart->iqwWeights;
                WRBGBATCH *pwb = (WRBGBATCH *)PvAllocSwClearP(sizeof(WRBGBATCH));
                AppendSlEntry(&pglob->pwrbg->slBatch, pwb);
                pwb->cvtx = pbatch->cvtx;
                pwb->pvPos = pbatch->pvPos;
                for (int imat = 0; imat < pglob->pwrbg->cmat; imat++)
                {
                    vifs.UnpackHelper(UPK_V4_32, pbatch->cvtx, iqwW, 0, (uint **)&pwb->apvWarp[imat]);
                    memset(pwb->apvWarp[imat], 0, pbatch->cvtx << 4);
                    iqwW += pbatch->cvtx;
                }
            }
            if (!(psubglob->bUnk02 & 1))
            {
                int shdk = pbatch->pshd->shdk;
                if (shdk != 1 && shdk != 7)
                {
                    RGBA5551 *pcol = (RGBA5551 *)pbatch->obColorA;
                    uchar *pbAlpha = pbatch->pbAlpha;
                    for (int iv = 0; iv < pbatch->cvtx; iv++)
                    {
                        RGBA rgbaA = argbaFile[pbVtxCur[iv]];
                        RGBA rgbaB = pbatch->pshd->rgba;
                        uchar a = (rgbaA.bAlpha * rgbaB.bAlpha + 0xff) / 0x1fe;
                        RGBA5551 *pc = pcol++;
                        RGBA5551 col;
                        col.g = (uchar)((rgbaA.bGreen * rgbaB.bGreen + 0xff) / 256) >> 3;
                        col.r = (uchar)((rgbaA.bRed * rgbaB.bRed + 0xff) / 256) >> 3;
                        col.b = (uchar)((rgbaA.bBlue * rgbaB.bBlue + 0xff) / 256) >> 3;
                        col.a = a >> 7;
                        *pc = col;
                        if (pbAlpha != 0)
                        {
                            *pbAlpha = a;
                            pbAlpha++;
                        }
                    }
                }
                else
                {
                    if (D_002BE580 != 0 || !(pbatch->grf & 0x20))
                    {
                        for (int iofs = 0; iofs < pbatch->cvtx; iofs += 0x27)
                        {
                            int n = Min(0x27, pbatch->cvtx - iofs);
                            uint *pun;
                            vifsLit.UnpackHelper(UPK_V4_32, 3, 0x307, 0, &pun);
                            pun[0] = 0x30a;
                            pun[1] = pun[0] + n * 3;
                            pun[2] = pun[1];
                            pun[3] = pun[2] + n;
                            pun[4] = pun[3] + n;
                            pun[5] = pbatch->obColorA + iofs * 2;
                            pun[6] = pbatch->obColorB + iofs * 2;
                            if (fAlpha != 0)
                                pun[8] = (uint)pbatch->pbAlpha + iofs;
                            else
                                pun[8] = 0;
                            pun[9] = pbatch->obAlpha + iofs;
                            *(float *)&pun[7] = (float)pbatch->pshd->OidAltSat * 0.000244140625f;
                            apvLit[nLit] = pun;
                            nLit++;
                            vifsLit.AddVifStcycl(1, 3);
                            VECTOR *pvNorm;
                            RGBA *prgbaCol;
                            VECTOR *pvPos;
                            vifsLit.UnpackHelper(UPK_V3_32, n, 0x30a, 0, (uint **)&pvNorm);
                            vifsLit.UnpackHelper(UPK_V4_8, n, 0x30b, 0, (uint **)&prgbaCol);
                            vifsLit.UnpackHelper(UPK_V3_32, n, 0x30c, 0, (uint **)&pvPos);
                            vifsLit.AddVifStcycl(1, 1);
                            {
                                for (int k = 0; k < n; k++)
                                {
                                    int ivtxFile = pbVtxCur[iofs + k];
                                    *pvNorm++ = anormFile[ivtxFile];
                                    *pvPos++ = aposFile[ivtxFile];
                                    RGBA rgba = argbaFile[ivtxFile];
                                    RGBA rgbaB = pbatch->pshd->rgba;
                                    rgba.bRed = (rgba.bRed * rgbaB.bRed + 0xff) / 256;
                                    rgba.bGreen = (rgba.bGreen * rgbaB.bGreen + 0xff) / 256;
                                    rgba.bBlue = (rgba.bBlue * rgbaB.bBlue + 0xff) / 256;
                                    rgba.bAlpha = (rgba.bAlpha * rgbaB.bAlpha + 0xff) / 0x1fe;
                                    *prgbaCol++ = rgba;
                                }
                            }
                            vifsLit.AddVifMscal(D_00000000 + 0x10);
                            AddDmaInt(&vifsLit, 0x90000000);
                        }
                    }
                }
            }
            for (int iv = 0; iv < pbatch->cvtx; iv++)
            {
                uint ivtxFile = pbVtxCur[iv];
                if (psubglob->cbnd == 0)
                {
                    *(VECTOR *)((char *)pbatch->pvPos + iv * 0x10) = aposFile[ivtxFile];
                }
                else if (iv < pbatch->ivtxOwn)
                {
                    float *pg = (float *)(iv * 0x10 + (uint)pbatch->pvPos);
                    pg[0] = pg[1] = pg[2] = -1.0f;
                }
                if (*punBits & un)
                    *(float *)((char *)pbatch->pvPos + iv * 0x10 + 0xc) = *pgFit + 4096.0f;
                else
                    *(float *)((char *)pbatch->pvPos + iv * 0x10 + 0xc) = *pgFit;
                un <<= 1;
                if (un == 0)
                {
                    punBits++;
                    un = 1;
                }
                if (pbatch->pvUvLink != 0)
                {
                    ((UVLINK *)((uchar *)pbatch->pvUvLink + iv * 6))->u = (int)(auvFile[ivtxFile].u * 4096.0f + 0.5f);
                    ((UVLINK *)((uchar *)pbatch->pvUvLink + iv * 6))->v = (int)(auvFile[ivtxFile].v * 4096.0f + 0.5f);
                }
                else
                {
                    ((short *)((char *)pbatch->pvUv + iv * 4))[0] = (short)(int)(auvFile[ivtxFile].u * 4096.0f + 0.5f);
                    ((short *)((char *)pbatch->pvUv + iv * 4))[1] = (short)(int)(auvFile[ivtxFile].v * 4096.0f + 0.5f);
                }
                if (pbatch->grf & 0x20)
                {
                    pbatch->aivtx[iv] = ivtxFile;
                    CLUTPAL *ppal = &pclutset->apal[pclutset->aipal[ibatch]];
                    CLUTCOLOR *pcc = &ppal->acolor[ivtxFile];
                    ((ushort *)pbatch->obColorA)[iv] = pcc->ucA;
                    ((ushort *)pbatch->obColorB)[iv] = pcc->ucB;
                    ((uchar *)pbatch->obAlpha)[iv] = pcc->bAlpha;
                    if (pbatch->pbAlpha != 0)
                    {
                        RGBA rgbaA = argbaFile[ivtxFile];
                        RGBA rgbaB = pbatch->pshd->rgba;
                        pbatch->pbAlpha[iv] = (rgbaA.bAlpha * rgbaB.bAlpha + 0xff) / 0x1fe;
                    }
                }
            }
            pbVtxCur += pbatch->cvtx;
            if (!fMulti)
            {
                if (ib == 0)
                {
                    vifs.AddVifMscal(D_00000000);
                    if (psubglob->bUnk00 == 0xb || psubglob->bUnk00 == 0xd)
                    {
                        vifs.AddVifMscal(D_00003628);
                    }
                    else
                    {
                        char *pvMscal;
                        switch (ibatch)
                        {
                        case 0:
                            pvMscal = fNoSaa ? D_00002E20 : D_00002EF8;
                            break;
                        case 1:
                            pvMscal = fNoSaa ? D_00002E68 : D_00002F80;
                            break;
                        default:
                            pvMscal = fNoSaa ? D_00002EB0 : D_00002FC8;
                            break;
                        }
                        vifs.AddVifMscal(pvMscal);
                    }
                }
                else if (psubglob->bUnk00 == 0xb || psubglob->bUnk00 == 0xd)
                {
                    vifs.AddVifMscal(D_000037A8);
                }
                else
                {
                    vifs.AddVifMscal(fNoSaa ? D_00002EE8 : D_00003358);
                }
            }
        }
        if (fMulti)
        {
            vifs.AddVifMscal(D_00000000);
            if (psubglob->bUnk00 == 0xb || psubglob->bUnk00 == 0xd)
            {
                vifs.AddVifMscal(D_00003628);
                vifs.AddVifMscal(D_000037A8);
            }
            else
            {
                switch (ibatch)
                {
                case 0:
                    vifs.AddVifMscal(D_00002EF8);
                    break;
                case 1:
                    vifs.AddVifMscal(D_00002F80);
                    break;
                default:
                    vifs.AddVifMscal(D_00002FC8);
                    break;
                }
                vifs.AddVifMscal(D_00003358);
            }
        }
        if (arec != 0)
        {
            VERTREC *precEnd = arec + cvtxTotal;
            BATCH *pbatchLast = 0;
            for (VERTREC *prec = arec; prec < precEnd; prec++)
            {
                BATCH *pbatch = prec->pbatch;
                BATCHPART0 *ppartRec = &phdr->apart[pbatch - abatch];
                if (pbatchLast != prec->pbatchFirst)
                {
                    pbatchLast = prec->pbatchFirst;
                    BATCHPART0 *ppartFirst = &phdr->apart[pbatchLast - abatch];
                    int iqwSkin = ppartRec->iqwVtx + (prec->ivtx << 2);
                    ppartFirst->iqwSkin = iqwSkin;
                }
                uint usLink;
                if (prec == precEnd - 1 || prec[1].pbatchFirst != pbatchLast)
                {
                    usLink = 0;
                }
                else
                {
                    BATCHPART0 *ppartNext = &phdr->apart[prec[1].pbatch - abatch];
                    usLink = ppartNext->iqwVtx + prec[1].ivtx * 4;
                    if (prec->ivtxFirst == prec[1].ivtxFirst)
                        usLink |= 0x8000;
                }
                ((UVLINK *)((uchar *)pbatch->pvUvLink + prec->ivtx * 6))->link = usLink;
            }
            for (VERTREC *prec = arec; prec < precEnd; )
            {
                int ivtxFile = prec->ivtxFile;
                BATCH *pbatchFirst = prec->pbatchFirst;
                int ivtxFirst = prec->ivtxFirst;
                RGBA rgbaA = argbaFile[ivtxFile];
                int bRed = rgbaA.bRed;
                int bGreen = rgbaA.bGreen;
                int bBlue = rgbaA.bBlue;
                int bMax;
                if (bRed > bGreen && bRed > bBlue)
                {
                    bMax = bRed;
                }
                else if (bGreen > bBlue)
                    bMax = bGreen;
                else
                    bMax = bBlue;
                float gBright = (float)bMax * 0.003921569f;
                if (pbatchFirst->pbNorm != 0)
                {
                    VECTOR *pnorm = &anormFile[ivtxFile];
                    char *pbN = (char *)pbatchFirst->pbNorm + (ivtxFirst - pbatchFirst->ivtxOwn) * 4;
                    pbN[0] = (int)(pnorm->x * 127.0f);
                    pbN[1] = (int)(pnorm->y * 127.0f);
                    pbN[2] = (int)(pnorm->z * 127.0f);
                    pbN[3] = (int)(gBright * 127.0f);
                }
                if (pbatchFirst->pbRgb != 0)
                {
                    RGBA rgbaB = pbatchFirst->pshd->rgba;
                    uchar *pbC = pbatchFirst->pbRgb + (ivtxFirst - pbatchFirst->ivtxOwn) * 4;
                    pbC[0] = (rgbaA.bRed * rgbaB.bRed + 0xff) / 512;
                    pbC[1] = (rgbaA.bGreen * rgbaB.bGreen + 0xff) / 512;
                    pbC[2] = (rgbaA.bBlue * rgbaB.bBlue + 0xff) / 512;
                    pbC[3] = (rgbaA.bAlpha * rgbaB.bAlpha + 0xff) / 0x1fe;
                }
                if (psubglob->cbnd != 0)
                {
                    VECTOR *pposDst = (VECTOR *)(ivtxFirst * 0x10 + (uint)pbatchFirst->pvPos);
                    *pposDst = aposFile[ivtxFile];
                    CopyAb(pbatchFirst->pbWeights + (ivtxFirst - pbatchFirst->ivtxOwn) * CwtFromSubglob(psubglob), (char *)pvWeights + ivtxFile * CwtFromSubglob(psubglob), CwtFromSubglob(psubglob));
                }
                prec++;
                while (prec < precEnd && prec->pbatchFirst == pbatchFirst && prec->ivtxFirst == ivtxFirst)
                    prec++;
            }
        }
    }
    int cqwMain;
    char *pbOld;
    char *pbNew;
    if (psubglob->bUnk02 & 1)
    {
        vifs.EndDmaCnt();
        vifs.AddDmaRet();
        vifs.DetachCopySw(&cqwMain, (QW **)&pbOld, (QW **)&pbNew);
        psubglob->cqw = cqwMain;
        pglob->pvUnk10 = pbNew;
    }
    else
    {
        vifs.Align(0);
        vifsAux.AddDmaRefs((vifs.m_pb - (uchar *)pqwChain) >> 4, pqwChain);
        vifsAux.AddDmaRet();
        vifs.DetachCopySw(&cqwMain, (QW **)&pbOld, (QW **)&pbNew);
        if (pglob->psaa != 0)
        {
            ((SAAFIELDS *)pglob->psaa)->cqwMain = cqwMain;
            ((SAAFIELDS *)pglob->psaa)->pvMain = pbNew;
        }
        CopyDmaRefsChainSw(psubglob, pglob, (vifsAux.m_pb - vifsAux.m_ab) / sizeof(QW), vifsAux.m_ab, cqwMain, pbNew, pbOld);
        if (nLit > 0)
        {
            vifsLit.EndDmaCnt();
            vifsLit.AddDmaRet();
            int cqwLit = (vifsLit.m_pb - vifsLit.m_ab) / sizeof(QW);
            psgl->nUnk04 = cqwLit;
            psubglob->pvLit = PvAllocSwCopy(cqwLit << 4, vifsLit.m_ab);
        }
    }
    if (pclutset != 0)
        pglobset->nUnk4c = D_003E11EC;
    pglob->iUnkEnd = D_003DA790;
    func_001EC740(pglob->iUnkFirst, pglob->iUnkEnd, cqwMain << 4, pbOld, pbNew);
    if (pglob->pwrbg != 0)
    {
        for (WRBGBATCH *pwb = (WRBGBATCH *)pglob->pwrbg->slBatch.head; pwb != 0; pwb = pwb->pwrbgbatchNext)
        {
            RelocPpv(pbOld, cqwMain << 4, pbNew, (char **)&pwb->pvPos);
            for (int imat = 0; imat < pglob->pwrbg->cmat; imat++)
                RelocPpv(pbOld, cqwMain << 4, pbNew, (char **)&pwb->apvWarp[imat]);
        }
    }
    if (psubglob->bUnk02 & 8)
    {
        int cbatchAll = 0;
        for (int i = 0; i < cbatch; i++)
            cbatchAll += acbatch[i];
        BATCHRECS *pbatchrecs = (BATCHRECS *)PvAllocSwClearP(sizeof(BATCHRECS));
        pbatchrecs->pbatchrecsNext = (BATCHRECS *)pglobset->pbatchrecsFirst;
        pglobset->pbatchrecsFirst = pbatchrecs;
        pbatchrecs->psubglob = psubglob;
        pbatchrecs->cbatch = cbatchAll;
        pbatchrecs->abatchrec = (BATCHREC *)PvAllocSw(cbatchAll << 5);
        BATCHREC *pbr = pbatchrecs->abatchrec;
        for (int i = 0; i < cbatch; i++)
        {
            BATCH *pb = apbatch[i];
            for (int j = 0; j < acbatch[i]; j++)
            {
                pbr->cvtx = pb->cvtx;
                pbr->grf = pb->grf;
                pbr->pvPos = PbReloc(pbOld, cqwMain << 4, pbNew, (char *)pb->pvPos);
                pbr->pvUv = PbReloc(pbOld, cqwMain << 4, pbNew, (char *)pb->pvUv);
                pbr->pvUvLink = PbReloc(pbOld, cqwMain << 4, pbNew, (char *)pb->pvUvLink);
                pbr->pbRgb = PbReloc(pbOld, cqwMain << 4, pbNew, (char *)pb->pbRgb);
                pbr->pbNorm = PbReloc(pbOld, cqwMain << 4, pbNew, (char *)pb->pbNorm);
                pbr->unk_0x1c = 0;
                pb++;
                pbr++;
            }
        }
    }
    LoadSubglobInstancesFromBrx(pglobset, psubglob, pglob, psubglobx, cSubExtra, cbatch, acbatch, apbatch, pbis);
}
