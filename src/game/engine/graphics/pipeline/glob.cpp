// glob.cpp -- globsets (the level's renderable geometry of an SO: GLOBSET, GLOB, SUBGLOB): loading them from the
// level file (LoadGlobsetFromBrx and the per-type subglob loaders), the render passes' begin/end packets
// (BeginRp*/EndRp*), building and sorting the draw records, and culling/rendering a globset.
// TU 0x167310..0x16F148 (39 functions, address order) plus three linkonce VECTOR component readers (0x2599D8..).
//
// Types: sly2 headers (basic.h, lo.h, xfm.h, so.h, cm.h) and vifs.h/gifs.h; the file-scope types after the
// includes are LoadGlobsetFromBrx's Lilly declarations (GLOBSET, GLOB, SUBGLOB..., and its CBinaryInputStream,
// which replaces bis.h here: bis.h's vec.h clashes with sly2/basic.h). The m2c bodies are rewritten on those types
// (VIFS members for the pass packets); the other functions keep the per-function views they were matched with,
// each in a namespace named after it (as vec.cpp and mark.cpp do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_glob
// SLY2_ALIAS: _GLOBAL_$I$*=func_0016F128

#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/cm.h"
#include "vifs.h"
#include "gifs.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/vu0.h"
#include "sly2/util.h"

// 0x16EFE0 StaticInit_glob and its _GLOBAL_$I$ stub func_0016F128: g++'s initializer for the cel-border colour
// (an RGBA word with a constructor, 0x80101010) and the six engine slot pools.
struct RGBAW { uint rgba; RGBAW(uint n) { rgba = n; } };
RGBAW D_002DEB50(0x80101010);
SLY2_INSTANTIATE_SLOT_POOLS()

typedef unsigned size_t; inline float GetVectorComponentX(VECTOR *pvec); inline float GetVectorComponentY(VECTOR *pvec); inline float GetVectorComponentZ(VECTOR *pvec);   /* the VECTOR component readers: inline, defined after their callers (end of file) */

extern "C" {
extern char D_0025B880[], D_0025C1B0[];   /* the cel-border VU1 microprogram */
extern QW D_002DF700[2], D_002DF720[2];   /* the cutout GS register packets */
void EnsureVu1Code(VIFS *pvifs, void *pvStart, void *pvEnd) __asm__("EnsureVu1Code__FP4VIFSPvT1");
void BeginRpGlobV(VIFS *pvifs) __asm__("BeginRpGlob");   /* named apart: other functions declare their own views */
void EndRpGlobV(VIFS *pvifs) __asm__("EndRpGlob");
void AddVifDirectFlushed(VIFS *pvifs, int cqw, QW *aqw);
}

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

struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
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

struct BND
{
      OID oid;
      LO *plo;
      int ixf;
      XF *pxf;
      MATRIX4 matInv;
};

struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float maxRenderDistance;   /* FLT_MAX in the file: 1e10, or 2000 for cel-border edges (type 1) */
      uint flags;
};

struct SUBGLOBLOAD
{
    float fogIntensity;     /* BuildCmFgfn's argument (the fog curve uploaded with the geometry) */
    int numLitQwords;       /* size of the lit-vertex DMA chain (set by the type-0 loader) */
    int batchMode;          /* 0 / 1 / 2: batch header flag 0x100 / 0x200 / 0x400 */
};

struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
};

struct SWALLOC
{
    int cb;
    int cbAlign;
};

// GLOBSET constructor, at the start of the glob TU.
/* GLOBSET's constructor (sly2/so.h; it was kept as a C function under the symbol __4CRef before). */
GLOBSET::GLOBSET()
{
    unknown4C = -1;
}

// LoadGlobsetFromBrx: ported from Lilly's personal Sly 2 decomp (src/P2/glob.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x00167320 LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream  (MATCHES Sly 2, T1064)
// SO::LoadFromBrx 0x1F31E0 calls it for the model of every SO. File order (re/notes/formats/07-globs.md):
//   s8 fCompact (0 = every sub-glob also has a bounds record), vec3 posCenter, f32 sRadius,
//   u8 cpsaa + that many PsaaLoadFromBrx (shader animations), u8 + that many s32, s16 cglob (the number of SLOTS: a sub-glob with cSubExtra instances uses 1 + cSubExtra of them),
//   then per sub-glob: s8 cfilter (non-zero: cfilter x s16 ids + s32 byte count; when the level being loaded is not one of the ids the record is skipped and cglob drops by one),
//   s8 type, u16 cSubExtra, s16 XF number (SO::GetXfFromIndex), s8 unknown00, f32 unknown04 (v * |v|), f32 -> SUBGLOBLOAD, s32 oid, s8 -> SUBGLOBLOAD, s8 isHidden,
//   [fCompact == 0: vec3 posCenter, f32 sRadius, u32 grf, f32 unknown10 (FLT_MAX = a default by type)], then the body by type (0 geometry, 1 / 2 cel border edges, 3 shadow volume),
//   and last LoadPoseSetsFromBrx. The scratch record sgl (12 bytes, zeroed per sub-glob) is handed to the body loader.
extern "C" {
void* memset(void* ptr, int value, size_t num);
}








void LoadPoseSetsFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis) __asm__("LoadPoseSetsFromBrx");

int FIsLoadFilterMatch(int cfilter, short *afilter) __asm__("IsCurrentLevelInList__FiPs");

void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromWorldHeap");
extern "C" SAA *PsaaLoadFromBrx(void *pvOwner, CBinaryInputStream *pbis) __asm__("LoadSaaFromBrx");
extern "C" void LoadSubglobType2FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
extern "C" void LoadSubglobType1FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
void LoadSubglobType3FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType3FromBrx");
extern "C" {
void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType0FromBrx");
}
static inline void *PvAllocSwClear(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    swalloc.cbAlign = 0x10;
    return PvAllocSwClearImpl(&swalloc);
}
void LoadGlobsetFromBrx(GLOBSET *pglobset, SO *pso, CBinaryInputStream *pbis) __asm__("LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream");
// Reads an SO model (GLOBSET): bounds, shader animations (SAA), sub-globs and their per-type geometry.
void LoadGlobsetFromBrx(GLOBSET *pglobset, SO *pso, CBinaryInputStream *pbis)
{
    pglobset->owner = pso;
    int fCompact = pbis->S8Read();
    pbis->ReadVector(&pglobset->boundsCenter);
    pglobset->boundsRadius = pbis->F32Read();
    pglobset->cpsaa = pbis->U8Read();
    pglobset->apsaa = (SAA **)PvAllocSwClear(pglobset->cpsaa * sizeof(SAA *));
    for (int i = 0; i < pglobset->cpsaa; i++)
    {
        pglobset->apsaa[i] = PsaaLoadFromBrx((void *)1, pbis);
    }
    pglobset->unknown2C = pbis->U8Read();
    pglobset->unknown30 = (int *)PvAllocSwClear(pglobset->unknown2C * sizeof(int));
    pglobset->unknown34 = 0;
    for (int i = 0; i < pglobset->unknown2C; i++)
    {
        pglobset->unknown30[i] = pbis->S32Read();
    }
    pglobset->numPieces = pbis->S16Read();
    pglobset->subMeshes = (SUBGLOB *)PvAllocSwClear(pglobset->numPieces * sizeof(SUBGLOB));
    pglobset->pieces = (GLOB *)PvAllocSwClear(pglobset->numPieces * sizeof(GLOB));
    if (fCompact == 0)
    {
        pglobset->asubglobx = (SUBGLOBX *)PvAllocSwClear(pglobset->numPieces * sizeof(SUBGLOBX));
    }
    for (int i = 0; i < pglobset->numPieces; i++)
    {
        SUBGLOB *psubglob = &pglobset->subMeshes[i];
        GLOB *pglob = &pglobset->pieces[i];
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
                pglobset->numPieces--;
                continue;
            }
        }
        psubglob->type = pbis->S8Read();
        ushort cSubExtra = pbis->U16Read();
        pglob->transform = pso->GetXfFromIndex(pbis->S16Read());
        psubglob->unknown00 = pbis->S8Read();
        float g = pbis->F32Read();
        if (g == 3.4028235e+38f)
            psubglob->unknown04 = 3.4028235e+38f;
        else
            psubglob->unknown04 = g * fabsf(g);
        sgl.fogIntensity = pbis->F32Read();
        pglob->oid = (OID)pbis->S32Read();
        sgl.batchMode = pbis->S8Read();
        pglob->isHidden = pbis->S8Read() & 1;
        if (fCompact == 0)
        {
            pbis->ReadVector(&psubglobx->posCenter);
            psubglobx->sRadius = pbis->F32Read();
            psubglobx->flags = pbis->U32Read();
            psubglobx->maxRenderDistance = pbis->F32Read();
            if (psubglobx->maxRenderDistance == 3.4028235e+38f)
            {
                switch (psubglob->type)
                {
                case 0:
                case 2:
                case 3:
                    psubglobx->maxRenderDistance = 1e10f;
                    break;
                case 1:
                    psubglobx->maxRenderDistance = 2000.0f;
                    break;
                }
            }
        }
        pglob->grfglobType = psubglob->type;
        pglob->unknownBits0CA = psubglob->unknown00;
        if (psubglob->unknown04 != 3.4028235e+38f)
            pglob->fLimit = 1;
        switch (psubglob->type)
        {
        case 0:
            LoadSubglobType0FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 1:
            LoadSubglobType1FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 2:
            LoadSubglobType2FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        case 3:
            LoadSubglobType3FromBrx(pglobset, psubglob, pglob, &sgl, psubglobx, cSubExtra, pbis);
            break;
        }
        i += cSubExtra;
    }
    LoadPoseSetsFromBrx(pglobset, pbis);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadPoseSetsFromBrx { extern "C" {
typedef unsigned int uint;
typedef unsigned long ulong;
struct META {
    float limit;
    long kind : 4;
    ulong count : 8;
    ulong channel : 6;
    ulong feature : 1;
    ulong index : 13;
};
struct RECORD {
    float position[3];
    float radius;
    struct META meta;
    void *link;
    int pad1C;
};
struct GROUP{struct RECORD records[4];};
struct D {
    signed char channel;
    char pad1[7];
    void *chain;
    char padC[8];
};
struct E {
    char pad[0xC];
    uint flags;
    void *matrix;
    int word;
    char pad18[0xC];
    struct RECORD *sphere;
};
struct P {
    char pad[0x3C];
    struct D *descriptors;
    struct E *entries;
    void *localSpheres;
    struct GROUP *groups;
};
struct STREAM;
struct REQUEST {
    int bytes, alignment;
};
typedef char MetaSize[sizeof(struct META)==8?1:-1];
typedef char RecordSize[sizeof(struct RECORD)==0x20?1:-1];
typedef char EntrySize[sizeof(struct E)==0x28?1:-1];
short S16Read__18CBinaryInputStream(struct STREAM*) __asm__("S16Read__18CBinaryInputStream");
char S8Read__18CBinaryInputStream(struct STREAM*) __asm__("S8Read__18CBinaryInputStream");
struct VECTOR;
void ReadVector__18CBinaryInputStreamP6VECTOR(struct STREAM*,struct VECTOR*) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
float F32Read__18CBinaryInputStream(struct STREAM*) __asm__("F32Read__18CBinaryInputStream");
unsigned char U8Read__18CBinaryInputStream(struct STREAM*) __asm__("U8Read__18CBinaryInputStream");
void*AllocClearedFromWorldHeap(struct REQUEST*);
// Reads groups of up to 4 distance/LOD records (kind, position, radius, limit<=1e10, count) per globset descriptor.
// (inferred)
void LoadPoseSetsFromBrx(struct P *p, struct STREAM *stream)
{
    int count = S16Read__18CBinaryInputStream(stream);
    if (count) {
        struct REQUEST request;
        struct GROUP *groups;
        int i;
        request.bytes = count * sizeof(struct GROUP);
        request.alignment = 0x10;
        groups = (struct GROUP *)AllocClearedFromWorldHeap(&request);
        for (i = 0; i < count; i++) {
            struct GROUP *group = &groups[i];
            uint j;
            for (j = 0; j < 4; j++) {
                struct RECORD *record = &group->records[j];
                record->meta.kind = S8Read__18CBinaryInputStream(stream);
                if ((int)record->meta.kind == -1) {
                    for (; j < 4; j++)
                        group->records[j].meta.kind = -1;
                    break;
                }
                ReadVector__18CBinaryInputStreamP6VECTOR(stream, (struct VECTOR *)record->position);
                record->radius = F32Read__18CBinaryInputStream(stream);
                record->meta.limit = F32Read__18CBinaryInputStream(stream);
                record->meta.count = U8Read__18CBinaryInputStream(stream);
                record->meta.limit = FpuMin(1.0e10f, record->meta.limit);
                switch (record->meta.kind) {
                case 0: {
                    int index = S16Read__18CBinaryInputStream(stream);
                    struct D *descriptor = &p->descriptors[index];
                    struct E *entry = &p->entries[index];
                    if (entry->flags & 0x1000) {
                        record->meta.kind = 3;
                        record->link = entry->matrix;
                    }
                    record->meta.channel = descriptor->channel;
                    record->meta.feature = (entry->flags >> 11) & 1;
                    record->meta.index = index;
                    entry->sphere = record;
                    break;
                }
                case 1:
                    record->link = &groups[S16Read__18CBinaryInputStream(stream)];
                    break;
                }
            }
        }
        p->groups = groups;
    }
}
} } // namespace M_LoadPoseSetsFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BindGlobset {
struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16)));
struct BONE { char pad[0x40]; MATRIX4 mat; };
class LO {
public:
    int a, b, c;
#define V(n) virtual void V##n();
    V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9) V(10)
    V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20)
    V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30)
    V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40)
    V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50)
#undef V
    virtual BONE *GetXfFromIndex(int n);
};
struct ITEM { int oid; LO *plo; int n8; BONE *pbone; MATRIX4 mat; };
struct GRP { int a; BONE *pbone; int pad[4]; ITEM *aitem; int pad2[3]; };
struct HDR { signed char c0; signed char citem; char pad[0x12]; };
struct X { char pad[8]; int oid; char pad2[0x9C - 0xC]; X *pxNext; };
struct S { void *pvOwner; char pad[0x38 - 4]; int cgrp; HDR *ahdr; GRP *agrp; char pad2[0x4C - 0x44]; int unknown4C; char pad3[0x58 - 0x50]; X *pxFirst; };   /* GLOBSET (+0x4C: so.h's unknown4C) */
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { 
 }
extern "C" LO *FindSwObject__FP2SWi3OIDP2LO(G *pg, int kind, int oid, void *owner);
extern "C" void InvertMatrix4Rigid(MATRIX4 *pmatDst, MATRIX4 *pmatSrc);
extern "C" void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, MATRIX4 *pmatA, MATRIX4 *pmatB);
extern "C" void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern "C" void AddWrWarpGroup(LO *plo, X *px);
// Resolves globset bone references after load: finds each referenced LO by oid, its bone, and computes the relative
// bind matrix. (inferred)
extern "C" void BindGlobset(S *p)
{
    if (p->unknown4C != -1)
        p->unknown4C = g_sw.nUnk3d5c;
    for (int i = 0; i < p->cgrp; i++) {
        HDR *phdr = &p->ahdr[i];
        GRP *pgrp = &p->agrp[i];
        for (int j = 0; j < phdr->citem; j++) {
            ITEM *pitem = &pgrp->aitem[j];
            pitem->plo = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, pitem->oid, p->pvOwner);
            if (pitem->plo)
                pitem->pbone = pitem->plo->GetXfFromIndex(pitem->n8);
            if (pitem->pbone) {
                MATRIX4 mat;
                if (pgrp->pbone) {
                    MATRIX4 matT;
                    InvertMatrix4Rigid(&matT, &pitem->pbone->mat);
                    __ml__FRC7MATRIX4T0(&mat, &matT, &pgrp->pbone->mat);
                } else {
                    InvertMatrix4Rigid(&mat, &pitem->pbone->mat);
                }
                pitem->mat = mat;
            } else if (pgrp->pbone) {
                pitem->mat = pgrp->pbone->mat;
            } else {
                MATRIX4 *pmat = &pitem->mat;
                func_002333B4(pmat, 0, sizeof(MATRIX4));
                pmat->mat[0][0] = pmat->mat[1][1] = pmat->mat[2][2] = pmat->mat[3][3] = 1.0f;
            }
        }
    }
    for (X *px = p->pxFirst; px; px = px->pxNext) {
        LO *plo = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, px->oid, p->pvOwner);
        if (plo)
            AddWrWarpGroup(plo, px);
    }
}
} // namespace M_BindGlobset

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ExpandSphereForProjection { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct M4 { qword r[4]; };
extern const qword D_002BE570;
static inline qword Transform(const struct M4 *m, qword p)
{
    qword r;
    __asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w"
            : "=j"(r)
            : "j"(m->r[0]), "j"(m->r[1]), "j"(p), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one)
            : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    return f;
}
static inline qword MAdd(qword p, qword v, qword k)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(p), "j"(v), "j"(k));
    return r;
}
// Grows a sub/model cull sphere to include its projection through pshd->pmatClip (projected-volume geometry)
// (inferred)
qword ExpandSphereForProjection(const struct M4 *m, qword p, float *radius)
{
    U u;
    qword d = VuSub(Transform(m, D_002BE570), p);
    float len = Length(d);
    if (*radius < len) {
        float ratio = (len - *radius) * 0.5f / len;
        *radius = (len + *radius) * 0.5f;
        u.q = VuFromFloat<qword>(ratio);
        p = MAdd(p, d, u.q);
    }
    return p;
}
} } // namespace M_ExpandSphereForProjection

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_StoreRelightColors { extern "C" {
/* RGBA (one byte per qword lane) -> 1555 conversion. The C16 bitfield struct copy gives the target's dead
   register copies after each sh. */
typedef struct { unsigned char r, pad0[3], g, pad1[3], b, pad2[3], a, pad3[3]; } CQW;
/* the readback request: qword indices of the two result arrays and their end in VU1 memory, and the EE buffers
   (offsets from pbBase) the colours, the second alpha bytes and the alpha bytes go to */
struct X { int pad[2]; int iA; int iB; int iEnd; char *colorsA; char *colorsB; int pad2; char *alphaA; char *alphaB; };
struct S { char pad[0x3010]; int pbBase; char pad2[0x3070 - 0x3014]; struct X x; };
typedef struct { unsigned short rgb : 15, a : 1; } C16;
// Relight readback: converts the two RGBA results per vertex in VU1 data memory to RGBA5551 into the glob's EE colour
// buffers (+alpha bytes) (inferred)
void StoreRelightColors(struct S *p)
{
    struct X *px = &p->x;
    int pbBase = p->pbBase;
    CQW *pqwA = (CQW *)p + px->iA;
    CQW *pqwB = (CQW *)p + px->iB;
    CQW *pqwEnd = (CQW *)p + px->iEnd;
    C16 *ps1 = (C16 *)(px->colorsA + pbBase);
    C16 *ps2 = (C16 *)(px->colorsB + pbBase);
    unsigned char *pb2 = (unsigned char *)(px->alphaA + pbBase);
    unsigned char *pbAlpha = (unsigned char *)(px->alphaB + pbBase);
    for (; pqwB < pqwEnd; pqwB++, pqwA++) {
        C16 c1, c2;
        C16 *pd1, *pd2;
        pd1 = ps1++;
        pd2 = ps2++;
        {
            int g = (pqwA->g >> 3) << 5;
            int r = pqwA->r >> 3;
            c1.rgb = g | r | ((pqwA->b >> 3) << 10);
        }
        c1.a = pqwA->a >> 7;
        *pd1 = c1;
        {
            int r = pqwB->r >> 3;
            int g = (pqwB->g >> 3) << 5;
            c2.rgb = g | r | ((pqwB->b >> 3) << 10);
        }
        c2.a = pqwA->a >> 7;
        *pd2 = c2;
        *pbAlpha++ = pqwB->a;
        if (px->alphaA)
            *pb2++ = pqwA->a;
    }
}
} } // namespace M_StoreRelightColors

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_VU1_Interrupt { extern "C" {
void StoreRelightColors(int n);
void iFlushCache(int n);
// INTC VIF1/VU1 interrupt handler: reads back VU1 data memory results (colour conversion), clears the VIF1 stall
// (VIF1_FBRST=8), returns -1. (inferred)
int VU1_Interrupt(void)
{
    StoreRelightColors(0x1100C320);
    iFlushCache(0);
    *(volatile int *)0x10003C10 = 8;
    asm volatile("sync\n\tei");
    return -1;
}
} } // namespace M_VU1_Interrupt

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpRelight {
struct X { char pad[0x120]; QW amat[3]; int pad150[4]; int isDisabled; };   /* a light: 3 qwords of relight data; isDisabled (inferred): skipped */
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { extern char D_0025E090[]; }
extern "C" { extern char D_0025E668[]; }
extern "C" void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" int FindSwObjectsByClass(G *pg, int kind, int n, int m, int c, void **apv);
extern "C" void CopyAb__FPvT0Ui(void *pvDst, void *pvSrc, unsigned int cb);
// Pass 4 begin (kind 0): upload relight microprogram D_0025E090..D_0025E668 (compute only), BASE 0x32 OFFSET 0, upload
// up to 256 lights x 3 qw. (inferred)
extern "C" void BeginRpRelight(VIFS *pvifs)
{
    X *apx[0x180];
    int i;
    X *px;
    int cActive = 0;
    EnsureVu1Code__FP4VIFSPvT1(pvifs, D_0025E090, D_0025E668);
    pvifs->AddDmaCnt();
    pvifs->AddVifFlusha();
    pvifs->AddVifBaseOffset(0x32, 0);
    int c = FindSwObjectsByClass(&D_003DD490, 0x505, 4, 0, 0x180, (void **)apx);
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (!px->isDisabled)
            cActive++;
    }
    if (cActive > 0x100)
        cActive = 0x100;
    uint *pun;
    pvifs->UnpackHelper(UPK_V4_32, 1, 0, 0, &pun);
    pun[0] = 1;
    pun[1] = cActive * 3 + 1;
    uint *pb = 0;
    int n = 0;
    int iEnd = 0;
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (px->isDisabled)
            continue;
        if (n >= iEnd) {
            iEnd = n + MinInt(cActive - n, 32);
            pvifs->UnpackHelper(UPK_V4_32, (iEnd - n) * 3, n * 3 + 1, 0, &pb);
        }
        CopyAb__FPvT0Ui(pb, px->amat, 0x30);
        n++;
        pb = (uint *)((char *)pb + 0x30);
        if (n >= 0x100)
            break;
    }
    pvifs->EndDmaCnt();
}
} // namespace M_BeginRpRelight

