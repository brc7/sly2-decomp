// func_00167320: ported from Lilly's personal Sly 2 decomp (src/P2/glob.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x00167320 LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream  (MATCHES Sly 2, T1064)
// SO::LoadFromBrx 0x1F31E0 calls it for the model of every SO. File order (re/notes/formats/07-globs.md):
//   s8 fCompact (0 = every sub-glob also has a bounds record), vec3 posCenter, f32 sRadius,
//   u8 cpsaa + that many PsaaLoadFromBrx (shader animations), u8 + that many s32, s16 cglob (the number of SLOTS: a sub-glob with cSubExtra instances uses 1 + cSubExtra of them),
//   then per sub-glob: s8 cfilter (non-zero: cfilter x s16 ids + s32 byte count; when the level being loaded is not one of the ids the record is skipped and cglob drops by one),
//   s8 type, u16 cSubExtra, s16 XF number (SO::PxfFromIxf), s8 bUnk00, f32 gUnk04 (v * |v|), f32 -> SUBGLOBLOAD, s32 oid, s8 -> SUBGLOBLOAD, s8 fHide,
//   [fCompact == 0: vec3 posCenter, f32 sRadius, u32 grf, f32 gUnk10 (FLT_MAX = a default by type)], then the body by type (0 geometry, 1 / 2 cel border edges, 3 shadow volume),
//   and last LoadPoseSetsFromBrx. The scratch record sgl (12 bytes, zeroed per sub-glob) is handed to the body loader.
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
enum MSGID
{
};
struct CBinaryInputStream;
struct LO;
struct SO;
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
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
    void Skip(int cb);
    uchar U8Read();
    ushort U16Read();
    uint U32Read();
    char S8Read();
    short S16Read();
    int S32Read();
    float F32Read();
    void ReadVector(VECTOR *pvec);
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
void LoadPoseSetsFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis);
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
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
struct XP;
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
struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
};
struct SOTREE
{
      qword qwMin;
      qword qwMax;
      qword qwCenter;
      float sRadius;
      DL dlpart;
      DLE dleAwake;
      DLE dleAll;
      DLE dleQueue1;
      DLE dleQueue2;
      SO *psoRoot;
      XPLIST *pxplistFirst;
      XP *pxpFirst;
      XP *pxpZap;
      ulong fQueued1 : 1;
               ulong fQueued2 : 1;
               ulong fNoUpdate : 1;
               ulong fTouched : 1;
               ulong fDone : 1;
               ulong unk_0x70 : 59;
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
};
int FIsLoadFilterMatch(int cfilter, short *afilter);
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
extern "C" SAA *PsaaLoadFromBrx(void *pvOwner, CBinaryInputStream *pbis);
void LoadSubglobCelFromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
void LoadSubglobCelNoShdFromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
void LoadSubglobType3FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
extern "C" {
void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
}
static inline void *PvAllocSwClear(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    swalloc.cbAlign = 0x10;
    return PvAllocSwClearImpl(&swalloc);
}
void LoadGlobsetFromBrx(GLOBSET *pglobset, SO *pso, CBinaryInputStream *pbis) __asm__("func_00167320");
/** @sly2-annot addr=0x167320
 * Purpose: Reads an SO model (GLOBSET): bounds, shader animations (SAA), sub-globs and their per-type
 *   geometry
 * Info: proposed name LoadGlobsetFromBrx (T1); TU glob
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void LoadGlobsetFromBrx(GLOBSET *pglobset, SO *pso, CBinaryInputStream *pbis)
{
    pglobset->pso = pso;
    int fCompact = pbis->S8Read();
    pbis->ReadVector(&pglobset->posCenter);
    pglobset->sRadius = pbis->F32Read();
    pglobset->cpsaa = pbis->U8Read();
    pglobset->apsaa = (SAA **)PvAllocSwClear(pglobset->cpsaa * sizeof(SAA *));
    for (int i = 0; i < pglobset->cpsaa; i++)
    {
        pglobset->apsaa[i] = PsaaLoadFromBrx((void *)1, pbis);
    }
    pglobset->cnUnk2c = pbis->U8Read();
    pglobset->anUnk30 = (int *)PvAllocSwClear(pglobset->cnUnk2c * sizeof(int));
    pglobset->nUnk34 = 0;
    for (int i = 0; i < pglobset->cnUnk2c; i++)
    {
        pglobset->anUnk30[i] = pbis->S32Read();
    }
    pglobset->cglob = pbis->S16Read();
    pglobset->asubglob = (SUBGLOB *)PvAllocSwClear(pglobset->cglob * sizeof(SUBGLOB));
    pglobset->aglob = (GLOB *)PvAllocSwClear(pglobset->cglob * sizeof(GLOB));
    if (fCompact == 0)
    {
        pglobset->asubglobx = (SUBGLOBX *)PvAllocSwClear(pglobset->cglob * sizeof(SUBGLOBX));
    }
    for (int i = 0; i < pglobset->cglob; i++)
    {
        SUBGLOB *psubglob = &pglobset->asubglob[i];
        GLOB *pglob = &pglobset->aglob[i];
        SUBGLOBX *psubglobx = pglobset->asubglobx ? &pglobset->asubglobx[i] : 0;
        SUBGLOBLOAD sgl;
        memset(&sgl, 0, sizeof(sgl));
        int cfilter = pbis->S8Read();
        if (cfilter != 0)
        {
            short afilter[128];
            pbis->Read(cfilter * 2, afilter);
            int cbSkip = pbis->S32Read();
            if (!FIsLoadFilterMatch(cfilter, afilter))
            {
                pbis->Skip(cbSkip);
                i--;
                pglobset->cglob--;
                continue;
            }
        }
        psubglob->type = pbis->S8Read();
        ushort cSubExtra = pbis->U16Read();
        pglob->pxf = pso->PxfFromIxf(pbis->S16Read());
        psubglob->bUnk00 = pbis->S8Read();
        float g = pbis->F32Read();
        if (g == 3.4028235e+38f)
            psubglob->gUnk04 = 3.4028235e+38f;
        else
            psubglob->gUnk04 = g * fabsf(g);
        sgl.gUnk00 = pbis->F32Read();
        pglob->oid = (OID)pbis->S32Read();
        sgl.nUnk08 = pbis->S8Read();
        pglob->fHide = pbis->S8Read() & 1;
        if (fCompact == 0)
        {
            pbis->ReadVector(&psubglobx->posCenter);
            psubglobx->sRadius = pbis->F32Read();
            psubglobx->grfUnk14 = pbis->U32Read();
            psubglobx->gUnk10 = pbis->F32Read();
            if (psubglobx->gUnk10 == 3.4028235e+38f)
            {
                switch (psubglob->type)
                {
                case 0:
                case 2:
                case 3:
                    psubglobx->gUnk10 = 1e10f;
                    break;
                case 1:
                    psubglobx->gUnk10 = 2000.0f;
                    break;
                }
            }
        }
        pglob->grfglobType = psubglob->type;
        pglob->iUnk0c3 = psubglob->bUnk00;
        if (psubglob->gUnk04 != 3.4028235e+38f)
            pglob->fLimit = 1;
        switch (psubglob->type)
        {
        case 0:
            LoadSubglobType0FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 1:
            LoadSubglobCelNoShdFromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 2:
            LoadSubglobCelFromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 3:
            LoadSubglobType3FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        }
        i += cSubExtra;
    }
    LoadPoseSetsFromBrx(pglobset, pbis);
}