// Pass 4 end: FLUSHA, restore BASE/OFFSET 0x32/0x1E7. (inferred)
extern "C" void EndRpRelight(VIFS *pvifs)
{
    pvifs->AddDmaCnt();
    pvifs->AddVifFlusha();
    pvifs->AddVifBaseOffset(0x32, 0x1E7);
    pvifs->EndDmaCnt();
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SubmitGlobsetRelight {
// SubmitGlobsetRelight (Claude agent, T1651): builds one VIF packet per entry of a model's packet list: a DMA CNT tag, an UNPACK of
// 6 qwords to VU address 0x301 (entry word + bounding sphere + model matrix), padding, and a DMA RET tag, then queues it
// in a render-list entry (AllocRenderListEntry) with the descriptor's chain. Stamps the game tick at the end.
// From lanef9v1 (aligned 5) plus BuildModelDrawPackets's packet idioms (template UNPACK tag, CopyQws with the empty do-while).
// STAND-IN (same trick as ApplySpriteKeyPoses / LoadOptionFromBrx): `if (fNever)` is dead code (gcse's last cprop folds it, jump
// deletes it; the final code is unchanged). It only supplies the extra basic block the game had between the UNPACK tag and
// the copy loop at gcse time: gcse PRE then inserts the %hi(D_002DEA28) copy (t2) before that block and the outer `i + 1`
// (s4) after it, below the do-while barrier. What the game really had there is unknown.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
union U
{
    qword q;
    float f[4];
};
struct M3
{
    U r[3];
};
struct M4
{
    U r[4];
};
struct V3
{
    float f[3];
};
struct SPHERE
{
    V3 center;
    float radius;
};
struct DESC
{
    char pad[8];
    void *chain;
    char padC[8];
};
struct XF
{
    char pad[0x40];
    M4 matrix;
};
struct ENTRY
{
    int unknown;
    XF *transform;
    M4 *parent;
    uint flags;
    char *matrixData;
    int word;
    char pad18[0xC];
    SPHERE *sphere;
};
struct LOCAL_SPHERE
{
    SPHERE sphere;
    char pad[8];
};
struct PACKETS
{
    char pad[0x38];
    int count;
    DESC *descriptors;
    ENTRY *entries;
    LOCAL_SPHERE *spheres;
    int pad48;
    int tick;
};
struct PACKET
{
    int word;
    int reserved[3];
    U sphere;
    M4 matrix;
};
struct DMA
{
    char pad[0xC];
    uchar *pb;
};
struct RPL
{
    int kind, pad4;
    uchar *pb;
    void *chain;
};
struct G
{
    char pad[0x3D5C];
    int tick;
};
extern "C" { extern DMA D_002DEA28; }
extern "C" { extern G D_003DD490; }
extern "C" {
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb);
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
M4 *__ml__FRC7MATRIX4T0(M4 *pmatDst, const M4 *pmatA, const M4 *pmatB);
float GetMaxRowLengthMatrix3(const M3 *pmat);
RPL *AllocRenderListEntry(void);
}
static inline qword QwTransformPos(M4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(pmat->r[0].q), "j"(pmat->r[1].q), "j"(pmat->r[2].q), "j"(pmat->r[3].q), "j"(qwPos));
    return qwResult;
}
static inline void CopyQws(uint *pwDst, const uint *pwSrc, int cw, int fNever)
{
    if (fNever) // STAND-IN for the game's dead code (see the top of the file)
        *pwDst = 0;
    // empty do-while(0) (as left by a statement macro): its loop notes keep later setup below the tag/pb stores
    do
    {
    } while (0);
    for (int k = 0; k < cw; k += 4)
    {
        uint w0 = pwSrc[0];
        uint w1 = pwSrc[1];
        uint w2 = pwSrc[2];
        uint w3 = pwSrc[3];
        pwSrc += 4;
        pwDst[0] = w0;
        pwDst[1] = w1;
        pwDst[2] = w2;
        pwDst[3] = w3;
        pwDst += 4;
    }
}
template <uint TAG> static inline void UnpackQws(const void *pv, int cw, int fNever)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = TAG;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw, fNever);
    pdma->pb += cw * 4;
}
static inline M4 ModelMatrix(const ENTRY *entry) return model
{
    const M4 *parent = entry->parent;
    func_0023320C(&model, &entry->transform->matrix, sizeof(M4));
    if (parent)
    {
        M4 product;
        __ml__FRC7MATRIX4T0(&product, &model, parent);
        func_0023320C(&model, &product, sizeof(M4));
    }
}
// Queues a kind-0 RPL per model packet: CNT + UNPACK 6 qw (EE dest word, sphere, matrix) to VU TOPS+0x301 + RET;
// stamps the model with g_sw+0x3D5C. (inferred)
extern "C" void SubmitGlobsetRelight(PACKETS *p)
{
    int fNever = 0;
    int i;
    for (i = 0; i < p->count; i++)
    {
        DESC *descriptor = &p->descriptors[i];
        ENTRY *entry = &p->entries[i];
        PACKET packet;
        if (!descriptor->chain)
            continue;
        packet.word = entry->word;
        if (entry->flags & 0x1000)
            CopyAb__FPvT0Ui(&packet.matrix, entry->matrixData + 0x14, sizeof(M4));
        else
            packet.matrix = ModelMatrix(entry);
        if (entry->sphere)
        {
            *(V3 *)&packet.sphere = entry->sphere->center;
            packet.sphere.f[3] = entry->sphere->radius;
        }
        else
        {
            // a stride variable (not the constant in the index) is what loop.c hoists into s7
            int sphereStride = sizeof(LOCAL_SPHERE);
            LOCAL_SPHERE *sphere = (LOCAL_SPHERE *)((char *)p->spheres + i * sphereStride);
            U center;
            CopyAb__FPvT0Ui(&center, &sphere->sphere.center, sizeof(V3));
            packet.sphere.q = QwTransformPos(&packet.matrix, center.q);
            packet.sphere.f[3] = GetMaxRowLengthMatrix3((const M3 *)&packet.matrix) * sphere->sphere.radius;
        }
        ulong *ptag = (ulong *)D_002DEA28.pb;
        *ptag = 0x10000000;
        D_002DEA28.pb += 0x10;
        UnpackQws<0x6C06C301>(&packet, sizeof(PACKET) / sizeof(uint), fNever);
        {
            DMA *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->pb; (uint)pbPad & 0xF; pbPad = pdma->pb)
            {
                *(uint *)pbPad = 0;
                pdma->pb += 4;
            }
        }
        *(uint *)ptag |= ((D_002DEA28.pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.pb = 0x60000000;
        D_002DEA28.pb += 0x10;
        RPL *prpl = AllocRenderListEntry();
        prpl->pb = (uchar *)ptag;
        prpl->kind = 0;
        prpl->pad4 = 0;
        prpl->chain = descriptor->chain;
    }
    p->tick = D_003DD490.tick;
}
} // namespace M_SubmitGlobsetRelight

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UnpackSubglobBones { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef struct M4 { qword r[4]; } M4;
typedef struct TRANSFORM { char pad0[0x40]; M4 world; } TRANSFORM;
typedef struct BONE {
    int value, unknown4, index;
    TRANSFORM *transform;
    M4 local;
} BONE;
typedef struct DESC {
    unsigned char flags;
    signed char count;
    unsigned short channel;
    float distance;
    int unknown8;
    uint base;
} DESC;
typedef struct ENTRY { char pad0[0x18]; BONE *bones; } ENTRY;
typedef struct DMA { uint fields[3]; uint *cur; } DMA;
extern DMA D_002DEA28;

static __inline__ qword ReadCPU(const qword *source)
{
    qword value;
    __asm__("lq %0,%1" : "=r"(value) : "m"(*source));
    return value;
}

static __inline__ void Align(uint align)
{
    DMA *dma = &D_002DEA28;
    qword *output = (qword *)dma->cur;
    while (((uint)output & 15) != align) {
        *(uint *)output = 0;
        dma->cur++;
        output = (qword *)dma->cur;
    }
}

// Appends VIF UNPACKs of a sub-glob's bone matrices to the instance packet. (inferred)
void UnpackSubglobBones(void *context, DESC *descriptor, ENTRY *entry)
{
    uint opcode;
    BONE *bone, *last;
    TRANSFORM *parent;
    qword *output;
    qword p0, p1, p2, p3, n0, n1, n2, n3;
    register qword v1 __asm__("vf1"), v2 __asm__("vf2");
    register qword v3 __asm__("vf3"), v4 __asm__("vf4");
    register qword v5 __asm__("vf5"), v6 __asm__("vf6");
    register qword v7 __asm__("vf7"), v8 __asm__("vf8");

    Align(12);
    opcode = (uint)descriptor->count << 18;
    opcode |= 0x6000C000;
    opcode |= 0x0C000000;
    opcode |= descriptor->base;
    *D_002DEA28.cur = opcode;
    D_002DEA28.cur++;
    output = (qword *)D_002DEA28.cur;
    bone = entry->bones;
    last = bone + descriptor->count - 1;
    parent = bone->transform;
    v5 = bone->local.r[0]; v6 = bone->local.r[1];
    v7 = bone->local.r[2]; v8 = bone->local.r[3];
    v1 = parent->world.r[0]; v2 = parent->world.r[1];
    v3 = parent->world.r[2]; v4 = parent->world.r[3];
    for (;;) {
        __asm__ __volatile__("vcallms 0x60" : :
                "j"(v1), "j"(v2), "j"(v3), "j"(v4),
                "j"(v5), "j"(v6), "j"(v7), "j"(v8));
        if (bone < last) {
            TRANSFORM *next = bone[1].transform;
            p0 = ReadCPU(&next->world.r[0]);
            p1 = ReadCPU(&next->world.r[1]);
            p2 = ReadCPU(&next->world.r[2]);
            p3 = ReadCPU(&next->world.r[3]);
            n0 = ReadCPU(&bone[1].local.r[0]);
            n1 = ReadCPU(&bone[1].local.r[1]);
            n2 = ReadCPU(&bone[1].local.r[2]);
            n3 = ReadCPU(&bone[1].local.r[3]);
        }
        {
            int sync;
            register qword result0 __asm__("vf5");
            register qword result1 __asm__("vf6");
            register qword result2 __asm__("vf7");
            register qword result3 __asm__("vf8");
            /* Publish the completed microprogram outputs after the interlock. */
            __asm__ __volatile__("cfc2.i %0,$vi0"
                    : "=r"(sync), "=j"(result0), "=j"(result1),
                      "=j"(result2), "=j"(result3));
            *output++ = result0;
            *output++ = result1;
            *output++ = result2;
            *output++ = result3;
        }
        if (bone == last)
            break;
        v1 = p0; v2 = p1; v3 = p2; v4 = p3;
        v5 = n0; v6 = n1; v7 = n2; v8 = n3;
        bone++;
    }
    D_002DEA28.cur = (uint *)output;
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_M4[(sizeof(M4)==0x40)?1:-1];
typedef char layout_BONE[(sizeof(BONE)==0x50)?1:-1];
typedef char layout_DESC[(sizeof(DESC)==0x10)?1:-1];
typedef char layout_TRANSFORM_world[((unsigned long)&((TRANSFORM*)0)->world==0x40)?1:-1];
typedef char layout_BONE_transform[((unsigned long)&((BONE*)0)->transform==0xc)?1:-1];
typedef char layout_BONE_local[((unsigned long)&((BONE*)0)->local==0x10)?1:-1];
typedef char layout_DESC_count[((unsigned long)&((DESC*)0)->count==0x1)?1:-1];
typedef char layout_DESC_base[((unsigned long)&((DESC*)0)->base==0xc)?1:-1];
typedef char layout_ENTRY_bones[((unsigned long)&((ENTRY*)0)->bones==0x18)?1:-1];
typedef char layout_DMA_cur[((unsigned long)&((DMA*)0)->cur==0xc)?1:-1];
typedef char layout_M4_row0[((unsigned long)&((M4*)0)->r[0]==0x0)?1:-1];
typedef char layout_M4_row1[((unsigned long)&((M4*)0)->r[1]==0x10)?1:-1];
typedef char layout_M4_row2[((unsigned long)&((M4*)0)->r[2]==0x20)?1:-1];
typedef char layout_M4_row3[((unsigned long)&((M4*)0)->r[3]==0x30)?1:-1];
} } // namespace M_UnpackSubglobBones

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_RenderSoGlobset {
// RenderSoGlobset (Claude agent, lane Z1, 2026-10-30): MATCH, 728/728, frame 0x290. Source = build/hand_wip/RenderSoGlobset.lilly5.cpp unchanged.
//   Policy: no GPR/FPU pins. The only register bindings are the microprogram 0x1C0 I/O registers (vf10..vf19) inside the
//   inline GetSphereVisibility/GetSphereVisibilitySync helpers (allowed: VU microprogram registers bound inside an inline helper). All other asms
//   are VU helper inlines (lw/qmfc2/mtc1 VU_FLOAT-style, as in Lilly's files) or commented empty asms; globals are D_ names only.
// lilly5 (Claude agent, T1703, 2026-10-26): MATCH (728/728, frame 0x290). NOT in src/hand only because of the team's one-pin rule:
//   the microprogram 0x1C0 inputs/outputs are register variables bound to vf10..vf19 (20 `register ... __asm__("vfN")`, as in
//   UnpackSubglobBones.c / ProjectBlipTransformAccel lilly1.c); the orchestrator counts each as a pin. No GPR/FPU pins.
//   From lilly3 (aligned 2), cull 2 (GetSphereVisibilitySync) only: (1) qmfc2 straight into a float "=r" (the qmfc2 output becomes a reload
//   in v0, reload's copy is the mtc1 v0,f0, so the i+1 reload after it gets v1 by round robin); (2) cfc2 outputs only vf19, an empty
//   `"=j"(qwDpos)` asm declares vf18, and one empty `"+j"(qwDpos)` asm with a dead "$3" clobber: two no-code insns ready right after
//   the cfc2 fill sched1's free slots (the PRE i+1 insn now issues after the swc1 gVisSub), and the $3 clobber holds the addiu v1
//   in sched2 until after it -> target `c.eq.s; swc1; bc1t; sw v1,0x1A4`. The same with two "+j" asms also MATCHes.
// (probe) lilly4 (Claude agent, T1700, 2026-10-26): PROBE, aligned 4 (lilly3 = 2 stays the best score), 728 instructions, frame 0x290, no pins.
//   Shows the mechanism behind lilly3's last diff (cull-2 tail sw v1,0x1A4 / swc1 f0,0x158). Proven with -fno-schedule-insns: if sched1
//   leaves the i+1 PRE insn AFTER the gVisSub store, sched2 + reorg give the target's swc1; bc1t; sw. In sched1 that insn is ready right
//   after the volatile vcallms and takes any free second issue slot (t13, t14), so it needs two insns ready there that vanish later.
//   Here: two empty "+j"(qwDpos) asms after the cfc2 (no code) -> the store order is now the target's. Left (4, registers only):
//   qmfc2/mtc1 use a0 (target v0) and the i+1 reload uses v0 (target v1). Without the "$2" clobber on the first filler n is v0 but the
//   i+1 reload also takes v0 (reload round-robin: the target's last reload before it must have used v0; a qmfc2 "=r,m" output into
//   *(int *)&gVisSub gives v1 but leaves a dead sw v0,0x158). Not a source form to keep: it is a map for the next attempt.
// lilly3 (Claude agent, T1698, 2026-10-26): aligned 2 (lilly2 11), 728 instructions, frame 0x290, no pins. Cull 1 and the cull-2 body MATCH.
//   Key: the target's three HOME stores (sq r,0(sp) / 0x50(sp)) are the asm-output temps of the VU_FLOATM ctors (MEM_SCALAR_P at a fixed
//   frame address in sched1). gcc 2.95's fixed_scalar_and_varying_struct_p lets a load whose MEM is a struct member at a varying address
//   pass them, so the range is read from the SPH itself (VU_FLOATS, "m"(psph->sRange)) LAST in source order; sched1 hoists the load
//   above the gain/radius HOME stores while its HOME store stays last. OBJ+0x28 is an embedded SPH (pos[3], rad, sRange at OBJ+0x38),
//   so both culls pass a const SPH *. Gain is VU_FLOATM too (all three are lw asms: unit none, 1-cycle links -> target issue order).
//   Cull 2 = combined-body loads + split vcallms/cfc2 + int qmfc2 ($3 clobber) + mtc1 asm.
//   Left (2): cull-2 tail `swc1 f0,0x158` / `sw v1,0x1A4` (sched2 tie prio 2/class 3/1 dep -> LUID; sched1 issues the i+1 PRE insn 2417
//   in the free slot at t13, before the gVisSub store, so reload's spill sw precedes swc1). Needs two more ready insns after cfc2 in sched1.
// lilly2 (Claude agent, T1691, 2026-10-26): aligned 11 (lilly1 14), 728 instructions, frame 0x290, no pins.
//   Cull 2's range is now a VU_FLOATM (asm lw) built at the call site and passed as a qword: the range load stays first and the
//   registers are the target's (range a0, radius v1, gain v0; one pseudo for range instead of the lw + tied empty asm qty,
//   so it no longer outranks the radius in local-alloc). Left: (a) cull 1 load order/registers (unchanged from lilly1);
//   (b) cull 2 range HOME sq now issues FIRST (the call-site temp is stored at the call), target stores gain, radius, range;
//   (c) cull-2 tail sw v1,0x1A4 / swc1 f0,0x158 (sched2 tie: both prio 2, class 3, 1 dependent; the reload spill's LUID wins
//   because sched1 issues the i+1 PRE addiu at t14, two cycles before the swc1).
// lilly1 (Claude agent, T1687, 2026-10-25): aligned 14 (728 instructions, frame 0x290; laneH21v2 30). Policy-clean: no register pins, VU helper asms only.
//   From laneH21v2: (1) cull 2 radius loaded by a `lw` asm straight into the qword (VU_FLOATM, as SetBentleyHelperState): real-load timing, target order (30 -> 28).
//   (2) case-3 SqDist: the compiler moves the length out of vf (qmfc2) and an `mtc1` asm makes the float (GetX idiom, SteerRobottankAlongPath): mtc1 before add.s (28 -> 26).
//   (3) cull-2 result: qmfc2 asm into an int (dead-$3 clobber kept) + separate `mtc1` asm: c.eq.s/swc1 one cycle earlier, the hazard nop and the 1-insn shift are gone (26 -> 15).
//   (4) UnpackWords copy as `for (k = 0; k < cw; k++)`: loop.c creates the counter init after gcse's &matModel PRE insn, as BuildModelDrawPackets (15 -> 14).
//   Left: cull 1 load/sq order + registers (target: radius lw after gain, regs v1/a0/v0/a2), cull 2 radius/range registers swapped (target radius v1, range a0),
//   cull-2 tail `sw v1,0x1A4` / `swc1 f0,0x158` (sched1 puts the i+1 PRE insn before the swc1, so reload's spill store wins the sched2 luid tie).
// laneH21v2 (Claude agent): aligned 30. laneI9v1 + CopyQws as `do { } while (0);` + for-k copy loop (no asm barrier; 31) + cull 2 through GetSphereVisibilitySync (vcallms and the cfc2.i sync as separate asms: target mtc1 zero / qmfc2 order) with a commented dead-$3 clobber on its qmfc2 (30).
// laneI9v1 (Claude agent): aligned 31. laneI8v1 + final dist qmfc2 written straight to prpl->dist via "=r,m" : "j,!r" (never-chosen m alternative keeps the MEM; the qmfc2 output reload in v0 advances reload's round robin, so the pbStart reload gets v1 as in the target).
// laneI8v1 (Claude agent): aligned 33. laneI7v2 + KindOf as (ulong)(psub->grf & 0xF8) >> 3 (target lbu + dsrl in the final switch, 35 -> 33).
// laneI7v2 (Claude agent): aligned 35. laneI6v2 + params (pmodel, pmatWorld, gFirst, gSecond, pshd) (entry pshd spill order, 51 -> 48) + case-3 SqDist returns float straight from qmfc2 (48 -> 35).
// laneI6v2 (Claude agent): aligned 51. laneI5v4 + final GetSquaredLengthBits with qwOne as operand 0, case 3 GBitsSqDist (old order), fAlt kind as (x != 1.0f) ? 23 : 22.
// laneI5v4 (Claude agent): aligned 57. laneI4v5 + pad loop via `DMA *pdma` and a `pbPad` cursor, skin offset `int ibMat = imat*0x40 + 0x6F0`, commented empty volatile asm at the start of CopyQws.
// RenderSoGlobset (Claude agent, lane I3): fresh rewrite in the style of Lilly's matched files.
// laneI4v5 (Claude agent): aligned 120 (raw 179), frame 0x290. v4 + posCam, pos, sRadius declaration order.
// Culls a model against the view (microprogram 0x1C0), then builds the VIF packet for every visible submodel
// (optional reference, model matrix, params, bone matrices (0x1684D0), skin matrices) and queues it for rendering.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef int qword __attribute__((mode(TI)));
struct MATRIX4
{
    float mat[4][4];
} __attribute__((aligned(16)));
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct XF
{
    char pad[0x40];
    MATRIX4 localMatrix;
};
struct VTENT
{
    short delta, pad;
    void (*pfn)(void *);
};
struct SPH
{
    float pos[3];
    float rad;
    float sRange;
    int pad;
};
struct OBJ
{
    char pad[0xC];
    VTENT *pvt;
    char pad10[0x18];
    SPH sph;
    char pad40[0x20];
    XF *pxf;
};
struct BONE;
struct SKIN
{
    char pad[0xC];
    char *pbOwner;
    int cmat;
    int fFirst;
    int fSecond;
    int pad1C;
    uint ib;
};
struct SUBDESC
{
    signed char kind;
    signed char cbone;
    short channel;
    float dist;
    void *pv;
    uint ib;
    uint cw;
};
struct SUB
{
    int n;
    XF *pxf;
    MATRIX4 *pmatParent;
    uint grf;
    char *pb;
    uint w;
    BONE *abone;
    SKIN *pskin;
    int pad20;
    qword *ppos;
};
struct MODEL
{
    OBJ *powner;
    float t;
    char pad8[8];
    VECTOR4 posCenter;
    float rad;
    int cobj;
    OBJ **apobj;
    char pad2C[8];
    uint grfMode;
    int csub;
    SUBDESC *asubdesc;
    SUB *asub;
    SPH *asph;
    int pad48;
    int nStamp;
    union
    {
        ulong grf;
        float scale;
    };
};
struct SHD
{
    uint aw[9];
    MATRIX4 *pmatClip;
    uint ibRef;
    uint cqwRef;
};
struct VIEW
{
    char pad[0x1C];
    float sGain;
    char pad20[0x28];
    float sNear;
    char pad4C[0x44];
    qword posCam;
    qword aqwPlane[3];
};
struct CLOCK
{
    float time, deltaTime;
};
struct G
{
    char pad[0x3BEC];
    int fAlt;
    char pad3BF0[0x16C];
    int nStamp;
};
struct DMA
{
    char pad[0xC];
    uchar *pb;
};
struct RPL
{
    int kind;
    float dist;
    uchar *pb;
    char *pv;
};
extern "C" { extern DMA D_002DEA28; }
extern "C" { extern G D_003DD490; }
extern "C" { extern VIEW D_002DDF40; }
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern MATRIX4 D_002A2D30; }
extern "C" {
void SubmitGlobsetRelight(MODEL *pmodel);
int ShouldUpdateSaa__FP3SAA(OBJ *pobj);
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb);
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB);
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc);
float GetMaxRowLengthMatrix3(const MATRIX4 *pmat);
qword ExpandSphereForProjection(const MATRIX4 *pmat, qword qwPos, float *psRadius);
int IsWithinCameraRenderDistance(VIEW *pview, qword qwDpos, float sRange, float *pgVis);
void UnpackSubglobBones(MODEL *pmodel, SUBDESC *pdesc, SUB *psub);
RPL *AllocRenderListEntry(void);
}
static inline MATRIX4 MulMat(const MATRIX4 &matA, const MATRIX4 &matB) return mat
{
    __ml__FRC7MATRIX4T0(&mat, &matA, &matB);
}
static inline void CopyMat(MATRIX4 &matDst, const MATRIX4 &matSrc)
{
    func_0023320C(&matDst, &matSrc, sizeof(MATRIX4));
}
static inline qword TransformPoint(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
static inline qword TransformPointRef(const MATRIX4 &mat, const qword &qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
static inline void StoreSquaredLengthBits(int &gDst, qword qwV)
{
    qword qwSq, qwOne;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    // the "m" / "!r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
    __asm__("qmfc2.ni %0, %1" : "=r,m"(gDst) : "j,!r"(qwSq));
}
static inline int GetSquaredLengthBits(qword qwV)
{
    qword qwSq, qwOne;
    int g;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwSq));
    return g;
}
static inline float SqDist(qword qw0, qword qw1)
{
    qword qwV = VuSub(qw0, qw1);
    qword qwSq, qwOne;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qwSq), "=&j"(qwOne) : "j"(qwV));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(qwSq));
    return g;
}
// the word loaded straight into the qword's register (as SetBentleyHelperState's VU_FLOATM)
struct VU_FLOATM
{
    qword qw;
    VU_FLOATM(const float &g) { __asm__("lw %0, %1" : "=r"(qw) : "m"(g)); }
};
// The sphere's range read straight from the SPH: the struct-member MEM (MEM_IN_STRUCT_P, varying address) does not alias the
// scalar VU_FLOAT temp HOME at a fixed frame address, so sched1 hoists this load above the gain/radius HOME stores (target order).
struct VU_FLOATS
{
    qword qw;
    VU_FLOATS(const SPH *psph) { __asm__("lw %0, %1" : "=r"(qw) : "m"(psph->sRange)); }
};
// Microprogram 0x1C0 (sphere against the view planes): its inputs and outputs live in fixed VU registers.
static inline float GetSphereVisibility(qword posCam, qword pos, const SPH *psph, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = D_002DDF40.aqwPlane;
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(D_002DDF40.sGain).qw;
    register qword qwPos __asm__("vf15") = pos;
    register qword qwRadius __asm__("vf16") = VU_FLOATM(sRadius).qw;
    register qword qwRange __asm__("vf17") = VU_FLOATS(psph).qw;
    register qword qwDpos __asm__("vf18");
    register qword qwVis __asm__("vf19");
    __asm__ __volatile__("vcallms 0x1C0\n\tcfc2.i $zero, $vi0"
                         : "=j"(qwDpos), "=j"(qwVis)
                         : "j"(qwPlane0), "j"(qwPlane1), "j"(qwPlane2), "j"(qwCam), "j"(qwGain), "j"(qwPos), "j"(qwRadius), "j"(qwRange)
                         : "vf20", "vf21", "vf22", "vf23", "vf24", "vf25");
    *pdpos = qwDpos;
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwVis));
    return g;
}
// Same cull, with the VU0 sync as its own statement and the visibility moved out through a GPR.
static inline float GetSphereVisibilitySync(qword posCam, qword pos, const SPH *psph, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = D_002DDF40.aqwPlane;
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(D_002DDF40.sGain).qw;
    register qword qwPos __asm__("vf15") = pos;
    register qword qwRadius __asm__("vf16") = VU_FLOATM(sRadius).qw;
    register qword qwRange __asm__("vf17") = VU_FLOATS(psph).qw;
    register qword qwDpos __asm__("vf18");
    register qword qwVis __asm__("vf19");
    __asm__ __volatile__("vcallms 0x1C0" : : "j"(qwPlane0), "j"(qwPlane1), "j"(qwPlane2), "j"(qwCam), "j"(qwGain), "j"(qwPos), "j"(qwRadius), "j"(qwRange) : "vf18", "vf19", "vf20", "vf21", "vf22", "vf23", "vf24", "vf25");
    // wait for the microprogram (cfc2 interlocks on it); vf19 = the visibility
    __asm__("cfc2.i $zero, $vi0" : "=j"(qwVis));
    // vf18 = the camera-to-sphere vector the microprogram left behind (no code)
    __asm__("" : "=j"(qwDpos));
    // scheduling-only barrier (no code): with the empty asm above it fills sched1's free issue slots after the cfc2, so the
    // i+1 PRE insn issues after the gVisSub store; its dead $3 clobber keeps the i+1 addiu (v1) after it in sched2
    __asm__("" : "+j"(qwDpos) : : "$3");
    *pdpos = qwDpos;
    // float "=r" output: the qmfc2 goes to a reload register (v0) and reload's copy is the mtc1 v0,f0 (next reload: v1)
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwVis));
    return g;
}
static inline void CopyQws(uint *pwDst, const uint *pwSrc, int cw)
{
    // empty do-while(0) (as left by a statement macro): its loop notes keep later setup below the tag/pb stores
    do
    {
    } while (0);
    for (int k = 0; k < cw; k += 4)
    {
        uint w0 = pwSrc[0];
        uint w1 = pwSrc[1];
        uint w2 = pwSrc[2];
        uint w3 = pwSrc[3];
        pwSrc += 4;
        pwDst[0] = w0;
        pwDst[1] = w1;
        pwDst[2] = w2;
        pwDst[3] = w3;
        pwDst += 4;
    }
}
template <uint TAG> static inline void UnpackQws(const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = TAG;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
static inline void UnpackQws(uint tag, const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = tag;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
static inline void UnpackWords(uint tag, const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = tag;
    pdma->pb += 4;
    uint *pwDst = (uint *)pdma->pb;
    const uint *pwSrc = (const uint *)pv;
    for (int k = 0; k < cw; k++)
        *pwDst++ = *pwSrc++;
    pdma->pb += cw * 4;
}
static inline int KindOf(SUB *psub)
{
    return (ulong)(psub->grf & 0xF8) >> 3;
}
// Per-model draw: sphere-culls each sub-model against the camera via VU0 microprogram 0x1C0, updates shader
// animations, builds/queues VIF packets (0x1681F8/0x1684D0) (inferred)
extern "C" void RenderSoGlobset(MODEL *pmodel, MATRIX4 *pmatWorld, float gFirst, float gSecond, SHD *pshd)
{
    SUBDESC *pdesc;
    qword posCam = D_002DDF40.posCam;
    qword pos = TransformPointRef(pmodel->powner->pxf->localMatrix, *(qword *)&pmodel->posCenter);
    float sRadius = pmodel->rad;
    if (pmatWorld)
    {
        pos = TransformPoint(*pmatWorld, pos);
        sRadius *= GetMaxRowLengthMatrix3(pmatWorld);
    }
    if ((pmodel->grf & 0x200000000UL) && pshd->pmatClip)
        pos = ExpandSphereForProjection(pshd->pmatClip, pos, &sRadius);
    qword dpos;
    float gVis = GetSphereVisibility(posCam, pos, &pmodel->powner->sph, sRadius, &dpos);
    if (gVis == 0.0f)
        return;
    pmodel->t = D_002DDED0.time;
    if (pmodel->nStamp != D_003DD490.nStamp)
        SubmitGlobsetRelight(pmodel);
    int i;
    for (i = 0; i < pmodel->cobj; i++)
    {
        OBJ *pobj = pmodel->apobj[i];
        if (ShouldUpdateSaa__FP3SAA(pobj))
        {
            VTENT *pvte = &pobj->pvt[8];
            pvte->pfn((char *)pobj + pvte->delta);
        }
    }
    int fAlt = 0;
    if (pmodel->grf & 0x400000000UL)
        fAlt = D_003DD490.fAlt == 0;
    for (i = 0; i < pmodel->csub; i++)
    {
        pdesc = &pmodel->asubdesc[i];
        SUB *psub = &pmodel->asub[i];
        SPH *psph = &pmodel->asph[i];
        VECTOR4 posSph;
        MATRIX4 matModel;
        if (psub->grf & 0x800)
            continue;
        if (psub->grf & 0x1000)
        {
            CopyAb__FPvT0Ui(&matModel, psub->pb + 0x14, sizeof(MATRIX4));
            if (pmatWorld)
                matModel = MulMat(*pmatWorld, matModel);
        }
        else
        {
            MATRIX4 *pmatParent = psub->pmatParent;
            MATRIX4 mat;
            CopyMat(mat, psub->pxf->localMatrix);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            if (pmatWorld)
                CopyMat(mat, MulMat(*pmatWorld, mat));
            matModel = mat;
        }
        CopyAb__FPvT0Ui(&posSph, psph, 12);
        qword posSub = TransformPoint(matModel, *(qword *)&posSph);
        float sRadiusSub = GetMaxRowLengthMatrix3(&matModel) * psph->rad;
        if ((psub->grf & 7) == 2 && pshd->pmatClip)
            posSub = ExpandSphereForProjection(pshd->pmatClip, posSub, &sRadiusSub);
        float gVisSub;
        qword dposSub;
        if (!psub->abone)
        {
            qword dposCull;
            gVisSub = GetSphereVisibilitySync(posCam, posSub, psph, sRadiusSub, &dposCull);
            if (gVisSub == 0.0f)
                continue;
            dposSub = dposCull;
        }
        else
        {
            dposSub = VuSub(posSub, posCam);
            if (!IsWithinCameraRenderDistance(&D_002DDF40, dposSub, psph->sRange, &gVisSub))
                continue;
        }
        uchar *pbStart = D_002DEA28.pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->aw[5] | 0x40000000) | ((ulong)(int)pshd->aw[6] << 32);
            D_002DEA28.pb += 0x10;
        }
        ulong *ptag;
        switch (psub->grf & 7)
        {
        case 2:
            if (!pshd->cqwRef)
                continue;
            *(ulong *)D_002DEA28.pb = (ulong)(int)(pshd->ibRef | 0x40000000) | ((ulong)pshd->cqwRef << 32);
            D_002DEA28.pb += 0x10;
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            D_002DEA28.pb += 0x10;
            break;
        case 3:
        {
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            float g = SqDist(posCam, posSub);
            float sNear = sRadiusSub + D_002DDF40.sNear;
            D_002DEA28.pb += 0x10;
            int fNear = g < sNear * sNear;
            UnpackWords(0x6001C012, &fNear, 1);
            break;
        }
        default:
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            D_002DEA28.pb += 0x10;
            break;
        }
        if (psub->abone)
        {
            MATRIX4 *pmat = pmatWorld;
            if (!pmat)
                pmat = &D_002A2D30;
            UnpackQws<0x6C04C000>(pmat, 16);
        }
        else
        {
            UnpackQws<0x6C04C000>(&matModel, 16);
        }
        VECTOR4 vecParams;
        if (gVis < gVisSub)
        {
            vecParams.x = gFirst * gVis;
            vecParams.y = gSecond * gVis;
        }
        else
        {
            vecParams.x = gFirst * gVisSub;
            vecParams.y = gSecond * gVisSub;
        }
        vecParams.z = pmodel->scale;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->grfMode : 0;
        UnpackQws<0x6C01C008>(&vecParams, 4);
        if (psub->abone)
            UnpackSubglobBones(pmodel, pdesc, psub);
        SKIN *pskin = psub->pskin;
        if (pskin)
        {
            MATRIX4 matInv;
            InvertMatrix4Rigid(&matInv, &matModel);
            uint ib = pskin->ib;
            for (int imat = 0; imat < pskin->cmat; imat++)
            {
                if (pskin->fFirst)
                {
                    MATRIX4 mat;
                    int ibMat = imat * 0x40 + 0x6F0;
                    mat = MulMat(matInv, *(MATRIX4 *)(pskin->pbOwner + ibMat));
                    UnpackQws(ib | 0x6C04C000, &mat, 16);
                    ib += 4;
                }
                if (pskin->fSecond)
                {
                    UnpackQws(ib | 0x6C04C000, pskin->pbOwner + 0x7F0 + imat * 0x40, 16);
                    ib += 4;
                }
            }
        }
        {
            DMA *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->pb; (uint)pbPad & 0xF; pbPad = pdma->pb)
            {
                *(uint *)pbPad = 0;
                pdma->pb += 4;
            }
        }
        *(uint *)ptag |= ((int)(D_002DEA28.pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.pb = 0x60000000;
        D_002DEA28.pb += 0x10;
        RPL *prpl = AllocRenderListEntry();
        switch (KindOf(psub))
        {
        case 3:
        case 4:
            if (fAlt)
                prpl->kind = (vecParams.x != 1.0f) ? 23 : 22;
            else if (vecParams.x == 1.0f)
                prpl->kind = KindOf(psub);
            else
                prpl->kind = ((psub->grf & 0xF8) == 0x18) ? 5 : 6;
            break;
        case 5:
        case 6:
            prpl->kind = fAlt ? 23 : KindOf(psub);
            break;
        case 9:
            prpl->kind = fAlt ? 24 : KindOf(psub);
            break;
        case 10:
            prpl->kind = fAlt ? 25 : KindOf(psub);
            break;
        default:
            prpl->kind = KindOf(psub);
            break;
        }
        if (psub->grf & 0x400)
            prpl->dist = pdesc->dist;
        else
            StoreSquaredLengthBits(*(int *)&prpl->dist, dposSub);
        prpl->pb = pbStart;
        prpl->pv = psub->pb;
    }
}
} // namespace M_RenderSoGlobset

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BuildModelDrawPackets {
// BuildModelDrawPackets (Claude agent, lanes I1-I9, H21): fresh rewrite in the style of Lilly's matched files.
// H21: the copy loop is a `for` counter loop (loop.c turns it into the target's 12/-4/bgez count) preceded by an
// empty do-while(0), whose loop notes act as the scheduling barrier the target shows after `pb += 4`.
// Builds the VIF packet for every submodel of a model: model matrix, optional bone matrices, optional skin matrices,
// then queues the packet with its sort distance.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef int qword __attribute__((mode(TI)));
struct MATRIX4
{
    float mat[4][4];
} __attribute__((aligned(16)));
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct XF
{
    char pad[0x40];
    MATRIX4 localMatrix;
};
struct VTENT
{
    short delta, pad;
    void (*pfn)(void *);
};
struct OBJ
{
    char pad[0xC];
    VTENT *pvt;
};
struct SHD
{
    uint aw[12];
};
struct ALO
{
    char pad[0x104];
    int cpshd;
    void *apshd[1];
};
struct BONE
{
    int oid;
    float g;
    short n, pad;
    XF *pxf;
    MATRIX4 mat;
};
struct SKIN
{
    char pad[0xC];
    char *pbOwner;
    int cmat;
    int fFirst;
    int fSecond;
    int pad1C;
    uint ib;
};
struct SUBDESC
{
    signed char kind;
    signed char cbone;
    short channel;
    float dist;
    void *pv;
    uint ib;
    uint cw;
};
struct SUB
{
    int n;
    XF *pxf;
    MATRIX4 *pmatParent;
    uint grf;
    char *pb;
    uint w;
    BONE *abone;
    SKIN *pskin;
    int pad20;
    qword *ppos;
};
struct SPH
{
    float pos[3];
    float rad;
    char pad[8];
};
struct MODEL
{
    char pad[0x24];
    int cobj;
    OBJ **apobj;
    char pad2C[8];
    uint grfMode;
    int csub;
    SUBDESC *asubdesc;
    SUB *asub;
    SPH *asph;
    char pad48[8];
    float scale;
};
struct DMA
{
    char pad[0xC];
    uchar *pb;
};
struct RPL
{
    int kind;
    float dist;
    uchar *pb;
    char *pv;
};
struct G;
extern "C" { extern DMA D_002DEA28; }
extern "C" { extern G D_003DD490; }
extern "C" { extern MATRIX4 D_002A2D30; }
extern "C" {
void *func_002333B4(void *pv, int b, uint cb) __asm__("memset");
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
void BuildLightPackets(G *pg, int c, void **apv, SHD *pshd);
int ShouldUpdateSaa__FP3SAA(OBJ *pobj);
int GetRenderTargetTexture(ALO *palo, int n);
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb);
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB);
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc);
qword GetLoWorldPos(ALO *palo);
RPL *AllocRenderListEntry(void);
}
static inline MATRIX4 MulMat(const MATRIX4 &matA, const MATRIX4 &matB) return mat
{
    __ml__FRC7MATRIX4T0(&mat, &matA, &matB);
}
static inline void CopyMat(MATRIX4 &matDst, const MATRIX4 &matSrc)
{
    func_0023320C(&matDst, &matSrc, sizeof(MATRIX4));
}
static inline qword TransformPoint(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
static inline void StoreSquaredLengthBits(int &gDst, qword qwV)
{
    qword qwSq, qwOne;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    // the "m"/"r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
    __asm__("qmfc2.ni %0, %1" : "=r,m"(gDst) : "j,!r"(qwSq));
}
static inline int GetSquaredLengthBits(qword qwV)
{
    qword qwSq, qwOne;
    int g;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwSq));
    return g;
}
static inline void CopyQws(uint *pwDst, const uint *pwSrc, int cw)
{
    // empty do-while(0) (as left by a statement macro): its loop notes keep later setup below the tag/pb stores
    do
    {
    } while (0);
    for (int k = 0; k < cw; k += 4)
    {
        uint w0 = pwSrc[0];
        uint w1 = pwSrc[1];
        uint w2 = pwSrc[2];
        uint w3 = pwSrc[3];
        pwSrc += 4;
        pwDst[0] = w0;
        pwDst[1] = w1;
        pwDst[2] = w2;
        pwDst[3] = w3;
        pwDst += 4;
    }
}
template <uint TAG> static inline void UnpackQws(const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = TAG;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
static inline void UnpackQws(uint tag, const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = tag;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
// Builds the VIF packet for every submodel of a model (model matrix, optional bone and skin matrices) and queues it
// with its sort distance. (inferred)
extern "C" void BuildModelDrawPackets(MODEL *pmodel, ALO *palo)
{
    SHD shd;
    MATRIX4 matModel;
    SHD *pshd;
    if (palo->cpshd)
    {
        func_002333B4(&shd, 0, sizeof(SHD));
        pshd = &shd;
        BuildLightPackets(&D_003DD490, palo->cpshd, palo->apshd, &shd);
    }
    else
    {
        pshd = 0;
    }
    int i;
    for (i = 0; i < pmodel->cobj; i++)
    {
        OBJ *pobj = pmodel->apobj[i];
        if (ShouldUpdateSaa__FP3SAA(pobj))
        {
            VTENT *pvte = &pobj->pvt[8];
            pvte->pfn((char *)pobj + pvte->delta);
        }
    }
    for (i = 0; i < pmodel->csub; i++)
    {
        SUBDESC *pdesc = &pmodel->asubdesc[i];
        SUB *psub = &pmodel->asub[i];
        uint grf = psub->grf;
        if (grf & 0x800)
            continue;
        if ((grf & 7) == 2)
            continue;
        int kind = GetRenderTargetTexture(palo, (grf >> 3) & 0x1F);
        if (kind == -1)
            continue;
        if (psub->grf & 0x1000)
        {
            CopyAb__FPvT0Ui(&matModel, psub->pb + 0x14, sizeof(MATRIX4));
        }
        else
        {
            MATRIX4 *pmatParent = psub->pmatParent;
            MATRIX4 mat;
            CopyMat(mat, psub->pxf->localMatrix);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            matModel = mat;
        }
        qword dpos;
        if (psub->ppos)
        {
            qword pos = *psub->ppos;
            dpos = VuSub(pos, GetLoWorldPos(palo));
        }
        else
        {
            VECTOR4 pos;
            CopyAb__FPvT0Ui(&pos, &pmodel->asph[i], 12);
            dpos = TransformPoint(matModel, *(qword *)&pos);
        }
        uchar *pbStart = D_002DEA28.pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->aw[5] | 0x40000000) | ((ulong)(int)pshd->aw[6] << 32);
            D_002DEA28.pb += 0x10;
        }
        ulong *ptag = (ulong *)D_002DEA28.pb;
        *ptag = 0x10000000;
        D_002DEA28.pb += 0x10;
        if (psub->abone)
            UnpackQws<0x6C04C000>((uint *)&D_002A2D30, 16);
        else
            UnpackQws<0x6C04C000>((uint *)&matModel, 16);
        VECTOR4 vecParams;
        vecParams.x = 1.0f;
        vecParams.y = 1.0f;
        vecParams.z = pmodel->scale;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->grfMode : 0;
        UnpackQws<0x6C01C008>((uint *)&vecParams, 4);
        if (psub->abone)
        {
            for (int ibone = 0; ibone < pdesc->cbone; ibone++)
            {
                BONE *pbone = &psub->abone[ibone];
                MATRIX4 mat;
                if (pbone->pxf)
                    mat = MulMat(pbone->pxf->localMatrix, pbone->mat);
                else
                    mat = pbone->mat;
                UnpackQws((ibone * 4 + pdesc->ib) | 0x6C04C000, (uint *)&mat, 16);
            }
        }
        SKIN *pskin = psub->pskin;
        if (pskin)
        {
            MATRIX4 matInv;
            InvertMatrix4Rigid(&matInv, &matModel);
            uint ib = pskin->ib;
            for (int imat = 0; imat < pskin->cmat; imat++)
            {
                if (pskin->fFirst)
                {
                    MATRIX4 mat;
                    MATRIX4 *pmatInv = &matInv;
                    int ibMat = imat * 0x40 + 0x6F0;
                    mat = MulMat(*pmatInv, *(MATRIX4 *)(pskin->pbOwner + ibMat));
                    UnpackQws(ib | 0x6C04C000, (uint *)&mat, 16);
                    ib += 4;
                }
                if (pskin->fSecond)
                {
                    UnpackQws(ib | 0x6C04C000, (uint *)(pskin->pbOwner + 0x7F0 + imat * 0x40), 16);
                    ib += 4;
                }
            }
        }
        {
            DMA *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->pb; (uint)pbPad & 0xF; pbPad = pdma->pb)
            {
                *(uint *)pbPad = 0;
                pdma->pb += 4;
            }
        }
        *(uint *)ptag |= ((int)(D_002DEA28.pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.pb = 0x60000000;
        D_002DEA28.pb += 0x10;
        RPL *prpl = AllocRenderListEntry();
        prpl->kind = kind;
        if (psub->grf & 0x400)
            prpl->dist = pdesc->dist;
        else
            StoreSquaredLengthBits(*(int *)&prpl->dist, dpos);
        prpl->pb = pbStart;
        prpl->pv = psub->pb;
    }
}
} // namespace M_BuildModelDrawPackets

struct SORTREC { int fileVertex; int firstBatch; int firstVertex; int batch; int vertex; };
// qsort comparator for the 0x14-byte vertex records (LoadSubglobType0FromBrx's VERTREC), grouping vertices by batch:
// by the first batch (+4, a pointer compared in 0x40 units), the first vertex, the batch (+0xC, likewise), then the
// vertex.
extern "C" int CompareSortRecs(SORTREC *prec0, SORTREC *prec1)
{
    int d = (prec0->firstBatch - prec1->firstBatch) >> 6;
    if (d != 0)
        return d;
    d = prec0->firstVertex - prec1->firstVertex;
    if (d != 0)
        return d;
    d = (prec0->batch - prec1->batch) >> 6;
    if (d != 0)
        return d;
    return prec0->vertex - prec1->vertex;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobInstancesFromBrx { extern "C" {
struct A { int a[5]; };
struct B { int a[10]; };
struct C { int a[6]; };
void LoadSubglobInstanceFromBrx(void *p, struct A *pa0, struct B *pb0, struct C *pc0, struct A *pa, struct B *pb, struct C *pc, int x, int y, int z, int w);
// For each extra instance slot of a sub-glob, copies the base descriptor and reads the instance record (0x169AC8)
// (inferred)
void LoadSubglobInstancesFromBrx(void *p, struct A *aa, struct B *ab, struct C *ac, int c, int x, int y, int z, int w)
{
    int i;
    for (i = 0; i < c; i++) {
        struct A *pa = &aa[i + 1];
        struct B *pb = &ab[i + 1];
        struct C *pcw = ac ? ac + i + 1 : 0;
        LoadSubglobInstanceFromBrx(p, aa, ab, ac, pa, pb, pcw, x, y, z, w);
    }
}
} } // namespace M_LoadSubglobInstancesFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobInstanceFromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
struct M4{qword rows[4];};
struct D {
    char channel, segments;
    unsigned char flags;
    char kind;
    char pad4[4];
    void *chain;
    char padC[4];
    int words;
};
struct TRANSFORM {
    char pad[0x40];
    M4 matrix;
};
union FLAGS {
    uint word;
    struct {
        uint pad : 11, feature : 1, other : 20;
    } bits;
};
struct E {
    int value;
    TRANSFORM *transform;
    M4 *parent;
    FLAGS flags;
    char *data;
    uint base;
    char pad18[12];
    void *sphere;
};
struct SPHERE {
    float center[3], radius, limit;
    uint metadata;
};
struct SO;
struct VE {
    short delta, pad;
    TRANSFORM *(*fn)(SO *, int);
};
struct SO {
    char pad[0xC];
    VE *vt;
};
struct P{SO*owner;};
struct REQUEST {
    int bytes, alignment;
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
};

struct STREAM;
struct LINK {
    short first, second;
    unsigned char kind, pad;
};
struct LINKS {
    int count;
    LINK *links;
};
struct BINDING {
    int count;
    LINKS *sets;
    int *indices;
};
struct FACE {
    int count, pad4;
    uint flags;
    char padC[8];
    int *indices;
    char pad18[12];
    int first, second, pad2C, kind;
    char pad34[12];
};
typedef char MatrixSize[sizeof(M4)==64?1:-1];
typedef char DescriptorSize[sizeof(D)==20?1:-1];
typedef char EntrySize[sizeof(E)==40?1:-1];
typedef char SphereSize[sizeof(SPHERE)==24?1:-1];
typedef char FaceSize[sizeof(FACE)==64?1:-1];
typedef char LinkSize[sizeof(LINK)==6?1:-1];
typedef char LinkSetSize[sizeof(LINKS)==8?1:-1];
typedef char BindingSize[sizeof(BINDING)==12?1:-1];
typedef char VirtualEntrySize[sizeof(VE)==8?1:-1];
typedef char D_flags_Offset[FIELD_OFFSET(D,flags)==2?1:-1];
typedef char D_kind_Offset[FIELD_OFFSET(D,kind)==3?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char E_transform_Offset[FIELD_OFFSET(E,transform)==4?1:-1];
typedef char E_parent_Offset[FIELD_OFFSET(E,parent)==8?1:-1];
typedef char E_flags_Offset[FIELD_OFFSET(E,flags)==12?1:-1];
typedef char E_data_Offset[FIELD_OFFSET(E,data)==16?1:-1];
typedef char E_base_Offset[FIELD_OFFSET(E,base)==20?1:-1];
typedef char E_sphere_Offset[FIELD_OFFSET(E,sphere)==36?1:-1];
typedef char SPHERE_radius_Offset[FIELD_OFFSET(SPHERE,radius)==12?1:-1];
typedef char SPHERE_limit_Offset[FIELD_OFFSET(SPHERE,limit)==16?1:-1];
typedef char SPHERE_metadata_Offset[FIELD_OFFSET(SPHERE,metadata)==20?1:-1];
typedef char TRANSFORM_matrix_Offset[FIELD_OFFSET(TRANSFORM,matrix)==64?1:-1];
typedef char SO_vt_Offset[FIELD_OFFSET(SO,vt)==12?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char FACE_flags_Offset[FIELD_OFFSET(FACE,flags)==8?1:-1];
typedef char FACE_indices_Offset[FIELD_OFFSET(FACE,indices)==20?1:-1];
typedef char FACE_first_Offset[FIELD_OFFSET(FACE,first)==36?1:-1];
typedef char FACE_second_Offset[FIELD_OFFSET(FACE,second)==40?1:-1];
typedef char FACE_kind_Offset[FIELD_OFFSET(FACE,kind)==48?1:-1];
typedef char LINKS_links_Offset[FIELD_OFFSET(LINKS,links)==4?1:-1];
typedef char BINDING_sets_Offset[FIELD_OFFSET(BINDING,sets)==4?1:-1];
typedef char BINDING_indices_Offset[FIELD_OFFSET(BINDING,indices)==8?1:-1];
typedef char LINK_second_Offset[FIELD_OFFSET(LINK,second)==2?1:-1];
typedef char LINK_kind_Offset[FIELD_OFFSET(LINK,kind)==4?1:-1];
extern "C"{
char S8Read__18CBinaryInputStream(STREAM*) __asm__("S8Read__18CBinaryInputStream");
uint U32Read__18CBinaryInputStream(STREAM*) __asm__("U32Read__18CBinaryInputStream");
short S16Read__18CBinaryInputStream(STREAM*) __asm__("S16Read__18CBinaryInputStream");
int S32Read__18CBinaryInputStream(STREAM*) __asm__("S32Read__18CBinaryInputStream");
void ReadMatrix4__18CBinaryInputStreamP7MATRIX4(STREAM*,M4*);
void*AllocFromWorldHeap(const REQUEST&);
void*AllocCopyFromWorldHeap(const REQUEST&,void*);
STACKSCOPE*STACKSCOPE_ctor(STACKSCOPE*);
void STACKSCOPE_ctor_1AE4A0(STACKSCOPE*,int) __asm__("_$_10STACKSCOPE");
BINDING*LoadClutsetFromBrx(P*,STREAM*);
M4*__ml__FRC7MATRIX4T0(M4*,const M4*,const M4*);
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
}
static inline void *Allocate(int bytes)
{
    return AllocFromWorldHeap(REQUEST(bytes, 16));
}
static inline void *Reallocate(int bytes, void *old)
{
    return AllocCopyFromWorldHeap(REQUEST(bytes, 16), old);
}
// Reads one sub-glob instance: transform owner lookup (vtable slot 51), matrix, flags, optional binding table,
// relocates packet data. (inferred)
extern "C" void LoadSubglobInstanceFromBrx(P *p, const D *sourceD, const E *sourceE, const SPHERE *sourceSphere,
                                           D *desc, E *entry, SPHERE *sphere, int groups, const int *counts,
                                           FACE **faces, STREAM *stream)
{
    M4 matrix;
    (void)S8Read__18CBinaryInputStream(stream);
    *desc = *sourceD;
    *entry = *sourceE;
    if (sphere) {
        *sphere = *sourceSphere;
        sphere->metadata = U32Read__18CBinaryInputStream(stream);
    }
    {
        SO *object = p->owner;
        VE *method = &object->vt[51];
        SO *receiver = (SO *)((char *)object + method->delta);
        int index = S16Read__18CBinaryInputStream(stream);
        entry->transform = method->fn(receiver, index);
    }
    if (entry->flags.word & 0x1000)
        ReadMatrix4__18CBinaryInputStreamP7MATRIX4(stream, &matrix);
    else {
        entry->parent = (M4 *)Allocate(sizeof(M4));
        ReadMatrix4__18CBinaryInputStreamP7MATRIX4(stream, entry->parent);
    }
    entry->value = S32Read__18CBinaryInputStream(stream);
    entry->flags.bits.feature = S8Read__18CBinaryInputStream(stream);
    STACKSCOPE scope;
    BINDING *binding = LoadClutsetFromBrx(p, stream);
    if (!(desc->flags & 1) && desc->kind == 0) {
        char *old = entry->data;
        char *data = (char *)Reallocate(desc->words * 16, old);
        uint delta = (uint)data - (uint)old;
        entry->data = data;
        entry->base += delta;
        if (entry->flags.word & 0x1000) {
            M4 product, temp;
            __ml__FRC7MATRIX4T0(&temp, &entry->transform->matrix, &matrix);
            product = temp;
            CopyAb__FPvT0Ui(entry->data + 0x14, &product, 0x40);
        }
        if (binding) {
            for (int g = 0; g < groups; g++) {
                int count = counts[g];
                FACE *set = faces[g];
                for (int i = 0; i < count; i++) {
                    FACE *face = &set[i];
                    if (face->flags & 0x20) {
                        uint base = (uint)entry->base;
                        unsigned short *first = (unsigned short *)((uint)face->first + base);
                        unsigned short *second = (unsigned short *)((uint)face->second + base);
                        unsigned char *kind = (unsigned char *)((uint)face->kind + base);
                        for (int j = 0; j < face->count; j++) {
                            int index = face->indices[j];
                            LINKS *links = &binding->sets[binding->indices[g]];
                            first[j] = links->links[index].first;
                            second[j] = links->links[index].second;
                            kind[j] = links->links[index].kind;
                        }
                    }
                }
            }
        }
    }
}
} // namespace M_LoadSubglobInstanceFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_CopyRelocateDmaChain { extern "C" {
typedef union QW { unsigned int aun[4]; unsigned short aus[8]; } QW;
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
struct X { int pad[4]; int cqw; };
struct Y { int pad[4]; QW *aqw; char *pb; };
QW *AllocCopyFromWorldHeap(ALLOCREQ *preq, QW *aqw);
// Copies a VIF/DMA packet into the SW heap and optionally relocates DMA REF addresses (tag id 4) by a delta.
// (inferred)
void CopyRelocateDmaChain(struct X *px, struct Y *py, int cqw, QW *aqwSrc, int fReloc, int nNew, int nOld)
{
    ALLOCREQ req;
    QW *aqw;
    req.cb = cqw << 4;
    req.nKind = 0x10;
    px->cqw = cqw;
    aqw = AllocCopyFromWorldHeap(&req, aqwSrc);
    py->aqw = aqw;
    py->pb += (char *)aqw - (char *)aqwSrc;
    if (fReloc) {
        int dn = nNew - nOld;
        QW *pqw = aqw;
        while (pqw < py->aqw + px->cqw) {
            switch ((int)(pqw->aun[0] >> 28)) {
            case 4:
                pqw->aun[1] += dn;
                pqw++;
                break;
            case 1:
                pqw += pqw->aus[0] + 1;
                break;
            case 6:
                pqw = py->aqw + px->cqw;
                break;
            }
        }
    }
}
} } // namespace M_CopyRelocateDmaChain

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType0FromBrx {
// LoadSubglobType0FromBrx = LoadSubglobType0FromBrx (name from Lilly's personal decomp, decomp/src/P2/glob.c T1082 draft). MATCH (T1701).
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
struct RGBA {
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
    void AllocStack(int cqw) __asm__("AllocSw__4DMASi");
    void DetachCopySw(int *pcqw, QW **paqw, QW **paqwCopy) __asm__("DetachCopySw__4DMASPiPP2QWT2");
    void AddDmaCnt() __asm__("AddDmaCnt__4DMAS");
    void AddDmaRefs(int cqw, QW *aqw) __asm__("AddDmaRefs__4DMASiP2QW");
    void AddDmaRet() __asm__("AddDmaRet__4DMAS");
    void EndDmaCnt() __asm__("EndDmaCnt__4DMAS");
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
    VIFS() __asm__("__4VIFS");
    void Align(int iun) __asm__("Align__4VIFSi");
    void AddVifMscal(void *pv) __asm__("AddVifMscal__4VIFSPv");
    void AddVifStcycl(int wl, int cl) __asm__("AddVifStcycl__4VIFSii");
    void AddVifStrow(QW *pqw) __asm__("AddVifStrow__4VIFSP2QW");
    void AddVifStmask(uint unMask) __asm__("AddVifStmask__4VIFSUi");
    void UnpackHelper(UPK upk, int c, int iqw, int *pcb, uint **ppun) __asm__("UnpackHelper__4VIFS3UPKiiPiPPUi");
    void AddVifUnpack(UPK upk, int c, void *pvSrc, int iqw) __asm__("AddVifUnpack__4VIFS3UPKiPvi");
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
void InitSl(SL *psl, int ibSle) __asm__("InitSl");
void AppendSlEntry(SL *psl, void *pv) __asm__("AppendSlEntry");
struct BASIC;
int IsBasicDerivedFrom(BASIC *pbasic, CID cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
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
    void Read(int cb, void *pv) __asm__("Read__18CBinaryInputStreamiPv");
    uchar U8Read() __asm__("U8Read__18CBinaryInputStream");
    char S8Read() __asm__("S8Read__18CBinaryInputStream");
    short S16Read() __asm__("S16Read__18CBinaryInputStream");
    int S32Read() __asm__("S32Read__18CBinaryInputStream");
    float F32Read() __asm__("F32Read__18CBinaryInputStream");
    void ReadVector(VECTOR *pvec) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
    void ReadMatrix4(MATRIX4 *pmat) __asm__("ReadMatrix4__18CBinaryInputStreamP7MATRIX4");
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
    ushort numFrames;          /* +0x06 (low byte: shd.cpp reads the frame count there) */
    SHD *pshd;
    BMP **apbmp;
    CLUT **apclut;
};
struct SHDF
{
      int oid;
      uchar shdk;
      uchar grfshd;
      uchar unknown06;
      uchar unknown07;
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
      XF *transform;
      MATRIX4 *unknown08;
      union
    {
        uint grf;
        struct
        {
            uint grfglobType : 3;
            uint unknownBits0CA : 5;
            uint unknownBits0CB : 2;
            uint fLimit : 1;
            uint isHidden : 1;
            uint fBakedMat : 1;
            uint iUnkFirst : 9;
            uint iUnkEnd : 9;
            uint unknownFlag0CA : 1;
        };
    };
      void *unknown10;
      void *unknown14;
      BND *abnd;
      WRBG *pwrbg;
      struct SAA *psaa;
      struct POSE *ppose;
};
struct SUBGLOB
{
      char unknown00;
      char cbnd;
      int unknown02 : 8;
      char type;
      float unknown04;
      void *pvLit;
      int iqwBone;
      int cqw;
};
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float maxRenderDistance;   /* FLT_MAX in the file: 1e10, or 2000 for cel-border edges (type 1) */
      uint flags;
};
struct POSE
{
      VECTOR pos;
      float radius;           /* +0x0C (LoadPoseSetsFromBrx: position, radius, then the bitfield word) */
      float limit;            /* +0x10 */
      long nKind : 4;
    ulong count : 8;
    ulong channel : 6;
    ulong isHidden : 1;
    long iglob : 13;
      void *link;
      undefined4 pad1C;
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
    float unknown00[3];     /* [1] read as /127, [2] as *6.2e-5 */
    float gPad0c;
    float unknown10[2];
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
      ushort unknown00;
      ushort oid;
      ushort unknown04;
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
    int unknown1C;
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
    int unknown00;
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
    float fogIntensity;     /* BuildCmFgfn's argument (the fog curve uploaded with the geometry) */
    int numLitQwords;       /* size of the lit-vertex DMA chain (set by the type-0 loader) */
    int batchMode;          /* 0 / 1 / 2: batch header flag 0x100 / 0x200 / 0x400 */
};
struct GLOBSET
{
      SO *owner;
      float unknown04;
      undefined4 unknown08[2];
      VECTOR boundsCenter;
      undefined4 unknown1C;
      float boundsRadius;
      int cpsaa;
      SAA **apsaa;
      int unknown2C;
      int *unknown30;
      int unknown34;
      int numPieces;
      SUBGLOB *subMeshes;
      GLOB *pieces;
      SUBGLOBX *asubglobx;
      POSESET *aposeset;
      int unknown4C;
      union
    {
        float unknown50;
        struct
        {
            ulong unknownBits50A : 32;
            ulong unknownBoundsFlag50 : 1;
            ulong unknownBoundsFlag50B : 1;
            ulong ignoreLocked : 1;
            ulong unknownBits50B : 29;
        };
    };
      WRBG *pwrbgFirst;
      void *pbatchrecsFirst;
};
struct ACT;
struct CM;
struct XF
{
      MATRIX4 localMatrix;
      MATRIX4 worldMatrix;
      float segmentStartTime;
      float segmentEndTime;
      float *timeSource;
      float *speedSource;
      ACT *firstActuator;
      int kind;
      short role;
      short phase;
      OID oid;
      XF *parent;
      int worldStamp;
      XF *reference;
      short stiffness;
      short damping;
      VECTOR4 velocity;
      qword restValue;
      qword rate;
      qword unknownE0;
      qword acceleration;
};
extern "C" { extern CM D_002DDF40; }
void *PvAllocSwCopyImpl(struct SWALLOCC *pswalloc, void *pvBase) __asm__("AllocCopyFromWorldHeap");
struct SWALLOCC
{
    int cb;
    int cbAlign;
    SWALLOCC(int cbArg, int cbAlignArg)
    {
        cb = cbArg;
        cbAlign = cbAlignArg;
    }
};
void *PvAllocSwImpl(SWALLOCC *pswalloc) __asm__("AllocFromWorldHeap");
void *PvAllocSwClearImpl(SWALLOCC *pswalloc) __asm__("AllocClearedFromWorldHeap");
void *PvAllocStackClearImpl(SWALLOCC *pswalloc) __asm__("AllocClearedFromStack");
void *PvAllocStackImpl(SWALLOCC *pswalloc) __asm__("AllocFromStack");
extern "C" { void CopyAb(void *pvDst, void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui"); }
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
extern "C" { extern char D_00000000[]; }
extern "C" {
void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType0FromBrx");
}
static inline void *PvAllocSw(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwImpl(&swalloc);
}
static inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwCopyImpl(&swalloc, pvBase);
}
SHD *PshdFromIshd(int ishd) __asm__("GetShdByIndex");
int AddVifUnpackShdp(SHD *pshd, int ishdp, int iqw, VIFS *pvifs) __asm__("AddShaderVifUnpack");
extern "C" { extern uint D_002BE540[]; }
extern "C" { extern int D_002BE558[]; }
extern "C" { extern QW D_002DEB60; }
void CalcRecipFitParams(float gScale, CM *pcm, float *pgOut) __asm__("BuildCmFgfn");
CLUTSET *LoadClutSetFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis) __asm__("LoadClutsetFromBrx");
void LoadSubglobInstancesFromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBX *psubglobx, int cSubExtra, int cbatch, int *acbatch, BATCH **apbatch, CBinaryInputStream *pbis) __asm__("LoadSubglobInstancesFromBrx");
int CompareVertRec(const void *pv1, const void *pv2) __asm__("CompareSortRecs");
void CopyDmaRefsChainSw(SUBGLOB *psubglob, GLOB *pglob, int cqw, void *pvBase, int cqwMain, char *pbNew, char *pbOld) __asm__("CopyRelocateDmaChain");
extern "C" {
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
}
extern "C" void ReadCb12__18CBinaryInputStreamPv(CBinaryInputStream *pbis, void *pv);
extern "C" void ReadCb8__18CBinaryInputStreamPv(CBinaryInputStream *pbis, void *pv);
void FixupRgba(RGBA *prgba) __asm__("ColorCorrectRgba");
extern "C" { 
 }
static inline void *AllocFromStack16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocStackImpl(&swalloc);
}
static inline void *AllocClearedFromStack16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocStackClearImpl(&swalloc);
}
static inline void *AllocClearedFromWorldHeap16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwClearImpl(&swalloc);
}
extern "C" SHD *FindShader(int oid);
extern "C" int AddShaderVifUnpackQword(SHD *pshd, int ishdp, int iqw, VIFS *pvifs);
extern "C" int AddSaaCopyRecord(SAA *psaa, int iqw, VIFS *pvifs);
extern "C" void RelocateShaderUploadRecords(int iFirst, int iEnd, int cb, void *pvOld, void *pvNew);
extern "C" { extern int D_003DA790; }
extern "C" { 
 }
extern "C" int D_002BE580;
extern "C" { extern int D_002DEB54; }
extern "C" const float D_002A52A0;   /* defined below where the ELF has it (.rodata order) */
extern "C" { extern char D_00003628[]; }
extern "C" { extern char D_000037A8[]; }
extern "C" { extern char D_00002EF8[]; }
extern "C" { extern char D_00002E20[]; }
extern "C" { extern char D_00002F80[]; }
extern "C" { extern char D_00002E68[]; }
extern "C" { extern char D_00002FC8[]; }
extern "C" { extern char D_00002EB0[]; }
extern "C" { extern char D_00003358[]; }
extern "C" { extern char D_00002EE8[]; }
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
static inline char *RelocatePointer(char *pbOld, int cb, char *pbNew, char *pb)
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
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A52A0 __asm__("D_002A52A0") = 3.40282347e+38f; }
// Sub-glob type 0 loader: textured lit triangle strips (the visible mesh); builds VIF batches.
void LoadSubglobType0FromBrx(GLOBSET *const pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis)
{
    psubglob->unknown02 = pbis->U8Read();
    int isaa = pbis->S16Read();
    if (isaa >= 0)
        pglob->psaa = pglobset->apsaa[isaa];
    if (psubglob->unknown02 & 1)
        pglob->grf |= 0x100;
    if (psubglob->unknown02 & 2)
        pglob->grf |= 0x200;

    STACKSCOPE scopeOuter;
    VIFS vifs;
    VIFS vifsAux;
    vifs.AllocStack(0x2000);
    QW *pqwChain = (QW *)vifs.m_pb;
    pglob->iUnkFirst = D_003DA790;
    if (psubglob->unknown02 & 1)
    {
        vifs.AddDmaCnt();
        pglobset->unknownBoundsFlag50 = 1;
    }
    else
    {
        vifsAux.AllocStack(0x800);
        if (IsBasicDerivedFrom((BASIC *)pglobset->owner, (CID)0x31) && !(psubglob->unknown02 & 4) && (uchar)(psubglob->unknown00 - 7) >= 2 /* not 7 or 8: the unsigned range test is what compiles to the target */ &&
            psubglob->unknown04 == D_002A52A0 && g_sw.fUnk3e40 == 0)
        {
            QW qwOne;
            qwOne.ag[0] = 1.0f;
            qwOne.ag[1] = 1.0f;
            qwOne.ag[2] = 0.0f;
            qwOne.ag[3] = 0.0f;
            pglob->grf |= 0x1000;
            vifsAux.AddDmaCnt();
            vifsAux.AddVifUnpack(UPK_V4_32, 4, &pglob->transform->worldMatrix, 0);
            vifsAux.AddVifUnpack(UPK_V4_32, 1, &qwOne, 8);
            vifsAux.EndDmaCnt();
        }
    }
    float *pgFit;
    vifs.UnpackHelper(UPK_V4_32, 1, 9, 0, (uint **)&pgFit);
    CalcRecipFitParams(psgl->fogIntensity, &D_002DDF40, pgFit);

    psubglob->cbnd = pbis->U8Read();
    pglob->abnd = (BND *)AllocClearedFromWorldHeap16(psubglob->cbnd * sizeof(BND));
    for (int ibnd = 0; ibnd < psubglob->cbnd; ibnd++)
    {
        BND *pbnd = &pglob->abnd[ibnd];
        pbnd->oid = (OID)pbis->S32Read();
        pbnd->ixf = pbis->S16Read();
    }
    int iqwBase = (psubglob->unknown02 & 1) ? 0x21 : 0x15;
    if (psubglob->cbnd != 0)
    {
        psubglob->iqwBone = iqwBase;
        iqwBase += psubglob->cbnd * 4;
    }
    SPECPARAMS spec;
    spec.iSpec = pbis->S8Read();
    if (spec.iSpec != -1)
    {
        ReadCb12__18CBinaryInputStreamPv(pbis, &spec.unknown00);
        ReadCb8__18CBinaryInputStreamPv(pbis, &spec.unknown10);
        pbis->ReadVector(&spec.vec);
        spec.unknown00[1] *= 0.007874016f;
        spec.unknown00[2] *= 6.200012e-05f;
        spec.fMode = psubglob->unknown02 & 1;
    }
    CLUTSET *pclutset = LoadClutSetFromBrx(pglobset, pbis);
    if (psubglob->unknown02 & 4)
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
        pwrbg->pso = pglobset->owner;
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
    if (!(psubglob->unknown02 & 1))
    {
        vifsLit.AllocStack(0x2000);
        vifsLit.AddDmaCnt();
    }
    int *acbatch = (int *)AllocClearedFromStack16(cbatch * 4);
    BATCH **apbatch = (BATCH **)AllocClearedFromStack16(cbatch * 4);
    void *apvLit[1024];
    for (int ibatch = 0; ibatch < cbatch; ibatch++)
    {
        int cbatchThis = pbis->U8Read();
        BATCH *abatch = (BATCH *)AllocClearedFromStack16(cbatchThis << 6);
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
            switch (psgl->batchMode)
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
        if (psubglob->unknown02 & 1)
            phdr->grf.grf |= 2;
        if (psubglob->unknown02 & 0x10)
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
                pbatch->aivtx = (int *)AllocClearedFromStack16(pbatch->cvtx << 2);
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
        VECTOR *aposFile = (VECTOR *)AllocFromStack16(cvtxFile * sizeof(VECTOR));
        pbis->Read(cvtxFile * sizeof(VECTOR), aposFile);
        UVF *auvFile = (UVF *)AllocFromStack16(cvtxFile * sizeof(UVF));
        pbis->Read(cvtxFile * sizeof(UVF), auvFile);
        VECTOR *anormFile = (VECTOR *)AllocFromStack16(cvtxFile * sizeof(VECTOR));
        pbis->Read(cvtxFile * sizeof(VECTOR), anormFile);
        RGBA *argbaFile = (RGBA *)AllocFromStack16(cvtxFile * sizeof(RGBA));
        pbis->Read(cvtxFile * sizeof(RGBA), argbaFile);
        for (RGBA *prgba = argbaFile; prgba < argbaFile + cvtxFile; prgba++)
            FixupRgba(prgba);
        int fAlpha = pbis->S8Read();
        if (psubglob->unknown00 < 9)
            fAlpha = (psubglob->unknown00 < 3) ? fAlpha : 1;
        void *pvWeights;
        if (psubglob->cbnd != 0)
        {
            for (int iwt = 0; iwt < 4; iwt++)
                phdr->aiqwBone[iwt] = psubglob->iqwBone + pbis->U8Read() * 4;
            pvWeights = AllocFromStack16(CwtFromSubglob(psubglob) * cvtxFile);
            pbis->Read(CwtFromSubglob(psubglob) * cvtxFile, pvWeights);
        }
        else
        {
            pvWeights = 0;
        }
        uchar *pbVtx = (uchar *)AllocFromStack16(cvtxTotal);
        uint *punBits;
        uint un = 1;
        pbis->Read(cvtxTotal, pbVtx);
        int cbBits = ((cvtxTotal + 31) / 32) * 4;
        int fMulti = 0;
        uint *aunBits = (uint *)AllocFromStack16(cbBits);
        pbis->Read(cbBits, aunBits);
        punBits = aunBits;
        if ((psubglob->unknown02 & 5) || psubglob->cbnd != 0 || spec.iSpec != -1)
            fMulti = 1;
        int fNoSaa = !fMulti && abatch[0].psaa == 0 && abatch[1].psaa == 0;
        int iqw = iqwBase;
        phdr->iqwBase = iqw;
        VERTREC *arec;
        if (psubglob->cbnd == 0 && !(psubglob->unknown02 & 1) && spec.iSpec == -1)
        {
            arec = 0;
        }
        else
        {
            if (psubglob->unknown02 & 8)
            {
                uchar *pbV = pbVtx;
                arec = (VERTREC *)AllocFromStack16(cvtxTotal * sizeof(VERTREC));
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
                VERTREC **apvtx = (VERTREC **)AllocClearedFromStack16(cvtxFile << 2);
                arec = (VERTREC *)AllocFromStack16(cvtxTotal * sizeof(VERTREC));
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
                    if (psubglob->unknown02 & 2)
                    {
                        int ioid = pglobset->unknown2C;
                        while (--ioid >= 0)
                            iqw += AddShaderVifUnpackQword(FindShader(pglobset->unknown30[ioid]), ishdp, iqw, &vifs);
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
                    iqw += AddSaaCopyRecord(pbatch->psaa, iqw, &vifs);
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
                if (psubglob->unknown02 & 1)
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
                if (psubglob->unknown02 & 1)
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
            if ((psubglob->unknown02 & 1) || psubglob->cbnd != 0 || spec.iSpec != -1)
                vifs.UnpackHelper((UPK)0x19, pbatch->cvtx, ppart->iqwVtx, 0, (uint **)&pbatch->pvUvLink);
            else
                vifs.UnpackHelper((UPK)0x15, pbatch->cvtx, ppart->iqwVtx, 0, (uint **)&pbatch->pvUv);
            if (psubglob->unknown02 & 1)
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
                WRBGBATCH *pwb = (WRBGBATCH *)AllocClearedFromWorldHeap16(sizeof(WRBGBATCH));
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
            if (!(psubglob->unknown02 & 1))
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
                            int n = MinInt(0x27, pbatch->cvtx - iofs);
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
                    if (psubglob->unknown00 == 0xb || psubglob->unknown00 == 0xd)
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
                else if (psubglob->unknown00 == 0xb || psubglob->unknown00 == 0xd)
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
            if (psubglob->unknown00 == 0xb || psubglob->unknown00 == 0xd)
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
    if (psubglob->unknown02 & 1)
    {
        vifs.EndDmaCnt();
        vifs.AddDmaRet();
        vifs.DetachCopySw(&cqwMain, (QW **)&pbOld, (QW **)&pbNew);
        psubglob->cqw = cqwMain;
        pglob->unknown10 = pbNew;
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
            psgl->numLitQwords = cqwLit;
            psubglob->pvLit = PvAllocSwCopy(cqwLit << 4, vifsLit.m_ab);
        }
    }
    if (pclutset != 0)
        pglobset->unknown4C = g_sw.nUnk3d5c;
    pglob->iUnkEnd = D_003DA790;
    RelocateShaderUploadRecords(pglob->iUnkFirst, pglob->iUnkEnd, cqwMain << 4, pbOld, pbNew);
    if (pglob->pwrbg != 0)
    {
        for (WRBGBATCH *pwb = (WRBGBATCH *)pglob->pwrbg->slBatch.head; pwb != 0; pwb = pwb->pwrbgbatchNext)
        {
            RelocPpv(pbOld, cqwMain << 4, pbNew, (char **)&pwb->pvPos);
            for (int imat = 0; imat < pglob->pwrbg->cmat; imat++)
                RelocPpv(pbOld, cqwMain << 4, pbNew, (char **)&pwb->apvWarp[imat]);
        }
    }
    if (psubglob->unknown02 & 8)
    {
        int cbatchAll = 0;
        for (int i = 0; i < cbatch; i++)
            cbatchAll += acbatch[i];
        BATCHRECS *pbatchrecs = (BATCHRECS *)AllocClearedFromWorldHeap16(sizeof(BATCHRECS));
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
                pbr->pvPos = RelocatePointer(pbOld, cqwMain << 4, pbNew, (char *)pb->pvPos);
                pbr->pvUv = RelocatePointer(pbOld, cqwMain << 4, pbNew, (char *)pb->pvUv);
                pbr->pvUvLink = RelocatePointer(pbOld, cqwMain << 4, pbNew, (char *)pb->pvUvLink);
                pbr->pbRgb = RelocatePointer(pbOld, cqwMain << 4, pbNew, (char *)pb->pbRgb);
                pbr->pbNorm = RelocatePointer(pbOld, cqwMain << 4, pbNew, (char *)pb->pbNorm);
                pbr->unknown1C = 0;
                pb++;
                pbr++;
            }
        }
    }
    LoadSubglobInstancesFromBrx(pglobset, psubglob, pglob, psubglobx, cSubExtra, cbatch, acbatch, apbatch, pbis);
}
} // namespace M_LoadSubglobType0FromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadClutsetFromBrx {
struct ALLOCREQ { int cb; int nKind; };
class CBinaryInputStream {
public:
    int U8Read() __asm__("U8Read__18CBinaryInputStream");
    int U16Read() __asm__("U16Read__18CBinaryInputStream");
    int S16Read() __asm__("S16Read__18CBinaryInputStream");
};
struct E6 { short a; short b; char c; char pad; };
struct E8 { int c; E6 *a; };
struct T { int c; E8 *a; int *ai; };
extern "C" void *AllocClearedFromStack(ALLOCREQ *preq);
extern "C" void ColorCorrectRgba16(short *ps);
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return AllocClearedFromStack(preq);
}
// Reads a u16-counted table of vertex-binding groups (u16/u16/u8 records) and fixes up indices (0x1ED1A8) (inferred)
extern "C" T *LoadClutsetFromBrx(void *pv, CBinaryInputStream *pbis)
{
    T *pt = 0;
    int c = pbis->U16Read();
    if (c) {
        ALLOCREQ req;
        pt = (T *)PvAllocReq(&req, sizeof(T), 0x10);
        pt->c = c;
        pt->a = (E8 *)PvAllocReq(&req, c * sizeof(E8), 0x10);
        for (int i = 0; i < pt->c; i++) {
            E8 *pe = &pt->a[i];
            int n = pbis->U16Read();
            pe->a = (E6 *)PvAllocReq(&req, n * sizeof(E6), 0x10);
            for (int j = 0; j < n; j++) {
                E6 *p6 = &pe->a[j];
                short *pb = &p6->b;
                p6->a = pbis->U16Read();
                p6->b = pbis->U16Read();
                p6->c = pbis->U8Read();
                ColorCorrectRgba16(&p6->a);
                ColorCorrectRgba16(pb);
            }
        }
        int m = pbis->U16Read();
        pt->ai = (int *)PvAllocReq(&req, m * 4, 0x10);
        for (int k = 0; k < m; k++)
            pt->ai[k] = pbis->S16Read();
    }
    return pt;
}
} // namespace M_LoadClutsetFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType2FromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
struct OWNER {
    char pad[0x50];
    ulong flags;
};
struct D {
    char kind, count;
    ushort flags;
    void *pad4;
    QW *chain;
    int base, words;
};
struct BONE {
    int value;
    char pad4[4];
    int index;
    char padC[0x44];
};
struct E {
    char pad[0x10];
    QW *chain;
    int pad14;
    BONE *bones;
};
struct SPHERE;struct FACE;
struct OBJECT {
    char pad[0x1C];
    int count;
    void *segments;
    char pad24[8];
};
struct HEADER {
    ushort flags, base, pad4, pad6, position, weights, color, end, extra, extra2;
    ushort segments[2], bones[4];
};
struct REQUEST {
    int bytes, alignment;
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
};
extern "C"{
void*STACKSCOPE_ctor(void*);
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&);
OBJECT*GetShdByIndex(int);
int AddShaderVifUnpack(OBJECT*,int,int,VIFS*);
void LoadSubglobInstancesFromBrx(OWNER*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
void*memset(void*,int,uint);
extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00001910[],D_00001A68[];
}
struct LEASE {
    LEASE() { STACKSCOPE_ctor(this); }
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
static inline void *Allocate(int bytes)
{
    return AllocClearedFromWorldHeap(REQUEST(bytes, 16));
}
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char OBJECT_Size[sizeof(OBJECT)==44?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char OWNER_flags_Offset[FIELD_OFFSET(OWNER,flags)==80?1:-1];
typedef char D_count_Offset[FIELD_OFFSET(D,count)==1?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_base_Offset[FIELD_OFFSET(D,base)==12?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char BONE_value_Offset[FIELD_OFFSET(BONE,value)==0?1:-1];
typedef char BONE_index_Offset[FIELD_OFFSET(BONE,index)==8?1:-1];
typedef char E_chain_Offset[FIELD_OFFSET(E,chain)==16?1:-1];
typedef char E_bones_Offset[FIELD_OFFSET(E,bones)==24?1:-1];
typedef char OBJECT_count_Offset[FIELD_OFFSET(OBJECT,count)==28?1:-1];
typedef char OBJECT_segments_Offset[FIELD_OFFSET(OBJECT,segments)==32?1:-1];
typedef char HEADER_flags_Offset[FIELD_OFFSET(HEADER,flags)==0?1:-1];
typedef char HEADER_base_Offset[FIELD_OFFSET(HEADER,base)==2?1:-1];
typedef char HEADER_position_Offset[FIELD_OFFSET(HEADER,position)==8?1:-1];
typedef char HEADER_weights_Offset[FIELD_OFFSET(HEADER,weights)==10?1:-1];
typedef char HEADER_color_Offset[FIELD_OFFSET(HEADER,color)==12?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==14?1:-1];
typedef char HEADER_extra_Offset[FIELD_OFFSET(HEADER,extra)==16?1:-1];
typedef char HEADER_extra2_Offset[FIELD_OFFSET(HEADER,extra2)==18?1:-1];
typedef char HEADER_segments_Offset[FIELD_OFFSET(HEADER,segments)==20?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==24?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
// Sub-glob type 2 loader: U16 shader + vertices + colours; HYPOTHESIS cel/outline.
extern "C" void LoadSubglobType2FromBrx(OWNER *p, D *desc, E *entry, void *, SPHERE *sphere, int groups,
                                        CBinaryInputStream *stream)
{
    int i = 0;
    p->flags |= 1UL << 33;
    VIFS vifs;
    STACKSCOPE outer;
    vifs.AllocSw(0x2000);
    vifs.AddDmaCnt();
    int count = stream->U8Read();
    desc->count = count;
    entry->bones = (BONE *)Allocate((char)count * sizeof(BONE));
    for (; i < desc->count; i++) {
        BONE *bone = &entry->bones[i];
        bone->value = stream->S32Read();
        bone->index = stream->S16Read();
    }
    int base = 0x1B;
    if (desc->count) {
        desc->base = base;
        base += desc->count * 4;
    }
    int subCount = stream->S16Read();
    QW *data;
    for (int part = 0; part < subCount; part++) {
        LEASE inner;
        ushort *header;
        vifs.UnpackHelper(UPK_V4_16, 4, 10, 0, (uint **)&header);
        memset(header, 0, 32);
        switch (part) {
        case 0:
            header[0] |= 1;
            if (subCount == 1)
                header[0] |= 8;
            break;
        case 1:
            header[0] |= 2;
            break;
        }
        if (desc->count)
            header[0] |= 4;
        int current = base;
        header[1] = current;
        OBJECT *object = GetShdByIndex(stream->U16Read());
        for (int segment = 0; segment < object->count; segment++) {
            HEADER *slot = (HEADER *)((uint)header + segment * 2);
            slot->segments[0] = current;
            current += AddShaderVifUnpack(object, segment, current, &vifs);
        }
        int vertices = stream->U8Read();
        vifs.AddVifStcycl(1, 2);
        vifs.AddVifUnpack(UPK_V3_32, vertices, stream, current + 1);
        header[4] = current;
        current += vertices * 2;
        header[5] = current;
        if (desc->count) {
            int bones = stream->U8Read();
            int k = 0;
            for (; k < bones; k++) {
                int bone = stream->U8Read();
                HEADER *slot = (HEADER *)((uint)header + k * 2);
                slot->bones[0] = (ushort)desc->base + bone * 4;
            }
            int small = bones < 4;
            for (; k < 4; k++) {
                HEADER *slot = (HEADER *)((uint)header + k * 2);
                slot->bones[0] = header[12];
            }
            if (small) {
                vifs.AddVifStrow(&D_002DEB60);
                vifs.AddVifStmask(D_002BE540[bones]);
            }
            vifs.AddVifUnpack(D_002BE558[bones], vertices, stream, header[4]);
            if (small)
                vifs.AddVifStmask(0);
        }
        vifs.AddVifStcycl(1, 1);
        int colors = stream->U8Read();
        vifs.AddVifUnpack(UPK_V4_8, colors, stream, current);
        header[6] = current;
        current += colors;
        header[7] = current;
        current++;
        header[8] = header[9] = current;
        vifs.AddVifMscal(D_00000000);
        vifs.AddVifMscal(D_00001910);
        vifs.AddVifMscal(D_00001A68);
        inner.Release();
    }
    vifs.EndDmaCnt();
    vifs.AddDmaRet();
    vifs.DetachCopySw(&desc->words, &data, &entry->chain);
    LoadSubglobInstancesFromBrx(p, desc, entry, sphere, groups, 0, 0, 0, stream);
}
} // namespace M_LoadSubglobType2FromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType3FromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
union U {
    qword q;
    float f[4];
};
struct P;struct SPHERE;struct FACE;
struct PARAM {
    float value;
    int pad, kind;
};
struct D {
    char channel, count;
    ushort flags;
    float radius;
    QW *chain;
    int base, words;
};
struct BONE {
    int value;
    char pad4[4];
    int index;
    char padC[0x44];
};
struct E {
    char pad[0x10];
    QW *chain;
    int base;
    BONE *bones;
};
struct OBJECT {
    char pad[0x1C];
    int count;
    void *segments;
};
struct HEADER {
    ushort flags, base, vertices, pad6, pad8, padA, padC, padE, weight, end, segments[2], bones[4];
};
struct REQUEST {
    int bytes, alignment;
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
    void *Allocate();
    void *AllocateZero();
};
extern "C"{
void*STACKSCOPE_ctor(void*);
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&);
void*AllocFromStack(const REQUEST&);
void*func_002333B4(void*,int,uint) __asm__("memset");
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
OBJECT*GetShdByIndex(int);
int AddShaderVifUnpack(OBJECT*,int,int,VIFS*);
void LoadSubglobInstancesFromBrx(P*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
/* GetVectorComponentX/E0/E8: the file-scope inline readers */


extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00001910[],D_00001A68[];
}
struct MARK {
    MARK() { STACKSCOPE_ctor(this); }
    ~MARK() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
struct LEASE {
    LEASE() { STACKSCOPE_ctor(this); }
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
inline void *REQUEST::Allocate()
{
    return AllocFromStack(*this);
}
inline void *REQUEST::AllocateZero()
{
    return AllocClearedFromWorldHeap(*this);
}
static inline void *Allocate(int bytes)
{
    return REQUEST(bytes, 16).Allocate();
}
static inline void *AllocateZero(int bytes)
{
    return REQUEST(bytes, 16).AllocateZero();
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char PARAM_Size[sizeof(PARAM)==12?1:-1];
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char MARK_Size[sizeof(MARK)==1?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
typedef char PARAM_value_Offset[FIELD_OFFSET(PARAM,value)==0?1:-1];
typedef char PARAM_kind_Offset[FIELD_OFFSET(PARAM,kind)==8?1:-1];
typedef char D_count_Offset[FIELD_OFFSET(D,count)==1?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_base_Offset[FIELD_OFFSET(D,base)==12?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char BONE_value_Offset[FIELD_OFFSET(BONE,value)==0?1:-1];
typedef char BONE_index_Offset[FIELD_OFFSET(BONE,index)==8?1:-1];
typedef char E_chain_Offset[FIELD_OFFSET(E,chain)==16?1:-1];
typedef char E_bones_Offset[FIELD_OFFSET(E,bones)==24?1:-1];
typedef char OBJECT_count_Offset[FIELD_OFFSET(OBJECT,count)==28?1:-1];
typedef char OBJECT_segments_Offset[FIELD_OFFSET(OBJECT,segments)==32?1:-1];
typedef char HEADER_flags_Offset[FIELD_OFFSET(HEADER,flags)==0?1:-1];
typedef char HEADER_base_Offset[FIELD_OFFSET(HEADER,base)==2?1:-1];
typedef char HEADER_vertices_Offset[FIELD_OFFSET(HEADER,vertices)==4?1:-1];
typedef char HEADER_weight_Offset[FIELD_OFFSET(HEADER,weight)==16?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==18?1:-1];
typedef char HEADER_segments_Offset[FIELD_OFFSET(HEADER,segments)==20?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==24?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
// empty asms: hide the flag constants from cse/loop.c so each flag store keeps its own short-lived constant
// (case 0: param form; case 1: volatile, header read first, int result keeps the gcse table size of the game)
static inline ushort Flags(ushort v)
{
    __asm__("" : "+r"(v));
    return v;
}
template <int V> static inline int Flags()
{
    int v = V;
    __asm__ __volatile__("" : "+r"(v));
    return v;
}
static inline void WriteVertex(U *destination, U value)
{
    float x = ::GetVectorComponentX((VECTOR *)&value);
    float y = ::GetVectorComponentY((VECTOR *)&value);
    float z = ::GetVectorComponentZ((VECTOR *)&value);
    destination->f[0] = x;
    destination->f[1] = y;
    destination->f[2] = z;
}
// Sub-glob type 3 loader: U16 shader + indexed vectors + strip bits; HYPOTHESIS cel edge strips.
extern "C" void LoadSubglobType3FromBrx(P *p, D *desc, E *entry, PARAM *param, SPHERE *sphere, int groups,
                                        CBinaryInputStream *stream)
{
    register CBinaryInputStream *input asm("$23") =
        stream; // pin: the game keeps the stream in s7 for the whole function
    int i = 0;
    int vertex;
    int index;
    VIFS vifs;
    MARK outer;
    vifs.AllocSw(0x2000);
    vifs.AddDmaCnt();
    int count = input->U8Read();
    desc->count = count;
    entry->bones = (BONE *)AllocateZero((char)count * sizeof(BONE));
    for (; i < desc->count; i++) {
        BONE *bone = &entry->bones[i];
        bone->value = input->S32Read();
        bone->index = input->S16Read();
    }
    int base = 0x1B;
    if (desc->count) {
        desc->base = base;
        base += desc->count * 4;
    }
    int subCount = input->S16Read();
    QW *data;
    for (int part = 0; part < subCount;) {
        LEASE inner;
        int vertices;
        U *vectors;
        HEADER *header;
        char *weights;
        U *positions;
        vifs.UnpackHelper(UPK_V4_16, 4, 10, 0, (uint **)&header);
        func_002333B4(header, 0, 32);
        switch (part) {
        case 0:
            header->flags = Flags(1);
            if (subCount == 1)
                header->flags |= 8;
            if (param->kind != 2)
                break;
            header->flags |= 0x400;
            break;
        case 1: {
            HEADER *h = header;
            h->flags = Flags<2>();
        } break;
        }
        header->base = base;
        OBJECT *object = GetShdByIndex(input->U16Read());
        vertices = input->U8Read();
        int vectorCount = input->U8Read();
        vectors = (U *)AllocFromStack(REQUEST(vectorCount * sizeof(U), 16));
        if (vectorCount > 0) {
            U *cursor = vectors;
            index = vectorCount;
            do {
                input->ReadVector((VECTOR *)cursor);
                --index;
                ++cursor;
            } while (index);
        }
        char *boneWeights;
        int bones = desc->count < 4 ? desc->count : 4;
        if (desc->count) {
            for (int b = 0; b < 4; b++) {
                int bone = input->U8Read();
                HEADER *slot = (HEADER *)((uint)header + b * 2);
                slot->bones[0] = (ushort)desc->base + bone * 4;
            }
            int bytes = bones * vectorCount;
            boneWeights = (char *)Allocate(bytes);
            input->Read(bytes, boneWeights);
        } else
            boneWeights = 0;
        byte *indices = (byte *)Allocate(vertices);
        input->Read(vertices, indices);
        int flagBytes = ((vertices + 31) / 32) * 4;
        uint *flags = (uint *)Allocate(flagBytes);
        input->Read(flagBytes, flags);
        uint *flagCursor = flags;
        uint mask = 1;
        int current = base;
        for (int segment = 0; segment < object->count; segment++) {
            HEADER *slot = (HEADER *)((uint)header + segment * 2);
            slot->segments[0] = current;
            current += AddShaderVifUnpack(object, segment, current, &vifs);
        }
        ++current;
        header->weight = current;
        current += 2 * vertices;
        header->end = current;
        header->vertices = vertices;
        vifs.AddVifStcycl(1, 2);
        if (desc->count) {
            int small = bones < 4;
            if (small) {
                vifs.AddVifStrow(&D_002DEB60);
                vifs.AddVifStmask(D_002BE540[bones]);
            }
            vifs.UnpackHelper(D_002BE558[bones], vertices, header->weight, 0, (uint **)&weights);
            if (small)
                vifs.AddVifStmask(0);
        }
        vifs.UnpackHelper(UPK_V4_32, vertices, header->weight + 1, 0, (uint **)&positions);
        vifs.AddVifStcycl(1, 1);
        for (vertex = 0; vertex < vertices; vertex++) {
            index = indices[vertex];
            WriteVertex(&positions[vertex], vectors[index]);
            if (*flagCursor & mask)
                positions[vertex].f[3] = 8421376.0f;
            else
                positions[vertex].f[3] = 8417280.0f;
            mask <<= 1;
            if (!mask) {
                ++flagCursor;
                mask = 1;
            }
        }
        if (desc->count) {
            for (vertex = 0; vertex < vertices; vertex++) {
                index = indices[vertex];
                CopyAb__FPvT0Ui(weights + vertex * bones, boneWeights + index * bones, bones);
            }
        }
        vifs.AddVifMscal(D_00000000);
        part++;
        vifs.AddVifMscal(D_00001910);
        vifs.AddVifMscal(D_00001A68);
        inner.Release();
    }
    vifs.EndDmaCnt();
    vifs.AddDmaRet();
    vifs.DetachCopySw(&desc->words, &data, &entry->chain);
    LoadSubglobInstancesFromBrx(p, desc, entry, sphere, groups, 0, 0, 0, input);
}
} // namespace M_LoadSubglobType3FromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddFramePatch { extern "C" {
struct D { int a; char *b; int c; char *d; };
struct H { short a; short b; };
// Records a (framebuffer word index, qword offset+0x23) patch for the VU1 microprogram, which fills FRAME/TEX0 of the
// current buffer from VU qw 0xB/0xC. (inferred)
void AddFramePatch(int n, struct D *pd, struct H **pph)
{
    struct H *ph = (*pph)++;
    ph->a = n;
    ph->b = ((pd->d - pd->b) >> 4) + 0x23;
}
} } // namespace M_AddFramePatch

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BuildProjectedVolumeGifs {
extern "C" void AddFramePatch(int n, GIFS *pgifs, short **pps);
// Builds one projected-volume stencil/apply GIF prim (FRAME_2 mask, TEX0_2, TEX1, ALPHA...) selected by grf bits.
// (inferred)
extern "C" void BuildProjectedVolumeGifs(GIFS *pgifs, uint grf, short **pps)
{
    pgifs->AddPrimPack(0x200, 1, 0xE);
    AddFramePatch((grf & 4) ? 4 : 5, pgifs, pps);
    if (grf & 0x200) {
        pgifs->PackAD(0x4D, 0x3FFF02000000L);
    } else if (grf & 0x100) {
        pgifs->PackAD(0x4D, 0xFFFFC00002000000L);
    } else {
        uint msk = (grf & 0x800000) ? 0x7FFFFFFF : 0xFFFFFFFF;
        if (grf & 0x400000) msk &= 0xFF7FFFFF;
        if (grf & 0x200000) msk &= 0xFFBFFFFF;
        if (grf & 0x100000) msk &= 0xFFDFFFFF;
        if (grf & 0x80000) msk &= 0xFFEFFFFF;
        if (grf & 0x40000) msk &= 0xFFF0FFFF;
        if (grf & 0x20000) msk &= 0xFFFF7FFF;
        if (grf & 0x10000) msk &= 0xFFFFBFFF;
        pgifs->PackAD(0x4D, ((ulong)msk << 32) | 0x2000000);
    }
    AddFramePatch((grf & 1) ? 6 : 7, pgifs, pps);
    pgifs->PackAD(7, 0xEA8200000L);
    pgifs->PackAD(0x15, 1);
    pgifs->PackAD(0x3F, 0);
    pgifs->PackAD(0x45, 0);
    pgifs->PackAD(0x19, 0);
    pgifs->PackAD(0x41, 0x1C0000002000000L);
    pgifs->PackAD(0x48, 0x31001);
    pgifs->PackAD(0x43, 0x2A);
    pgifs->PackAD(0x3B, 0x8000000000L);
    pgifs->PackAD(1, 0x180808080L);
    pgifs->PackAD(0, 0x356);
    pgifs->AddPrimList(4, 0x5353);
    int u = (grf & 0x10) ? 0 : 8;
    for (int x = (grf & 0x40) ? 0 : 8; x < 0x200; x += 0x10, u += 0x10) {
        pgifs->ListUV((u << 4) + 8, 8);
        pgifs->ListXYZ(x << 4, 0, 0);
        pgifs->ListUV(((u + 8) << 4) + 8, 0x1C08);
        pgifs->ListXYZ((x + 8) << 4, 0x1C00, 0);
    }
    pgifs->AddPrimPack(0x200, 1, 0xE);
    pgifs->PackAD(0x41, 0xE0000002000000L);
}
} // namespace M_BuildProjectedVolumeGifs

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpProjVolume {
extern "C" { extern char D_0025C1C0[]; }
extern "C" { extern char D_0025E040[]; }
extern "C" { extern char D_00001AA0[]; }
extern "C" { 
 }
extern "C" { extern QW D_002DF800[]; }
extern "C" { extern int D_002DEB70[2]; }
extern "C" void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" void AddFramePatch(int n, GIFS *pgifs, short **pps);
extern "C" void BuildProjectedVolumeGifs(GIFS *pgifs, uint grf, short **pps);
// Pass 6/7 begin (kinds 7/8): upload projected-volume microprogram D_0025C1C0..D_0025E040, build/kick (MSCAL 0x1AA0)
// the masked full-screen stencil-init strips. (inferred)
extern "C" void BeginRpProjVolume(VIFS *pvifs, int n1, int n2)
{
    EnsureVu1Code__FP4VIFSPvT1(pvifs, D_0025C1C0, D_0025E040);
    pvifs->AddDmaCnt();
    if (n2 != 7 && g_sw.fUnk3e38) {
        pvifs->AddVifFlush();
        pvifs->AddDmaCall(D_002DF800);
        pvifs->AddVifBaseOffset(0x32, 0x1E7);
    }
    uint *pun;
    pvifs->UnpackHelper(UPK_V2_16, 5, 0x1B, 0, &pun);
    short *ps = (short *)pun;
    GIFS gifs;
    QW aqw[0x100];
    gifs.AllocStatic(0x100, aqw);
    if (n2 != 7)
        BuildProjectedVolumeGifs(&gifs, 0x299, &ps);
    gifs.AddPrimPack(0, 1, 0xE);
    AddFramePatch(0, &gifs, &ps);
    if (n1 == 7 || n2 != 7)
        gifs.PackAD(0x4C, 0xFFFF00FF00000000L);
    else
        gifs.PackAD(0x4C, 0xFFFF80FF00000000L);
    gifs.PackAD(0x46, 0);
    gifs.PackAD(0x47, 0x31001);
    if (n1 == 7)
        gifs.PackAD(1, 0x100007E00L);
    else
        gifs.PackAD(1, 0x100003E00L);
    gifs.PackAD(0, 6);
    gifs.AddPrimList(2, 0x55);
    for (int x = 0; x < 0x200; x += 0x20) {
        gifs.ListXYZ((0x700 + x) * 16, 0x7900, 0);
        gifs.ListXYZ((0x720 + x) * 16, 0x8700, 0);
    }
    if (n1 == 8 && n2 == 7) {
        gifs.AddPrimPack(0, 1, 0xE);
        AddFramePatch(0, &gifs, &ps);
        gifs.PackAD(0x4C, 0xFFFF00FF00000000L);
    }
    gifs.EndPrim(1);
    AddFramePatch(-1, &gifs, &ps);
    pvifs->AddVifUnpack(UPK_V4_32, (uint)(gifs.m_pb - gifs.m_ab) >> 4, gifs.m_ab, 0x23);
    pvifs->AddVifFlusha();
    pvifs->AddVifMscal(D_00001AA0);
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
    D_002DEB70[n1 == 8 ? 1 : 0] = 1;
}
} // namespace M_BeginRpProjVolume

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_EndRpProjVolume {
// Build the screen-effect VIF and GIF packets.
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
struct COMMAND {
    short kind, index;
};
struct GAME {
    char pad[0x3E38];
    int ready;
};
extern "C" { extern GAME D_003DD490; }
extern "C" { extern int D_002DEB70[2]; }
extern "C" { 
 }
extern "C" { extern QW*D_002DEA4C __asm__("g_pfrmOpen"); }
extern "C" { extern QW D_002DEA50[],D_002DF940[],D_002DFA80[]; }
extern "C" void AddFramePatch(int,GIFS*,COMMAND**);
extern "C" void BuildProjectedVolumeGifs(GIFS*,uint,COMMAND**);
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char COMMAND_Size[sizeof(COMMAND)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
typedef char GIFS_m_ab_Offset[FIELD_OFFSET(GIFS,m_ab)==4?1:-1];
typedef char GIFS_m_pb_Offset[FIELD_OFFSET(GIFS,m_pb)==12?1:-1];
typedef char GAME_ready_Offset[FIELD_OFFSET(GAME,ready)==15928?1:-1];
typedef char COMMAND_kind_Offset[FIELD_OFFSET(COMMAND,kind)==0?1:-1];
typedef char COMMAND_index_Offset[FIELD_OFFSET(COMMAND,index)==2?1:-1];
// Pass 6/7 end: apply pass reading the framebuffer as an indexed texture (TEX2 g_sw.tex2ProjVolume) with alpha blend
// over full-screen strips (MSCAL 0x1C48) (inferred)
extern "C" void EndRpProjVolume(VIFS *input, int first, int mode)
{
    register VIFS *vifs __asm__("$20") = input;
    register uint programHigh __asm__("$22");
    const uint entryAddress = 0x1C48;
    __asm__("lui %0,%1" : "=r"(programHigh) : "i"(entryAddress >> 16));
    vifs->AddDmaCnt();
    uint *pun;
    vifs->UnpackHelper(UPK_V2_16, 8, 0x1B, 0, &pun);
    COMMAND *cursor = (COMMAND *)pun;
    GIFS gifs;
    QW data[0x200];
    gifs.AllocStatic(0x200, data);
    if (mode != 8) {
        if (D_002DEB70[1]) {
            gifs.AddPrimPack(0, 1, 0xE);
            int right = 0x7200, left = 0x7000;
            AddFramePatch(0, &gifs, &cursor);
            gifs.PackAD(0x4C, 0xFFFFFEFF00000000L);
            gifs.PackAD(0x47, 0x31001L);
            gifs.PackAD(0x42, 0x2000000A9L);
            gifs.PackAD(1, 0x100000000L);
            gifs.PackAD(0, 0x46L);
            gifs.AddPrimList(2, 0x55UL);
            int remaining = 0x1E0;
            do {
                gifs.ListXYZ(left, 0x7900, 0);
                left += 0x200;
                gifs.ListXYZ(right, 0x8700, 0);
                remaining -= 0x20;
                right += 0x200;
            } while (remaining >= 0);
        }
        BuildProjectedVolumeGifs(&gifs, 0x810095, &cursor);
        BuildProjectedVolumeGifs(&gifs, 0x266, &cursor);
    }
    gifs.AddPrimPack(0, 1, 0xE);
    AddFramePatch(0, &gifs, &cursor);
    gifs.PackAD(0x4C, 0);
    gifs.PackAD(0x46, 1);
    if (mode != 8) {
        AddFramePatch(2, &gifs, &cursor);
        gifs.PackAD(6, 0x6A8000000L);
        gifs.PackAD(0x14, 1);
        gifs.PackAD(0x16, g_sw.tex2ProjVolume);
        gifs.PackAD(0x3F, 0);
        gifs.PackAD(0x47, 0x31001);
        gifs.PackAD(1, 0x180808080L);
        gifs.PackAD(0x42, 0x44);
        gifs.PackAD(0, 0x156);
        gifs.AddPrimList(4, 0x5353);
        for (int x = 0; x < 0x200; x += 0x20) {
            gifs.ListUV((x << 4) | 8, 8);
            gifs.ListXYZ((x + 0x700) << 4, 0x7900, 0);
            gifs.ListUV(((x + 0x20) << 4) | 8, 0xE08);
            gifs.ListXYZ((x + 0x720) << 4, 0x8700, 0);
        }
    }
    gifs.EndPrim(1);
    AddFramePatch(-1, &gifs, &cursor);
    int count = (uint)(gifs.m_pb - gifs.m_ab) >> 4;
    if (count >= 0x100) {
        vifs->AddVifUnpack(UPK_V4_32, 0xFF, gifs.m_ab, 0x23);
        vifs->AddVifUnpack(UPK_V4_32, ((uint)(gifs.m_pb - gifs.m_ab) >> 4) - 0xFF, gifs.m_ab + 0xFF0, 0x122);
    } else
        vifs->AddVifUnpack(UPK_V4_32, count, gifs.m_ab, 0x23);
    vifs->AddVifFlusha();
    {
        register char *program __asm__("$5");
        __asm__("addiu %0,%1,%2" : "=r"(program) : "r"(programHigh), "i"(entryAddress & 0xFFFF));
        vifs->AddVifMscal(program);
    }
    vifs->AddVifFlusha();
    GAME *game = &D_003DD490;
    if (game->ready) {
        if (D_002DEA4C == D_002DEA50)
            vifs->AddDmaCall(D_002DF940);
        else
            vifs->AddDmaCall(D_002DFA80);
        vifs->AddVifBaseOffset(0x32, 0x1E7);
    }
    vifs->EndDmaCnt();
    if (mode != 8) {
        D_002DEB70[0] = 0;
        D_002DEB70[1] = 0;
    }
}

typedef char UINT_Size[sizeof(uint)==4?1:-1];
typedef char ULONG_Size[sizeof(ulong)==8?1:-1];
typedef char GAME_Size[sizeof(GAME)==0x3E3C?1:-1];
} // namespace M_EndRpProjVolume

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpGlob {
extern "C" void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern "C" void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" { extern char D_0025E680[]; }
extern "C" { extern char D_00262320[]; }
extern "C" { extern char D_00000000[]; }
// Pass 0 begin: upload main glob microprogram D_0025E680..D_00262320, zero the 11-qw batch descriptor at TOPS+0xA,
// MSCAL 0. (inferred)
extern "C" void BeginRpGlob(VIFS *pvifs)
{
    uint *pun;
    EnsureVu1Code__FP4VIFSPvT1(pvifs, D_0025E680, D_00262320);
    pvifs->AddDmaCnt();
    pvifs->UnpackHelper(UPK_V4_16, 11, 10, 0, &pun);
    func_002333B4(pun, 0, 0x58);
    pvifs->AddVifMscal(D_00000000);
    pvifs->EndDmaCnt();
}
} // namespace M_BeginRpGlob

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_EndRpGlob {
extern "C" void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern "C" { extern char D_00003AB0[]; }
// Pass 0 end (shared by passes 8-10): zero batch descriptor, MSCAL 0x3AB0 (flush entry), FLUSHA. (inferred)
extern "C" void EndRpGlob(VIFS *pvifs)
{
    uint *pun;
    pvifs->AddDmaCnt();
    pvifs->UnpackHelper(UPK_V4_16, 11, 10, 0, &pun);
    func_002333B4(pun, 0, 0x58);
    pvifs->AddVifMscal(D_00003AB0);
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
}
} // namespace M_EndRpGlob

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpBackground {
extern "C" { extern int D_003DCFA0; }
extern "C" { extern QW *D_003DCFA8[]; }
extern "C" void BeginRpGlob(VIFS *pvifs);
// Pass 3 begin (kind 1 sky/background): BeginRpGlob + DMA CALL each per-frame texture upload chain
// D_003DCFA8[0..D_003DCFA0) (inferred)
extern "C" void BeginRpBackground(VIFS *pvifs)
{
    int i;
    BeginRpGlob(pvifs);
    for (i = 0; i < D_003DCFA0; i++)
        pvifs->AddDmaCall(D_003DCFA8[i]);
}
} // namespace M_BeginRpBackground

// CNT{FLUSH, DIRECT(cqw, static GIF packet), FLUSH}. (inferred)
/* A flushed VIF DIRECT transfer of cqw quadwords (its own DMA cnt). */
extern "C" void AddVifDirectFlushed(VIFS *pvifs, int cqw, QW *aqw)
{
    pvifs->AddDmaCnt();
    pvifs->AddVifFlush();
    pvifs->AddVifDirect(cqw, aqw, 0);
    pvifs->AddVifFlush();
    pvifs->EndDmaCnt();
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_EndRpBackground { extern "C" {
extern int D_003DCFA0;
extern char D_002DF010[];
void EndRpGlob(void *p);
void AddVifDirectFlushed(void *p, int n, void *pv);
// Pass 3 end: EndRpGlob + Z-only clear (D_002DF010) when per-frame uploads were made (textures parked in Z-buffer
// memory) (inferred)
void EndRpBackground(void *p)
{
    EndRpGlob(p);
    if (D_003DCFA0 > 0)
        AddVifDirectFlushed(p, 0x25, D_002DF010);
}
} } // namespace M_EndRpBackground

// Pass 11 begin (kinds 4/6): BeginRpGlob + DIRECT FBA_1=1 packet D_002DF700. (inferred)
extern "C" void BeginRpCutout(VIFS *pvifs)
{
    BeginRpGlobV(pvifs);
    AddVifDirectFlushed(pvifs, 2, D_002DF700);
}

// Pass 11 end: EndRpGlob + DIRECT FBA_1=0 packet D_002DF720. (inferred)
extern "C" void EndRpCutout(VIFS *pvifs)
{
    EndRpGlobV(pvifs);
    AddVifDirectFlushed(pvifs, 2, D_002DF720);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType1FromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
struct P;struct SPHERE;struct FACE;
struct ENV {
    char pad[0x240];
    float width, height, fade;
};
struct PARAM {
    float value;
    int pad, kind;
};
struct D {
    char channel, count;
    ushort flags;
    float radius;
    QW *chain;
    int base, words;
};
struct BONE {
    int value;
    char pad4[4];
    int index;
    char padC[0x44];
};
struct E {
    char pad[0x10];
    QW *chain;
    int base;
    BONE *bones;
};
struct HEADER {
    ushort flags, base, weight, pad6, position, positionEnd, color, end, bones[4];
};
struct REQUEST {
    int bytes, alignment;
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
};
extern "C"{
void*STACKSCOPE_ctor(void*);
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&);
void BuildCmFgfn(ENV*,float*,float);
void*func_002333B4(void*,int,uint) __asm__("memset");
void LoadSubglobInstancesFromBrx(P*,D*,E*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*);
extern ENV D_002DDF40;
extern uint D_002DEB50;
extern QW D_002DEB60;
extern uint D_002BE540[];
extern UPK D_002BE558[];
extern char D_00000000[],D_00000738[],D_000007F0[];
}
struct LEASE {
    ~LEASE() {}
    void Acquire() { STACKSCOPE_ctor(this); }
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
static inline void *Allocate(int bytes)
{
    return AllocClearedFromWorldHeap(REQUEST(bytes, 16));
}
typedef char PARAM_Size[sizeof(PARAM)==12?1:-1];
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char BONE_Size[sizeof(BONE)==80?1:-1];
typedef char E_Size[sizeof(E)==28?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==24?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char MARK_Size[sizeof(STACKSCOPE)==1?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
typedef char PARAM_value_Offset[FIELD_OFFSET(PARAM,value)==0?1:-1];
typedef char PARAM_kind_Offset[FIELD_OFFSET(PARAM,kind)==8?1:-1];
typedef char D_count_Offset[FIELD_OFFSET(D,count)==1?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_base_Offset[FIELD_OFFSET(D,base)==12?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char BONE_value_Offset[FIELD_OFFSET(BONE,value)==0?1:-1];
typedef char BONE_index_Offset[FIELD_OFFSET(BONE,index)==8?1:-1];
typedef char E_chain_Offset[FIELD_OFFSET(E,chain)==16?1:-1];
typedef char E_bones_Offset[FIELD_OFFSET(E,bones)==24?1:-1];
typedef char HEADER_flags_Offset[FIELD_OFFSET(HEADER,flags)==0?1:-1];
typedef char HEADER_base_Offset[FIELD_OFFSET(HEADER,base)==2?1:-1];
typedef char HEADER_weight_Offset[FIELD_OFFSET(HEADER,weight)==4?1:-1];
typedef char HEADER_position_Offset[FIELD_OFFSET(HEADER,position)==8?1:-1];
typedef char HEADER_positionEnd_Offset[FIELD_OFFSET(HEADER,positionEnd)==10?1:-1];
typedef char HEADER_color_Offset[FIELD_OFFSET(HEADER,color)==12?1:-1];
typedef char HEADER_end_Offset[FIELD_OFFSET(HEADER,end)==14?1:-1];
typedef char HEADER_bones_Offset[FIELD_OFFSET(HEADER,bones)==16?1:-1];
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
typedef char ENV_width_Offset[FIELD_OFFSET(ENV,width)==576?1:-1];
typedef char ENV_height_Offset[FIELD_OFFSET(ENV,height)==580?1:-1];
typedef char ENV_fade_Offset[FIELD_OFFSET(ENV,fade)==584?1:-1];
// Sub-glob type 1 loader: vertices + colours, no shader (GIF prims); HYPOTHESIS shadow/cel-no-shader.
extern "C" void LoadSubglobType1FromBrx(P *p, D *desc, E *entry, PARAM *param, SPHERE *sphere, int groups,
                                        CBinaryInputStream *stream)
{
    int i = 0;
    VIFS vifs;
    STACKSCOPE outer;
    vifs.AllocSw(0x2000);
    vifs.AddDmaCnt();
    float *environment;
    vifs.UnpackHelper(UPK_V4_32, 1, 9, 0, (uint **)&environment);
    BuildCmFgfn(&D_002DDF40, environment, param->value);
    int count = stream->U8Read();
    desc->count = count;
    entry->bones = (BONE *)Allocate((char)count * sizeof(BONE));
    for (; i < desc->count; i++) {
        BONE *bone = &entry->bones[i];
        bone->value = stream->S32Read();
        bone->index = stream->S16Read();
    }
    int base = 0x22;
    GIFS gifs;
    LEASE inner;
    gifs.AllocSw(8);
    gifs.AddPrimPack(0, 2, 0x1EUL);
    gifs.PackAD(0x42L, 0x44L);
    gifs.PackRGBA(D_002DEB50);
    gifs.EndPrim(1);
    vifs.AddVifUnpack(UPK_V4_32, ((uint)gifs.m_pb - (uint)gifs.m_ab) >> 4, gifs.m_ab, 0xD);
    gifs.Reset();
    gifs.AddPrimPack(0xC1, 3, 0x44EUL);
    gifs.PackAD(0x47L, 0x73001L);
    gifs.PackXYZF(0, 0, 0, 0);
    gifs.PackXYZF(0, 0, 0, 0);
    gifs.EndPrim(0);
    gifs.AddPrimPack(1, 3, 0x44EUL);
    gifs.PackAD(0x47L, 0x72001L);
    gifs.PackXYZF(0, 0, 0, 0);
    gifs.PackXYZF(0, 0, 0, 0);
    gifs.EndPrim(1);
    vifs.AddVifUnpack(UPK_V4_32, 2, gifs.m_ab, 0x12);
    vifs.AddVifUnpack(UPK_V4_32, 2, gifs.m_ab + 0x40, 0x16);
    vifs.AddVifUnpack(UPK_V4_32, 2, gifs.m_ab, 0x1A);
    vifs.AddVifUnpack(UPK_V4_32, 2, gifs.m_ab + 0x40, 0x1E);
    if (desc->count) {
        desc->base = base;
        base += desc->count * 4;
    }
    int subCount = stream->S16Read();
    QW *data;
    for (int part = 0; part < subCount;) {
        {
            LEASE *lease = &inner;
            lease->Acquire();
        }
        ushort *header;
        vifs.UnpackHelper(UPK_V4_16, 3, 10, 0, (uint **)&header);
        func_002333B4(header, 0, 24);
        switch (part) {
        case 0:
            header[0] |= 1;
            break;
        case 1:
            header[0] |= 2;
            break;
        }
        int current = base;
        header[1] = current;
        int nextPart = part + 1;
        int vertices = stream->U8Read();
        vifs.AddVifUnpack(UPK_V3_32, vertices, stream, current);
        header[4] = current;
        current += vertices;
        header[5] = current;
        if (desc->count) {
            part = stream->U8Read();
            int k = 0;
            for (; k < part; k++) {
                int bone = stream->U8Read();
                HEADER *slot = (HEADER *)((uint)header + k * 2);
                slot->bones[0] = (ushort)desc->base + bone * 4;
            }
            int next = current + vertices;
            int small = part < 4;
            for (; k < 4; k++) {
                HEADER *slot = (HEADER *)((uint)header + k * 2);
                slot->bones[0] = header[8];
            }
            if (small) {
                vifs.AddVifStrow(&D_002DEB60);
                vifs.AddVifStmask(D_002BE540[part]);
            }
            vifs.AddVifUnpack(D_002BE558[part], vertices, stream, current);
            header[2] = current;
            current = next;
            if (small)
                vifs.AddVifStmask(0);
        }
        part = nextPart;
        int colors = stream->U8Read();
        vifs.AddVifUnpack(UPK_V4_8, colors, stream, current);
        header[6] = current;
        current += colors;
        header[7] = current;
        vifs.AddVifMscal(D_00000000);
        vifs.AddVifMscal(D_00000738);
        vifs.AddVifMscal(D_000007F0);
        {
            LEASE *lease = &inner;
            lease->Release();
        }
    }
    vifs.EndDmaCnt();
    vifs.AddDmaRet();
    vifs.DetachCopySw(&desc->words, &data, &entry->chain);
    LoadSubglobInstancesFromBrx(p, desc, entry, sphere, groups, 0, 0, 0, stream);
}
} // namespace M_LoadSubglobType1FromBrx

// Pass 1 begin (kinds 9/17/20/24): upload the cel-border microprogram D_0025B880..D_0025C1B0. (inferred)
extern "C" void BeginRpCelBorder(VIFS *pvifs)
{
    EnsureVu1Code(pvifs, D_0025B880, D_0025C1B0);
}

// Pass 1 end: FLUSHA. (inferred)
extern "C" void EndRpCelBorder(VIFS *pvifs)
{
    pvifs->AddDmaCnt();
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_CloneGlobset {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
struct TRANSFORM;
struct STREAM;
struct SO;
struct VE {
    short delta, pad;
    void (*readTransform)(SO *, STREAM *, TRANSFORM **);
};
struct SO {
    char pad[0xC];
    VE *vt;
    char pad10[0x10];
    uint flags;
    char position[0x10];
    char list[0x10];
    int vectorCount;
    void *vectors;
};
struct D {
    char channel, segments;
    int flags : 8;
    char kind;
    char pad4[4];
    void *chain;
    char padC[4];
    int words;
};
union FLAGS {
    uint word;
    struct {
        uint pad : 11, feature : 1, matrix : 1, first : 9, last : 9, reserved : 1;
    } bits;
};
struct E {
    int value;
    TRANSFORM *transform;
    void *parent;
    FLAGS flags;
    char *data;
    uint base;
    char *segments;
    int word1C;
    SO *object;
    void *sphere;
};
struct P {
    SO *owner;
    char pad4[0x20];
    int objectCount;
    SO **objects;
    char pad2C[12];
    int count;
    D *descriptors;
    E *entries;
    char pad44[0x14];
    int word58;
};
struct LINK {
    int pad0;
    void *bounds;
    int pad8;
    SO *object;
    int next;
};
struct REQUEST {
    int bytes, alignment;
};
typedef char D_Size[sizeof(D)==20?1:-1];
typedef char E_Size[sizeof(E)==40?1:-1];
typedef char LINK_Size[sizeof(LINK)==20?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char FLAGS_Size[sizeof(FLAGS)==4?1:-1];
typedef char D_channel_Offset[FIELD_OFFSET(D,channel)==0?1:-1];
typedef char D_segments_Offset[FIELD_OFFSET(D,segments)==1?1:-1];
typedef char D_kind_Offset[FIELD_OFFSET(D,kind)==3?1:-1];
typedef char D_chain_Offset[FIELD_OFFSET(D,chain)==8?1:-1];
typedef char D_words_Offset[FIELD_OFFSET(D,words)==16?1:-1];
typedef char E_transform_Offset[FIELD_OFFSET(E,transform)==4?1:-1];
typedef char E_parent_Offset[FIELD_OFFSET(E,parent)==8?1:-1];
typedef char E_flags_Offset[FIELD_OFFSET(E,flags)==12?1:-1];
typedef char E_data_Offset[FIELD_OFFSET(E,data)==16?1:-1];
typedef char E_base_Offset[FIELD_OFFSET(E,base)==20?1:-1];
typedef char E_segments_Offset[FIELD_OFFSET(E,segments)==24?1:-1];
typedef char E_object_Offset[FIELD_OFFSET(E,object)==32?1:-1];
typedef char E_sphere_Offset[FIELD_OFFSET(E,sphere)==36?1:-1];
typedef char P_owner_Offset[FIELD_OFFSET(P,owner)==0?1:-1];
typedef char P_objectCount_Offset[FIELD_OFFSET(P,objectCount)==36?1:-1];
typedef char P_objects_Offset[FIELD_OFFSET(P,objects)==40?1:-1];
typedef char P_count_Offset[FIELD_OFFSET(P,count)==56?1:-1];
typedef char P_descriptors_Offset[FIELD_OFFSET(P,descriptors)==60?1:-1];
typedef char P_entries_Offset[FIELD_OFFSET(P,entries)==64?1:-1];
typedef char P_word58_Offset[FIELD_OFFSET(P,word58)==88?1:-1];
typedef char SO_vt_Offset[FIELD_OFFSET(SO,vt)==12?1:-1];
typedef char SO_flags_Offset[FIELD_OFFSET(SO,flags)==32?1:-1];
typedef char SO_position_Offset[FIELD_OFFSET(SO,position)==36?1:-1];
typedef char SO_list_Offset[FIELD_OFFSET(SO,list)==52?1:-1];
typedef char SO_vectorCount_Offset[FIELD_OFFSET(SO,vectorCount)==68?1:-1];
typedef char SO_vectors_Offset[FIELD_OFFSET(SO,vectors)==72?1:-1];
typedef char LINK_bounds_Offset[FIELD_OFFSET(LINK,bounds)==4?1:-1];
typedef char LINK_object_Offset[FIELD_OFFSET(LINK,object)==12?1:-1];
typedef char LINK_next_Offset[FIELD_OFFSET(LINK,next)==16?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_readTransform_Offset[FIELD_OFFSET(VE,readTransform)==4?1:-1];
extern "C"{
extern int D_003DA790;
extern LINK D_003DA798[];
void*AllocCopyFromWorldHeap(REQUEST*,void*);
SO*CloneSaa(SO*);
void CopyRelocateDmaChain(D*,E*,int,void*,int,void*,void*);
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
void ClearSle(int*);
void PrependSlEntry(void*,LINK*);
void RelocateShaderUploadRecords(int,int,int,uint,uint);
}
static inline void *Copy(int bytes, void *source)
{
    REQUEST request;
    request.bytes = bytes;
    request.alignment = 16;
    return AllocCopyFromWorldHeap(&request, source);
}
static inline void ReadTransform(SO *object, STREAM *stream, TRANSFORM **output)
{
    VE *method = &object->vt[52];
    method->readTransform((SO *)((char *)object + method->delta), stream, output);
}
static inline void CopyObjects(P *p)
{
    for (int i = 0; i < p->objectCount; i++) {
        if (p->objects[i]->flags & 4) {
            p->objects = (SO **)Copy(p->objectCount * 4, p->objects);
            return;
        }
    }
}
// Clones a globset for a cloned SO: copies entries/segments, re-resolves transforms, swaps cloned objects, relocates
// packet copies. (inferred)
extern "C" void CloneGlobset(P *p, SO *owner, STREAM *stream)
{
    int bytes;
    p->owner = owner;
    p->entries = (E *)Copy(p->count * sizeof(E), p->entries);
    p->word58 = 0;
    CopyObjects(p);
    for (int i = 0; i < p->count; i++) {
        D *descriptor = &p->descriptors[i];
        E *entry = &p->entries[i];
        ReadTransform(owner, stream, &entry->transform);
        entry->segments = (char *)Copy(descriptor->segments * 0x50, entry->segments);
        for (int j = 0; j < descriptor->segments; j++) {
            uint address = (uint)entry->segments;
            address += j * 0x50;
            ReadTransform(owner, stream, (TRANSFORM **)(address + 0xC));
        }
        int cloned = entry->object && (entry->object->flags & 4);
        if (cloned) {
            SO *object = CloneSaa(entry->object);
            for (int j = 0; j < p->objectCount; j++)
                if (p->objects[j] == entry->object) {
                    p->objects[j] = object;
                    break;
                }
            entry->object = object;
        }
        if (!descriptor->chain && !cloned)
            continue;
        int count = 0;
        void *old = 0;
        void *data = 0;
        if (descriptor->flags & 1) {
            count = descriptor->words;
            old = entry->data;
            entry->data = (char *)Copy(count * 16, old);
            data = entry->data;
        } else {
            if (entry->object) {
                count = entry->object->vectorCount;
                old = data = entry->object->vectors;
                if (cloned) {
                    entry->object->vectors = Copy(count * 16, data);
                    data = entry->object->vectors;
                }
            }
            CopyRelocateDmaChain(descriptor, entry, descriptor->words, entry->data, count, data, old);
        }
        if (entry->object) {
            bytes = count * 16;
            int span = entry->flags.bits.last - entry->flags.bits.first;
            CopyAb__FPvT0Ui(&D_003DA798[D_003DA790], &D_003DA798[entry->flags.bits.first], span * sizeof(LINK));
            entry->flags.bits.first = D_003DA790;
            D_003DA790 += span;
            entry->flags.bits.last = D_003DA790;
            for (uint j = entry->flags.bits.first; j < entry->flags.bits.last; j++) {
                LINK *link = &D_003DA798[j];
                if (cloned) {
                    link->bounds = &entry->object->position;
                    link->object = entry->object;
                }
                ClearSle(&link->next);
                PrependSlEntry(link->object->list, link);
            }
            RelocateShaderUploadRecords(entry->flags.bits.first, entry->flags.bits.last, bytes, (uint)old, (uint)data);
        }
    }
}
} // namespace M_CloneGlobset

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ResetGlobAttachment { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct V4 { float x, y, z, w; } __attribute__((aligned(16)));   /* attribute on the struct (C++) */
typedef struct { char pad[0x148]; unsigned long renderFlags; } A;   /* the attached ALO: its 64-bit flag word at +0x148 */
typedef struct { float x, y, z, w; int a, b; } E;
typedef struct { A *pa; int pad[3]; V4 offset; float radius; int pad2[8]; E *ae; } S;
extern V4 D_002A2DD0;
void RemoveLoFromRenderTree(A *pa);
/* GetVectorComponentX/E0/E8: the file-scope inline readers */



// Resets an attachment record (clear flag on attached object, zero vector, radius 1e6) (inferred)
void ResetGlobAttachment(S *p, int i)
{
    float g = 1000000.0f;
    E *pe;
    V4 v;
    float x, y, z;
    RemoveLoFromRenderTree(p->pa);
    p->pa->renderFlags &= 0xFFFFFFF7FFFFFFFFUL;   /* clear bit 35 */
    p->offset = D_002A2DD0;
    p->radius = g;
    pe = &p->ae[i];
    v = D_002A2DD0;
    x = ::GetVectorComponentX((VECTOR *)&v);
    y = ::GetVectorComponentY((VECTOR *)&v);
    z = ::GetVectorComponentZ((VECTOR *)&v);
    pe->x = x;
    pe->y = y;
    pe->z = z;
    pe->w = g;
}
} } // namespace M_ResetGlobAttachment

// Return v[0] [game linkonce (inline/template)]. (inferred)
// 0x2599D8: a VECTOR component reader: inline, defined after its callers, so g++ calls it and emits the
// out-of-line copy (linkonce), as the original did.
inline float GetVectorComponentX(VECTOR *pvec)
{
    return pvec->x;
}

// Return v[1] [game linkonce (inline/template)]. (inferred)
// 0x2599E0: a VECTOR component reader: inline, defined after its callers, so g++ calls it and emits the
// out-of-line copy (linkonce), as the original did.
inline float GetVectorComponentY(VECTOR *pvec)
{
    return pvec->y;
}

// Return v[2] [game linkonce (inline/template)]. (inferred)
// 0x2599E8: a VECTOR component reader: inline, defined after its callers, so g++ calls it and emits the
// out-of-line copy (linkonce), as the original did.
inline float GetVectorComponentZ(VECTOR *pvec)
{
    return pvec->z;
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
   /* STMASK per weight count */
   /* UNPACK format per weight count */

// .bss after D_002DEB50 (which has a constructor and is emitted where it is defined). Plain objects are emitted at
// the end of the file, file scope first and then namespace by namespace (the latest-opened namespace first), so
// these are defined here at file scope, in address order, instead of inside their functions' namespaces.
extern "C" {
int D_002DEB54;
QW D_002DEB60;
int D_002DEB70[2];
}

// ---- glob's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
uint D_002BE540[5] = {0, 0x54545454, 0x50505050, 0x40404040, 0};
int D_002BE558[5] = {-1, 2, 6, 10, 14};
VECTOR4 D_002BE570 = {0.0f, 0.0f, 1.5f, 0.0f};
namespace M_LoadSubglobType0FromBrx {
extern "C" int D_002BE580 = 0;
}
