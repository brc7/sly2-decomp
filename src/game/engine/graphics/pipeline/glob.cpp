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
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_glob

#include "sly2/basic.h"
#include "sly2/port.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/glob.h"
#include "sly2/wr.h"
#include "sly2/render.h"
#include "sly2/shd.h"
#include "sly2/light.h"
#include "sly2/cm.h"
#include "vifs.h"
#include "gifs.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/classes.h"
#include "sly2/clock.h"
#include "sly2/hw.h"

// The real class behind the per-function SO views below, for its virtual calls.
static inline ::SO *AsSo(void *pso) { return (::SO *)pso; }

// 0x16EFE0 StaticInit_glob and its stub GlobalConstructors_glob: g++'s initializer for the cel-border colour
// (an RGBA word with a constructor, 0x80101010) and the six engine slot pools.
struct RGBAW { uint rgba; RGBAW(uint n) { rgba = n; } };
RGBAW D_002DEB50(0x80101010);
SLY2_INSTANTIATE_SLOT_POOLS()

typedef unsigned size_t; inline float GetVectorComponentX(VECTOR *pvec); inline float GetVectorComponentY(VECTOR *pvec); inline float GetVectorComponentZ(VECTOR *pvec);   /* the VECTOR component readers: inline, defined after their callers (end of file) */

extern "C" {
extern char D_0025B880[], D_0025C1B0[];   /* the cel-border VU1 microprogram */
extern QW D_002DF700[2], D_002DF720[2];   /* the cutout GS register packets */
extern VIFS D_002DEA28;                     /* the frame's VIF1 packet stream */
void EnsureVu1Code(VIFS *pvifs, void *pvStart, void *pvEnd) __asm__("EnsureVu1Code__FP4VIFSPvT1");
void BeginRpGlobV(VIFS *pvifs) __asm__("BeginRpGlob");   /* named apart: other functions declare their own views */
void EndRpGlobV(VIFS *pvifs) __asm__("EndRpGlob");
void AddVifDirectFlushed(VIFS *pvifs, int cqw, QW *aqw);
}




#include "bis.h"

struct BND
{
      OID oid;
      LO *plo;
      int ixf;
      XF *pxf;
      union { MATRIX4 matInv; MATRIX4Q matqInv; };   /* the quadword rows for UnpackSubglobBones */
};





// GLOBSET constructor, at the start of the glob TU.
/* GLOBSET's constructor (sly2/so.h; it was kept as a C function under the symbol __4CRef before). */
GLOBSET::GLOBSET()
{
    relightStamp = -1;
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
extern "C" SAA *PsaaLoadFromBrx(int isGlobsetOwned, CBinaryInputStream *pbis) __asm__("LoadSaaFromBrx");
extern "C" void LoadSubglobType2FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
extern "C" void LoadSubglobType1FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis);
void LoadSubglobType3FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType3FromBrx");
extern "C" {
void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType0FromBrx");
}
// Allocate cb zeroed bytes, 16-byte aligned, from the world heap.
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
        pglobset->apsaa[i] = PsaaLoadFromBrx(1, pbis);
    }
    pglobset->numShaderOids = pbis->U8Read();
    pglobset->shaderOids = (int *)PvAllocSwClear(pglobset->numShaderOids * sizeof(int));
    pglobset->selectedShader = 0;
    for (int i = 0; i < pglobset->numShaderOids; i++)
    {
        pglobset->shaderOids[i] = pbis->S32Read();
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
namespace M_LoadPoseSetsFromBrx {
typedef unsigned int uint;
typedef unsigned long ulong;
struct STREAM;
short S16Read__18CBinaryInputStream(struct STREAM*) __asm__("S16Read__18CBinaryInputStream");
char S8Read__18CBinaryInputStream(struct STREAM*) __asm__("S8Read__18CBinaryInputStream");
struct VECTOR;
void ReadVector__18CBinaryInputStreamP6VECTOR(struct STREAM*,struct VECTOR*) __asm__("ReadVector__18CBinaryInputStreamP6VECTOR");
float F32Read__18CBinaryInputStream(struct STREAM*) __asm__("F32Read__18CBinaryInputStream");
unsigned char U8Read__18CBinaryInputStream(struct STREAM*) __asm__("U8Read__18CBinaryInputStream");
void*AllocClearedFromWorldHeap(struct SWALLOC*) __asm__("AllocClearedFromWorldHeap");
// Reads groups of up to 4 distance/LOD records (kind, position, radius, limit<=1e10, count) per globset descriptor.
// (inferred)
// Also known as: LoadGlobSpheresFromBrx (upstream readable / Sly 1 name;
extern "C" void LoadPoseSetsFromBrx(GLOBSET *p, struct STREAM *stream)
{
    int count = S16Read__18CBinaryInputStream(stream);
    if (count) {
        struct SWALLOC request;
        POSESET *groups;
        int i;
        request.cb = count * sizeof(POSESET);
        request.cbAlign = 0x10;
        groups = (POSESET *)AllocClearedFromWorldHeap(&request);
        for (i = 0; i < count; i++) {
            POSESET *group = &groups[i];
            uint j;
            for (j = 0; j < 4; j++) {
                POSE *record = &group->apose[j];
                record->nKind = S8Read__18CBinaryInputStream(stream);
                if ((int)record->nKind == -1) {
                    for (; j < 4; j++)
                        group->apose[j].nKind = -1;
                    break;
                }
                ReadVector__18CBinaryInputStreamP6VECTOR(stream, (struct VECTOR *)&record->pos);
                record->sRadius = F32Read__18CBinaryInputStream(stream);
                record->sLimit = F32Read__18CBinaryInputStream(stream);
                record->nCount = U8Read__18CBinaryInputStream(stream);
                record->sLimit = FpuMin(1.0e10f, record->sLimit);
                switch (record->nKind) {
                case 0: {
                    int index = S16Read__18CBinaryInputStream(stream);
                    SUBGLOB *descriptor = &p->subMeshes[index];
                    GLOB *entry = &p->pieces[index];
                    if (entry->grf & 0x1000) {
                        record->nKind = 3;
                        record->pvLink = entry->pqwChain;
                    }
                    record->nChannel = descriptor->unknown00;
                    record->isHidden = (entry->grf >> 11) & 1;
                    record->iglob = index;
                    entry->ppose = record;
                    break;
                }
                case 1:
                    record->pvLink = &groups[S16Read__18CBinaryInputStream(stream)];
                    break;
                }
            }
        }
        p->aposeset = groups;
    }
}
} // namespace M_LoadPoseSetsFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BindGlobset {
extern SO *FindSwObject__FP2SWi3OIDP2LO(SW *pg, int kind, int oid, void *owner) __asm__("FindSwObject__FP2SWi3OIDP2LO");   /* the found object is an SO here */
extern void InvertMatrix4Rigid(MATRIX4 *pmatDst, MATRIX4 *pmatSrc) __asm__("InvertMatrix4Rigid");
extern void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, MATRIX4 *pmatA, MATRIX4 *pmatB) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
extern void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern void AddWrWarpGroup(SO *plo, WRBG *pwrbg) __asm__("AddWrWarpGroup");
// Resolves globset bone references after load: finds each referenced SO by oid, its bone, and computes the relative
// bind matrix. (inferred)
// Also known as: BindAloBones (upstream readable / Sly 1 name; BindAloBones__FP3ALO).
extern "C" void BindGlobset(GLOBSET *p)
{
    if (p->relightStamp != -1)
        p->relightStamp = g_sw.relightStamp;
    for (int i = 0; i < p->numPieces; i++) {
        SUBGLOB *phdr = &p->subMeshes[i];
        GLOB *pgrp = &p->pieces[i];
        for (int j = 0; j < phdr->cbnd; j++) {
            BND *pitem = &pgrp->abnd[j];
            pitem->plo = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, pitem->oid, p->owner);
            if (pitem->plo)
                pitem->pxf = ((SO *)pitem->plo)->GetXfFromIndex(pitem->ixf);
            if (pitem->pxf) {
                MATRIX4 mat;
                if (pgrp->transform) {
                    MATRIX4 matT;
                    InvertMatrix4Rigid(&matT, &pitem->pxf->worldMatrix);
                    __ml__FRC7MATRIX4T0(&mat, &matT, &pgrp->transform->worldMatrix);
                } else {
                    InvertMatrix4Rigid(&mat, &pitem->pxf->worldMatrix);
                }
                pitem->matInv = mat;
            } else if (pgrp->transform) {
                pitem->matInv = pgrp->transform->worldMatrix;
            } else {
                MATRIX4 *pmat = &pitem->matInv;
                func_002333B4(pmat, 0, sizeof(MATRIX4));
                pmat->mat[0][0] = pmat->mat[1][1] = pmat->mat[2][2] = pmat->mat[3][3] = 1.0f;
            }
        }
    }
    for (WRBG *px = p->pwrbgFirst; px; px = px->pwrbgNextGlobset) {
        SO *plo = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, px->oid, p->owner);
        if (plo)
            AddWrWarpGroup(plo, px);
    }
}
} // namespace M_BindGlobset

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ExpandSphereForProjection {
struct M4 { qword r[4]; };
extern const qword D_002BE570 __asm__("D_002BE570");
// Transform point p by the 4x4 matrix m.
extern "C" { static inline qword Transform(const struct M4 *m, qword p)
{
    qword r;
    __asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w"
            : "=j"(r)
            : "j"(m->r[0]), "j"(m->r[1]), "j"(p), "j"(m->r[2]), "j"(m->r[3]));
    return r;
} }
// Length of the xyz part of v as a float.
extern "C" { static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one)
            : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    return f;
} }
// Return p + v * k.x.
extern "C" { static inline qword MAdd(qword p, qword v, qword k)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(p), "j"(v), "j"(k));
    return r;
} }
// Grows a sub/model cull sphere to include its projection through pshd->pmatClip (projected-volume geometry)
// (inferred)
extern "C" qword ExpandSphereForProjection(const struct M4 *m, qword p, float *radius)
{
    QWROW u;
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
} // namespace M_ExpandSphereForProjection

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_StoreRelightColors {
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
extern "C" void StoreRelightColors(struct S *p)
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
} // namespace M_StoreRelightColors

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_VU1_Interrupt {
void StoreRelightColors(int n) __asm__("StoreRelightColors");
void iFlushCache(int n) __asm__("iFlushCache");
// INTC VIF1/VU1 interrupt handler: reads back VU1 data memory results (colour conversion), clears the VIF1 stall
// (VIF1_FBRST=8), returns -1. (inferred)
extern "C" int VU1_Interrupt(void)
{
    StoreRelightColors(0x1100C320);
    iFlushCache(0);
    *(volatile int *)SLY2_HW_VIF1_FBRST = 8;
    asm volatile("sync\n\tei");
    return -1;
}
} // namespace M_VU1_Interrupt

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpRelight {
struct X { char pad[0x120]; QW amat[3]; int pad150[4]; int isDisabled; };   /* a light: 3 qwords of relight data; isDisabled (inferred): skipped */
extern char D_0025E090[] __asm__("D_0025E090");
extern char D_0025E668[] __asm__("D_0025E668");
extern void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd) __asm__("EnsureVu1Code__FP4VIFSPvT1");
extern int FindSwObjectsByClass(SW *pg, int kind, int n, int m, int c, void **apv) __asm__("FindSwObjectsByClass");
extern void CopyAb__FPvT0Ui(void *pvDst, void *pvSrc, unsigned int cb) __asm__("CopyAb__FPvT0Ui");
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
    int c = FindSwObjectsByClass(&g_sw, 0x505, 4, 0, 0x180, (void **)apx);
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
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;

struct M3
{
    QWROW r[3];
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

struct ENTRY
{
    int unknown;
    XF *transform;
    MATRIX4Q *parent;
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
    QWROW sphere;
    MATRIX4Q matrix;
};

void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
MATRIX4Q *__ml__FRC7MATRIX4T0(MATRIX4Q *pmatDst, const MATRIX4Q *pmatA, const MATRIX4Q *pmatB) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
float GetMaxRowLengthMatrix3(const M3 *pmat) __asm__("GetMaxRowLengthMatrix3");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");

// Transform point qwPos by the 4x4 matrix pmat.
static inline qword QwTransformPos(MATRIX4Q *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(pmat->r[0].q), "j"(pmat->r[1].q), "j"(pmat->r[2].q), "j"(pmat->r[3].q), "j"(qwPos));
    return qwResult;
}
// Copy cw words (a multiple of 4) from pwSrc to pwDst one quadword at a time; fNever stands in for dead code the
// original kept.
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
// Append a VIF code TAG and then cw words from pv to the DMA packet D_002DEA28.
template <uint TAG> static inline void UnpackQws(const void *pv, int cw, int fNever)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = TAG;
    pdma->m_pb += 4;
    CopyQws((uint *)pdma->m_pb, (const uint *)pv, cw, fNever);
    pdma->m_pb += cw * 4;
}
// Return the entry's world matrix: its transform, times its parent's matrix when it has one.
static inline MATRIX4Q ModelMatrix(const ENTRY *entry) return model
{
    const MATRIX4Q *parent = entry->parent;
    func_0023320C(&model, &entry->transform->worldMatq, sizeof(MATRIX4Q));
    if (parent)
    {
        MATRIX4Q product;
        __ml__FRC7MATRIX4T0(&product, &model, parent);
        func_0023320C(&model, &product, sizeof(MATRIX4Q));
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
            CopyAb__FPvT0Ui(&packet.matrix, entry->matrixData + 0x14, sizeof(MATRIX4Q));
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
            QWROW center;
            CopyAb__FPvT0Ui(&center, &sphere->sphere.center, sizeof(V3));
            packet.sphere.q = QwTransformPos(&packet.matrix, center.q);
            packet.sphere.f[3] = GetMaxRowLengthMatrix3((const M3 *)&packet.matrix) * sphere->sphere.radius;
        }
        ulong *ptag = (ulong *)D_002DEA28.m_pb;
        *ptag = 0x10000000;
        D_002DEA28.m_pb += 0x10;
        UnpackQws<0x6C06C301>(&packet, sizeof(PACKET) / sizeof(uint), fNever);
        {
            VIFS *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->m_pb; (uptr)pbPad & 0xF; pbPad = pdma->m_pb)
            {
                *(uint *)pbPad = 0;
                pdma->m_pb += 4;
            }
        }
        *(uint *)ptag |= ((D_002DEA28.m_pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.m_pb = 0x60000000;
        D_002DEA28.m_pb += 0x10;
        RPL *prpl = AllocRenderListEntry();
        prpl->pbCall = (uchar *)ptag;
        prpl->kind = 0;
        *(int *)&prpl->depth = 0;   /* an int store of 0.0f */
        prpl->pvCall = descriptor->chain;
    }
    p->tick = g_sw.relightStamp;
}
} // namespace M_SubmitGlobsetRelight

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UnpackSubglobBones {
typedef unsigned int uint;

// Load a 128-bit value with lq into a GPR (no VU0 round trip).
extern "C" { static __inline__ qword ReadCPU(const qword *source)
{
    qword value;
    __asm__("lq %0,%1" : "=r"(value) : "m"(*source));
    return value;
} }

// Pad the DMA packet with zero words until its cursor is at the given offset within a quadword.
extern "C" { static __inline__ void Align(uint align)
{
    VIFS *dma = &D_002DEA28;
    qword *output = (qword *)dma->m_pb;
    while (((uptr)output & 15) != align) {
        *(uint *)output = 0;
        dma->m_pb += 4;
        output = (qword *)dma->m_pb;
    }
} }

// Appends VIF UNPACKs of a sub-glob's bone matrices to the instance packet. (inferred)
// Also known as: FGlobBoneMatrices (upstream readable / Sly 1 name; FGlobBoneMatrices__FP3ALOP4DESCP5ENTRY).
extern "C" void UnpackSubglobBones(void *context, SUBGLOB *descriptor, GLOB *entry)
{
    uint opcode;
    BND *bone, *last;
    XF *parent;
    qword *output;
    qword p0, p1, p2, p3, n0, n1, n2, n3;
    register qword v1 __asm__("vf1"), v2 __asm__("vf2");
    register qword v3 __asm__("vf3"), v4 __asm__("vf4");
    register qword v5 __asm__("vf5"), v6 __asm__("vf6");
    register qword v7 __asm__("vf7"), v8 __asm__("vf8");

    Align(12);
    opcode = (uint)descriptor->cbnd << 18;
    opcode |= 0x6000C000;
    opcode |= 0x0C000000;
    opcode |= descriptor->iqwBone;
    *(uint *)D_002DEA28.m_pb = opcode;
    D_002DEA28.m_pb += 4;
    output = (qword *)D_002DEA28.m_pb;
    bone = entry->abnd;
    last = bone + descriptor->cbnd - 1;
    parent = bone->pxf;
    v5 = bone->matqInv.r[0].q; v6 = bone->matqInv.r[1].q;
    v7 = bone->matqInv.r[2].q; v8 = bone->matqInv.r[3].q;
    v1 = parent->worldMatq.r[0].q; v2 = parent->worldMatq.r[1].q;
    v3 = parent->worldMatq.r[2].q; v4 = parent->worldMatq.r[3].q;
    for (;;) {
        __asm__ __volatile__("vcallms 0x60" : :
                "j"(v1), "j"(v2), "j"(v3), "j"(v4),
                "j"(v5), "j"(v6), "j"(v7), "j"(v8));
        if (bone < last) {
            XF *next = bone[1].pxf;
            p0 = ReadCPU(&next->worldMatq.r[0].q);
            p1 = ReadCPU(&next->worldMatq.r[1].q);
            p2 = ReadCPU(&next->worldMatq.r[2].q);
            p3 = ReadCPU(&next->worldMatq.r[3].q);
            n0 = ReadCPU(&bone[1].matqInv.r[0].q);
            n1 = ReadCPU(&bone[1].matqInv.r[1].q);
            n2 = ReadCPU(&bone[1].matqInv.r[2].q);
            n3 = ReadCPU(&bone[1].matqInv.r[3].q);
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
    D_002DEA28.m_pb = (uchar *)output;
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];

} // namespace M_UnpackSubglobBones

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
// laneI9v1 (Claude agent): aligned 31. laneI8v1 + final dist qmfc2 written straight to prpl->depth via "=r,m" : "j,!r" (never-chosen m alternative keeps the MEM; the qmfc2 output reload in v0 advances reload's round robin, so the pbStart reload gets v1 as in the target).
// laneI8v1 (Claude agent): aligned 33. laneI7v2 + KindOf as (ulong)(psub->grf & 0xF8) >> 3 (target lbu + dsrl in the final switch, 35 -> 33).
// laneI7v2 (Claude agent): aligned 35. laneI6v2 + params (pmodel, pmatWorld, gFirst, gSecond, pshd) (entry pshd spill order, 51 -> 48) + case-3 SqDist returns float straight from qmfc2 (48 -> 35).
// laneI6v2 (Claude agent): aligned 51. laneI5v4 + final GetSquaredLengthBits with qwOne as operand 0, case 3 GBitsSqDist (old order), fAlt kind as (x != 1.0f) ? 23 : 22.
// laneI5v4 (Claude agent): aligned 57. laneI4v5 + pad loop via `DMA *pdma` and a `pbPad` cursor, skin offset `int ibMat = imat*0x40 + 0x6F0`, commented empty volatile asm at the start of CopyQws.
// RenderSoGlobset (Claude agent, lane I3): fresh rewrite in the style of Lilly's matched files.
// laneI4v5 (Claude agent): aligned 120 (raw 179), frame 0x290. v4 + posCam, pos, sRadius declaration order.
// Culls a model against the view (microprogram 0x1C0), then builds the VIF packet for every visible submodel
// (optional reference, model matrix, params, bone matrices (0x1684D0), skin matrices) and queues it for rendering.
// The model is so.h's GLOBSET (SO::model): its GLOBs (SUB in the matched view), SUBGLOBs, SUBGLOBX spheres and the
// owner SO; a GLOB's warp group is a WRBG (wr.h), whose WR holds the warp / uv matrices the skin unpacks copy.
extern MATRIX4 D_002A2D30 __asm__("D_002A2D30");

void SubmitGlobsetRelight(GLOBSET *pglobset) __asm__("SubmitGlobsetRelight");
int ShouldUpdateSaa__FP3SAA(SAA *psaa) __asm__("ShouldUpdateSaa__FP3SAA");
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc) __asm__("InvertMatrix4Rigid");
float GetMaxRowLengthMatrix3(const MATRIX4 *pmat) __asm__("GetMaxRowLengthMatrix3");
qword ExpandSphereForProjection(const MATRIX4 *pmat, qword qwPos, float *psRadius) __asm__("ExpandSphereForProjection");
int IsWithinCameraRenderDistance(CM *pcm, qword qwDpos, float sRange, float *pgVis) __asm__("IsWithinCameraRenderDistance");
void UnpackSubglobBones(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob) __asm__("UnpackSubglobBones");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");

// Return the 4x4 matrix product matA * matB.
static inline MATRIX4 MulMat(const MATRIX4 &matA, const MATRIX4 &matB) return mat
{
    __ml__FRC7MATRIX4T0(&mat, &matA, &matB);
}
// Copy a 4x4 matrix.
static inline void CopyMat(MATRIX4 &matDst, const MATRIX4 &matSrc)
{
    func_0023320C(&matDst, &matSrc, sizeof(MATRIX4));
}
// Transform point qwPos by the 4x4 matrix mat.
static inline qword TransformPoint(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
// Transform point qwPos (by reference) by the 4x4 matrix mat.
static inline qword TransformPointRef(const MATRIX4 &mat, const qword &qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
// Store the bits of qwV's squared length (xyz) into gDst.
static inline void StoreSquaredLengthBits(int &gDst, qword qwV)
{
    qword qwSq, qwOne;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    // the "m" / "!r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
    __asm__("qmfc2.ni %0, %1" : "=r,m"(gDst) : "j,!r"(qwSq));
}
// Bits of qwV's squared length (xyz) as an int.
static inline int GetSquaredLengthBits(qword qwV)
{
    qword qwSq, qwOne;
    int g;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwSq));
    return g;
}
// Squared distance between qw0 and qw1 as a float.
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
    // Load a float into a GPR qword (lw) for later VU0 use.
    VU_FLOATM(const float &g) { __asm__("lw %0, %1" : "=r"(qw) : "m"(g)); }
};
// The sphere's range read straight from the SPH: the struct-member MEM (MEM_IN_STRUCT_P, varying address) does not alias the
// scalar VU_FLOAT temp HOME at a fixed frame address, so sched1 hoists this load above the gain/radius HOME stores (target order).
struct VU_FLOATS
{
    qword qw;
    VU_FLOATS(const ::SO *pso) { __asm__("lw %0, %1" : "=r"(qw) : "m"(pso->busyRadius)); }   // the owner's range (LO::busyRadius)
    VU_FLOATS(const SUBGLOBX *psph) { __asm__("lw %0, %1" : "=r"(qw) : "m"(psph->maxRenderDistance)); }
};
// Microprogram 0x1C0 (sphere against the view planes): its inputs and outputs live in fixed VU registers.
static inline float GetSphereVisibility(qword posCam, qword pos, const ::SO *pso, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = (qword *)g_cm.agFrustumXyz;   /* the transposed plane normals as three VU0 registers */
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(g_cm.rCullScale).qw;
    register qword qwPos __asm__("vf15") = pos;
    register qword qwRadius __asm__("vf16") = VU_FLOATM(sRadius).qw;
    register qword qwRange __asm__("vf17") = VU_FLOATS(pso).qw;
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
static inline float GetSphereVisibilitySync(qword posCam, qword pos, const SUBGLOBX *psph, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = (qword *)g_cm.agFrustumXyz;   /* the transposed plane normals as three VU0 registers */
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(g_cm.rCullScale).qw;
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
// Copy cw words (a multiple of 4) from pwSrc to pwDst one quadword at a time.
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
// Append the VIF code TAG and then cw words from pv to the DMA packet D_002DEA28.
template <uint TAG> static inline void UnpackQws(const void *pv, int cw)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = TAG;
    pdma->m_pb += 4;
    CopyQws((uint *)pdma->m_pb, (const uint *)pv, cw);
    pdma->m_pb += cw * 4;
}
// Append the VIF code tag and then cw words from pv to the DMA packet D_002DEA28.
static inline void UnpackQws(uint tag, const void *pv, int cw)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = tag;
    pdma->m_pb += 4;
    CopyQws((uint *)pdma->m_pb, (const uint *)pv, cw);
    pdma->m_pb += cw * 4;
}
// Append the VIF code tag and then cw words from pv (copied one word at a time) to the DMA packet.
static inline void UnpackWords(uint tag, const void *pv, int cw)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = tag;
    pdma->m_pb += 4;
    uint *pwDst = (uint *)pdma->m_pb;
    const uint *pwSrc = (const uint *)pv;
    for (int k = 0; k < cw; k++)
        *pwDst++ = *pwSrc++;
    pdma->m_pb += cw * 4;
}
// Sub-glob kind: bits 3..7 of its flags.
static inline int KindOf(GLOB *psub)
{
    return (ulong)(psub->grf & 0xF8) >> 3;
}
// Per-model draw: sphere-culls each sub-model against the camera via VU0 microprogram 0x1C0, updates shader
// animations, builds/queues VIF packets (0x1681F8/0x1684D0) (inferred)
extern "C" void RenderSoGlobset(GLOBSET *pmodel, MATRIX4 *pmatWorld, float gFirst, float gSecond, SOBOUNDS *pshd)
{
    SUBGLOB *pdesc;
    qword posCam = g_cm.matq.pos;
    qword pos = TransformPointRef(pmodel->owner->rootTransform->worldMatrix, pmodel->boundsCenterq);
    float sRadius = pmodel->boundsRadius;
    if (pmatWorld)
    {
        pos = TransformPoint(*pmatWorld, pos);
        sRadius *= GetMaxRowLengthMatrix3(pmatWorld);
    }
    if ((pmodel->grf50 & 0x200000000UL) && pshd->pmatClip)
        pos = ExpandSphereForProjection(pshd->pmatClip, pos, &sRadius);
    qword dpos;
    float gVis = GetSphereVisibility(posCam, pos, pmodel->owner, sRadius, &dpos);
    if (gVis == 0.0f)
        return;
    pmodel->unknown04 = g_clock.time;
    if (pmodel->relightStamp != g_sw.relightStamp)
        SubmitGlobsetRelight(pmodel);
    int i;
    for (i = 0; i < pmodel->cpsaa; i++)
    {
        SAA *psaa = pmodel->apsaa[i];
        if (ShouldUpdateSaa__FP3SAA(psaa))
        {
            AsSo(psaa)->Add();
        }
    }
    int fAlt = 0;
    if (pmodel->grf50 & 0x400000000UL)
        fAlt = g_sw.numShownWidgets == 0;
    for (i = 0; i < pmodel->numPieces; i++)
    {
        pdesc = &pmodel->subMeshes[i];
        GLOB *psub = &pmodel->pieces[i];
        SUBGLOBX *psph = &pmodel->asubglobx[i];
        VECTOR4 posSph;
        MATRIX4 matModel;
        if (psub->grf & 0x800)
            continue;
        if (psub->grf & 0x1000)
        {
            CopyAb__FPvT0Ui(&matModel, (char *)psub->pqwChain + 0x14, sizeof(MATRIX4));
            if (pmatWorld)
                matModel = MulMat(*pmatWorld, matModel);
        }
        else
        {
            MATRIX4 *pmatParent = psub->unknown08;
            MATRIX4 mat;
            CopyMat(mat, psub->transform->worldMatrix);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            if (pmatWorld)
                CopyMat(mat, MulMat(*pmatWorld, mat));
            matModel = mat;
        }
        CopyAb__FPvT0Ui(&posSph, psph, 12);
        qword posSub = TransformPoint(matModel, *(qword *)&posSph);
        float sRadiusSub = GetMaxRowLengthMatrix3(&matModel) * psph->sRadius;
        if ((psub->grf & 7) == 2 && pshd->pmatClip)
            posSub = ExpandSphereForProjection(pshd->pmatClip, posSub, &sRadiusSub);
        float gVisSub;
        qword dposSub;
        if (!psub->abnd)
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
            if (!IsWithinCameraRenderDistance(&g_cm, dposSub, psph->maxRenderDistance, &gVisSub))
                continue;
        }
        uchar *pbStart = D_002DEA28.m_pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->cqwPacket | 0x40000000) | ((ulong)(int)pshd->pbPacket << 32);
            D_002DEA28.m_pb += 0x10;
        }
        ulong *ptag;
        switch (psub->grf & 7)
        {
        case 2:
            if (!pshd->pbVolume)
                continue;
            *(ulong *)D_002DEA28.m_pb = (ulong)(int)(pshd->cqwVolume | 0x40000000) | ((ulong)(uint)pshd->pbVolume << 32);
            D_002DEA28.m_pb += 0x10;
            ptag = (ulong *)D_002DEA28.m_pb;
            *ptag = 0x10000000;
            D_002DEA28.m_pb += 0x10;
            break;
        case 3:
        {
            ptag = (ulong *)D_002DEA28.m_pb;
            *ptag = 0x10000000;
            float g = SqDist(posCam, posSub);
            float sNear = sRadiusSub + g_cm.nearCornerDistance;
            D_002DEA28.m_pb += 0x10;
            int fNear = g < sNear * sNear;
            UnpackWords(0x6001C012, &fNear, 1);
            break;
        }
        default:
            ptag = (ulong *)D_002DEA28.m_pb;
            *ptag = 0x10000000;
            D_002DEA28.m_pb += 0x10;
            break;
        }
        if (psub->abnd)
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
        vecParams.z = pmodel->unknown50;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->selectedShader : 0;
        UnpackQws<0x6C01C008>(&vecParams, 4);
        if (psub->abnd)
            UnpackSubglobBones(pmodel, pdesc, psub);
        WRBG *pskin = psub->pwrbg;
        if (pskin)
        {
            MATRIX4 matInv;
            InvertMatrix4Rigid(&matInv, &matModel);
            uint ib = pskin->iqw;
            for (int imat = 0; imat < pskin->cmat; imat++)
            {
                if (pskin->fDpos)
                {
                    MATRIX4 mat;
                    int ibMat = imat * (int)sizeof(MATRIX4Q) + SLY2_OFFSETOF(WR, warpMatrices);   // &pwr->warpMatrices[imat] changes the code
                    mat = MulMat(matInv, *(MATRIX4 *)((char *)pskin->pwr + ibMat));
                    UnpackQws(ib | 0x6C04C000, &mat, 16);
                    ib += 4;
                }
                if (pskin->fDuv)
                {
                    UnpackQws(ib | 0x6C04C000, &pskin->pwr->uvMatrices[imat], 16);
                    ib += 4;
                }
            }
        }
        {
            VIFS *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->m_pb; (uptr)pbPad & 0xF; pbPad = pdma->m_pb)
            {
                *(uint *)pbPad = 0;
                pdma->m_pb += 4;
            }
        }
        *(uint *)ptag |= ((int)(D_002DEA28.m_pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.m_pb = 0x60000000;
        D_002DEA28.m_pb += 0x10;
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
            prpl->depth = pdesc->unknown04;
        else
            StoreSquaredLengthBits(*(int *)&prpl->depth, dposSub);
        prpl->pbCall = pbStart;
        prpl->pvCall = psub->pqwChain;
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
// The model is so.h's GLOBSET (as in RenderSoGlobset); ALOV is rtcam.cpp's render-target camera (RTCAM, TU-private there).
struct ALOV
{
    char pad[0x104];
    int cpshd;
    void *apshd[1];
};
extern MATRIX4 D_002A2D30 __asm__("D_002A2D30");

void *func_002333B4(void *pv, int b, uint cb) __asm__("memset");
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb) __asm__("memcpy");
void BuildLightPackets(SW *pg, int c, void **apv, SOBOUNDS *pshd) __asm__("BuildLightPackets");
int ShouldUpdateSaa__FP3SAA(SAA *psaa) __asm__("ShouldUpdateSaa__FP3SAA");
int GetRenderTargetTexture(ALOV *palo, int n) __asm__("GetRenderTargetTexture");
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc) __asm__("InvertMatrix4Rigid");
qword GetLoWorldPos(ALOV *palo) __asm__("GetLoWorldPos");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");

// Return the 4x4 matrix product matA * matB.
static inline MATRIX4 MulMat(const MATRIX4 &matA, const MATRIX4 &matB) return mat
{
    __ml__FRC7MATRIX4T0(&mat, &matA, &matB);
}
// Copy a 4x4 matrix.
static inline void CopyMat(MATRIX4 &matDst, const MATRIX4 &matSrc)
{
    func_0023320C(&matDst, &matSrc, sizeof(MATRIX4));
}
// Transform point qwPos by the 4x4 matrix mat.
static inline qword TransformPoint(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
// Store the bits of qwV's squared length (xyz) into gDst.
static inline void StoreSquaredLengthBits(int &gDst, qword qwV)
{
    qword qwSq, qwOne;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    // the "m"/"r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
    __asm__("qmfc2.ni %0, %1" : "=r,m"(gDst) : "j,!r"(qwSq));
}
// Bits of qwV's squared length (xyz) as an int.
static inline int GetSquaredLengthBits(qword qwV)
{
    qword qwSq, qwOne;
    int g;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwSq));
    return g;
}
// Copy cw words (a multiple of 4) from pwSrc to pwDst one quadword at a time.
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
// Append the VIF code TAG and then cw words from pv to the DMA packet D_002DEA28.
template <uint TAG> static inline void UnpackQws(const void *pv, int cw)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = TAG;
    pdma->m_pb += 4;
    CopyQws((uint *)pdma->m_pb, (const uint *)pv, cw);
    pdma->m_pb += cw * 4;
}
// Append the VIF code tag and then cw words from pv to the DMA packet D_002DEA28.
static inline void UnpackQws(uint tag, const void *pv, int cw)
{
    VIFS *pdma = &D_002DEA28;
    *(uint *)pdma->m_pb = tag;
    pdma->m_pb += 4;
    CopyQws((uint *)pdma->m_pb, (const uint *)pv, cw);
    pdma->m_pb += cw * 4;
}
// Builds the VIF packet for every submodel of a model (model matrix, optional bone and skin matrices) and queues it
// with its sort distance. (inferred)
extern "C" void BuildModelDrawPackets(GLOBSET *pmodel, ALOV *palo)
{
    SOBOUNDS shd;
    MATRIX4 matModel;
    SOBOUNDS *pshd;
    if (palo->cpshd)
    {
        func_002333B4(&shd, 0, sizeof(SOBOUNDS));
        pshd = &shd;
        BuildLightPackets(&g_sw, palo->cpshd, palo->apshd, &shd);
    }
    else
    {
        pshd = 0;
    }
    int i;
    for (i = 0; i < pmodel->cpsaa; i++)
    {
        SAA *psaa = pmodel->apsaa[i];
        if (ShouldUpdateSaa__FP3SAA(psaa))
        {
            AsSo(psaa)->Add();
        }
    }
    for (i = 0; i < pmodel->numPieces; i++)
    {
        SUBGLOB *pdesc = &pmodel->subMeshes[i];
        GLOB *psub = &pmodel->pieces[i];
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
            CopyAb__FPvT0Ui(&matModel, (char *)psub->pqwChain + 0x14, sizeof(MATRIX4));
        }
        else
        {
            MATRIX4 *pmatParent = psub->unknown08;
            MATRIX4 mat;
            CopyMat(mat, psub->transform->worldMatrix);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            matModel = mat;
        }
        qword dpos;
        if (psub->ppose)
        {
            qword pos = *(qword *)&psub->ppose->pos;
            dpos = VuSub(pos, GetLoWorldPos(palo));
        }
        else
        {
            VECTOR4 pos;
            CopyAb__FPvT0Ui(&pos, &pmodel->asubglobx[i], 12);
            dpos = TransformPoint(matModel, *(qword *)&pos);
        }
        uchar *pbStart = D_002DEA28.m_pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->cqwPacket | 0x40000000) | ((ulong)(int)pshd->pbPacket << 32);
            D_002DEA28.m_pb += 0x10;
        }
        ulong *ptag = (ulong *)D_002DEA28.m_pb;
        *ptag = 0x10000000;
        D_002DEA28.m_pb += 0x10;
        if (psub->abnd)
            UnpackQws<0x6C04C000>((uint *)&D_002A2D30, 16);
        else
            UnpackQws<0x6C04C000>((uint *)&matModel, 16);
        VECTOR4 vecParams;
        vecParams.x = 1.0f;
        vecParams.y = 1.0f;
        vecParams.z = pmodel->unknown50;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->selectedShader : 0;
        UnpackQws<0x6C01C008>((uint *)&vecParams, 4);
        if (psub->abnd)
        {
            for (int ibone = 0; ibone < pdesc->cbnd; ibone++)
            {
                BND *pbone = &psub->abnd[ibone];
                MATRIX4 mat;
                if (pbone->pxf)
                    mat = MulMat(pbone->pxf->worldMatrix, pbone->matInv);
                else
                    mat = pbone->matInv;
                UnpackQws((ibone * 4 + pdesc->iqwBone) | 0x6C04C000, (uint *)&mat, 16);
            }
        }
        WRBG *pskin = psub->pwrbg;
        if (pskin)
        {
            MATRIX4 matInv;
            InvertMatrix4Rigid(&matInv, &matModel);
            uint ib = pskin->iqw;
            for (int imat = 0; imat < pskin->cmat; imat++)
            {
                if (pskin->fDpos)
                {
                    MATRIX4 mat;
                    MATRIX4 *pmatInv = &matInv;
                    int ibMat = imat * (int)sizeof(MATRIX4Q) + SLY2_OFFSETOF(WR, warpMatrices);   // WR::warpMatrices[imat]
                    mat = MulMat(*pmatInv, *(MATRIX4 *)((char *)pskin->pwr + ibMat));
                    UnpackQws(ib | 0x6C04C000, (uint *)&mat, 16);
                    ib += 4;
                }
                if (pskin->fDuv)
                {
                    UnpackQws(ib | 0x6C04C000, (uint *)&pskin->pwr->uvMatrices[imat], 16);
                    ib += 4;
                }
            }
        }
        {
            VIFS *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->m_pb; (uptr)pbPad & 0xF; pbPad = pdma->m_pb)
            {
                *(uint *)pbPad = 0;
                pdma->m_pb += 4;
            }
        }
        *(uint *)ptag |= ((int)(D_002DEA28.m_pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.m_pb = 0x60000000;
        D_002DEA28.m_pb += 0x10;
        RPL *prpl = AllocRenderListEntry();
        prpl->kind = kind;
        if (psub->grf & 0x400)
            prpl->depth = pdesc->unknown04;
        else
            StoreSquaredLengthBits(*(int *)&prpl->depth, dpos);
        prpl->pbCall = pbStart;
        prpl->pvCall = psub->pqwChain;
    }
}
} // namespace M_BuildModelDrawPackets

// BATCH: one draw batch of a type-0 sub-glob (0x40), built by LoadSubglobType0FromBrx; VERTREC: its 0x14-byte vertex
// sort record (CompareSortRecs orders them by batch and vertex).
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
      uchar *pbColorA;   /* the batch's colour unpack data in the aux chain (UnpackHelper) */
      uchar *pbColorB;
      uchar *pbAlpha;
      uchar *pbAlphaUnpack;   /* its alpha unpack data (0: none) */
      uchar *pbRgb;
      uchar *pbNorm;
      uchar *pbWeights;
};
struct VERTREC
{
      int ivtxFile;
      BATCH *pbatchFirst;
      int ivtxFirst;
      BATCH *pbatch;
      int ivtx;
};
// qsort comparator for the vertex records, grouping vertices by batch: by the first batch, the first vertex, the batch,
// then the vertex.
extern "C" int CompareSortRecs(VERTREC *prec0, VERTREC *prec1)
{
    int d = prec0->pbatchFirst - prec1->pbatchFirst;
    if (d != 0)
        return d;
    d = prec0->ivtxFirst - prec1->ivtxFirst;
    if (d != 0)
        return d;
    d = prec0->pbatch - prec1->pbatch;
    if (d != 0)
        return d;
    return prec0->ivtx - prec1->ivtx;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobInstancesFromBrx {
void LoadSubglobInstanceFromBrx(GLOBSET *pglobset, SUBGLOB *psubglobBase, GLOB *pglobBase, SUBGLOBX *psubglobxBase, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBX *psubglobx, int cbatch, int *acbatch, BATCH **apbatch, CBinaryInputStream *pbis) __asm__("LoadSubglobInstanceFromBrx");
// For each extra instance slot of a sub-glob, copies the base descriptor and reads the instance record (0x169AC8)
// (inferred)
// Also known as: LoadGlobSubmeshesFromBrx (upstream readable / Sly 1 name;
//   LoadGlobSubmeshesFromBrx__FP7GLOBSETP18CBinaryInputStreamPvT2iiiii).
extern "C" void LoadSubglobInstancesFromBrx(GLOBSET *pglobset, SUBGLOB *asubglob, GLOB *aglob, SUBGLOBX *asubglobx, int cSubExtra, int cbatch, int *acbatch, BATCH **apbatch, CBinaryInputStream *pbis)
{
    int i;
    for (i = 0; i < cSubExtra; i++) {
        SUBGLOB *psubglob = &asubglob[i + 1];
        GLOB *pglob = &aglob[i + 1];
        SUBGLOBX *psubglobx = asubglobx ? asubglobx + i + 1 : 0;
        LoadSubglobInstanceFromBrx(pglobset, asubglob, aglob, asubglobx, psubglob, pglob, psubglobx, cbatch, acbatch, apbatch, pbis);
    }
}
} // namespace M_LoadSubglobInstancesFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobInstanceFromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
struct REQUEST {
    int bytes, alignment;
    // Allocation request: byte count and alignment.
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
typedef BATCH FACE;   /* the vertex batches (BATCH above; LoadGlobsetFromBrx's apbatch) */
typedef char LinkSize[sizeof(LINK)==6?1:-1];
typedef char LinkSetSize[sizeof(LINKS)==8?1:-1];
typedef char BindingSize[sizeof(BINDING)==12?1:-1];
typedef char LINKS_links_Offset[FIELD_OFFSET(LINKS,links)==4?1:-1];
typedef char BINDING_sets_Offset[FIELD_OFFSET(BINDING,sets)==4?1:-1];
typedef char BINDING_indices_Offset[FIELD_OFFSET(BINDING,indices)==8?1:-1];
typedef char LINK_second_Offset[FIELD_OFFSET(LINK,second)==2?1:-1];
typedef char LINK_kind_Offset[FIELD_OFFSET(LINK,kind)==4?1:-1];

char S8Read__18CBinaryInputStream(STREAM*) __asm__("S8Read__18CBinaryInputStream");
uint U32Read__18CBinaryInputStream(STREAM*) __asm__("U32Read__18CBinaryInputStream");
short S16Read__18CBinaryInputStream(STREAM*) __asm__("S16Read__18CBinaryInputStream");
int S32Read__18CBinaryInputStream(STREAM*) __asm__("S32Read__18CBinaryInputStream");
void ReadMatrix4__18CBinaryInputStreamP7MATRIX4(STREAM*,MATRIX4*) __asm__("ReadMatrix4__18CBinaryInputStreamP7MATRIX4");
void*AllocFromWorldHeap(const REQUEST&) __asm__("AllocFromWorldHeap");
void*AllocCopyFromWorldHeap(const REQUEST&,void*) __asm__("AllocCopyFromWorldHeap");
STACKSCOPE*STACKSCOPE_ctor(STACKSCOPE*) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(STACKSCOPE*,int) __asm__("_$_10STACKSCOPE");
BINDING*LoadClutsetFromBrx(GLOBSET*,STREAM*) __asm__("LoadClutsetFromBrx");
MATRIX4*__ml__FRC7MATRIX4T0(MATRIX4*,const MATRIX4*,const MATRIX4*) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");

// Allocate bytes, 16-byte aligned, from the world heap.
static inline void *Allocate(int bytes)
{
    return AllocFromWorldHeap(REQUEST(bytes, 16));
}
// Reallocate old as a copy of bytes bytes, 16-byte aligned, on the world heap.
static inline void *Reallocate(int bytes, void *old)
{
    return AllocCopyFromWorldHeap(REQUEST(bytes, 16), old);
}
// Reads one sub-glob instance: transform owner lookup (vtable slot 51), matrix, flags, optional binding table,
// relocates packet data. (inferred)
// Also known as: LoadGlobSubmeshFromBrx (upstream readable / Sly 1 name;
extern "C" void LoadSubglobInstanceFromBrx(GLOBSET *p, const SUBGLOB *sourceD, const GLOB *sourceE, const SUBGLOBX *sourceSphere,
                                           SUBGLOB *desc, GLOB *entry, SUBGLOBX *sphere, int groups, const int *counts,
                                           FACE **faces, STREAM *stream)
{
    MATRIX4 matrix;
    (void)S8Read__18CBinaryInputStream(stream);
    *desc = *sourceD;
    *entry = *sourceE;
    if (sphere) {
        *sphere = *sourceSphere;
        sphere->flags = U32Read__18CBinaryInputStream(stream);
    }
    {
        entry->transform = AsSo(p->owner)->GetXfFromIndex(S16Read__18CBinaryInputStream(stream));
    }
    if (entry->grf & 0x1000)
        ReadMatrix4__18CBinaryInputStreamP7MATRIX4(stream, &matrix);
    else {
        entry->unknown08 = (MATRIX4 *)Allocate(sizeof(MATRIX4));
        ReadMatrix4__18CBinaryInputStreamP7MATRIX4(stream, entry->unknown08);
    }
    entry->oid = (OID)S32Read__18CBinaryInputStream(stream);
    entry->isHidden = S8Read__18CBinaryInputStream(stream);
    STACKSCOPE scope;
    BINDING *binding = LoadClutsetFromBrx(p, stream);
    if (!(desc->unknown02 & 1) && desc->type == 0) {
        char *old = (char *)entry->pqwChain;
        char *data = (char *)Reallocate(desc->cqw * 16, old);
        int delta = data - old;
        entry->pqwChain = (QW *)data;
        entry->pbChain += delta;
        if (entry->grf & 0x1000) {
            MATRIX4 product, temp;
            __ml__FRC7MATRIX4T0(&temp, &entry->transform->worldMatrix, &matrix);
            product = temp;
            CopyAb__FPvT0Ui((char *)entry->pqwChain + 0x14, &product, 0x40);
        }
        if (binding) {
            for (int g = 0; g < groups; g++) {
                int count = counts[g];
                FACE *set = faces[g];
                for (int i = 0; i < count; i++) {
                    FACE *face = &set[i];
                    if (face->grf & 0x20) {
                        uptr base = (uptr)entry->pbChain;   /* int arithmetic: a char * sum swaps the addu operands */
                        unsigned short *first = (unsigned short *)((uptr)face->pbColorA + base);
                        unsigned short *second = (unsigned short *)((uptr)face->pbColorB + base);
                        unsigned char *kind = (unsigned char *)((uptr)face->pbAlphaUnpack + base);
                        for (int j = 0; j < face->cvtx; j++) {
                            int index = face->aivtx[j];
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
namespace M_CopyRelocateDmaChain {
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
QW *AllocCopyFromWorldHeap(ALLOCREQ *preq, QW *aqw) __asm__("AllocCopyFromWorldHeap");
// Copies a VIF/DMA packet into the SW heap and optionally relocates DMA REF addresses (tag id 4) by a delta.
// (inferred)
extern "C" void CopyRelocateDmaChain(SUBGLOB *px, GLOB *py, int cqw, QW *aqwSrc, int fReloc, int nNew, int nOld)
{
    ALLOCREQ req;
    QW *aqw;
    req.cb = cqw << 4;
    req.nKind = 0x10;
    px->cqw = cqw;
    aqw = AllocCopyFromWorldHeap(&req, aqwSrc);
    py->pqwChain = aqw;
    py->pbChain += (char *)aqw - (char *)aqwSrc;
    if (fReloc) {
        int dn = nNew - nOld;
        QW *pqw = aqw;
        while (pqw < py->pqwChain + px->cqw) {
            switch ((int)(pqw->aun[0] >> 28)) {
            case 4:
                pqw->aun[1] += dn;
                pqw++;
                break;
            case 1:
                pqw += pqw->aus[0] + 1;
                break;
            case 6:
                pqw = py->pqwChain + px->cqw;
                break;
            }
        }
    }
}
} // namespace M_CopyRelocateDmaChain

// Keeps its per-function views in a namespace of its own (see the file header).

namespace M_LoadSubglobType0FromBrx {
// LoadSubglobType0FromBrx = LoadSubglobType0FromBrx (name from Lilly's personal decomp, decomp/src/P2/glob.c T1082 draft). MATCH (T1701).
// Built on lilly4 (T1697) / lilly3 (T1693); see notes/160000.md for the tricks that matched it, the last ones being:
//  - the header flags word (BATCHHDR0 +0x8) is a one-member struct, set from an aggregate initializer in the ibatch
//    switch: the clear-then-set keeps loop.c from hoisting the constant with the other 1s;
//  - the lit-batch count is Min(0x27, cvtx - iofs) through an inline Min (no REG_EQUAL on n's first set);
//  - ppart->iqwVtx is re-read for iqwEnd/iqwOwn; pg[0] = pg[1] = pg[2] = -1.0f.
/* the word-append helper as this function was matched with: store, then advance (vifs.h's AddDmaInt advances first,
   which reorders the code here; named apart: ADL would find both) */
// Append one int to the DMA packet.
inline void AddDmaIntStore(DMAS *pdmas, int val)
{
    *(int *)pdmas->m_pb = val;
    pdmas->m_pb += 4;
}
void InitSl(SL *psl, int ibSle) __asm__("InitSl");
void AppendSlEntry(SL *psl, void *pv) __asm__("AppendSlEntry");
int IsBasicDerivedFrom(BASIC *pbasic, CID cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
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
void CopyAb(void *pvDst, void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");
struct UVF
{
    float u, v;
};
extern char D_00000000[] __asm__("D_00000000");

void LoadSubglobType0FromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBLOAD *psgl, SUBGLOBX *psubglobx, int cSubExtra, CBinaryInputStream *pbis) __asm__("LoadSubglobType0FromBrx");

// Allocate cb bytes, 16-byte aligned, from the world heap.
static inline void *PvAllocSw(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwImpl(&swalloc);
}
// Allocate cb bytes, 16-byte aligned, from the world heap and copy pvBase into them.
static inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwCopyImpl(&swalloc, pvBase);
}
SHD *PshdFromIshd(int ishd) __asm__("GetShdByIndex");
int AddVifUnpackShdp(SHD *pshd, int ishdp, int iqw, VIFS *pvifs) __asm__("AddShaderVifUnpack");
extern uint D_002BE540[] __asm__("D_002BE540");
extern int D_002BE558[] __asm__("D_002BE558");
extern QW D_002DEB60 __asm__("D_002DEB60");
void CalcRecipFitParams(float gScale, CM *pcm, float *pgOut) __asm__("BuildCmFgfn");
CLUTSET *LoadClutSetFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis) __asm__("LoadClutsetFromBrx");
void LoadSubglobInstancesFromBrx(GLOBSET *pglobset, SUBGLOB *psubglob, GLOB *pglob, SUBGLOBX *psubglobx, int cSubExtra, int cbatch, int *acbatch, BATCH **apbatch, CBinaryInputStream *pbis) __asm__("LoadSubglobInstancesFromBrx");
int CompareVertRec(const void *pv1, const void *pv2) __asm__("CompareSortRecs");
void CopyDmaRefsChainSw(SUBGLOB *psubglob, GLOB *pglob, int cqw, void *pvBase, int cqwMain, char *pbNew, char *pbOld) __asm__("CopyRelocateDmaChain");

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) __asm__("qsort");

extern void ReadCb12__18CBinaryInputStreamPv(CBinaryInputStream *pbis, void *pv) __asm__("ReadCb12__18CBinaryInputStreamPv");
extern void ReadCb8__18CBinaryInputStreamPv(CBinaryInputStream *pbis, void *pv) __asm__("ReadCb8__18CBinaryInputStreamPv");
void FixupRgba(RGBA *prgba) __asm__("ColorCorrectRgba");


// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *AllocFromStack16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocStackImpl(&swalloc);
}
// Allocate cb zeroed bytes, 16-byte aligned, from the frame stack.
static inline void *AllocClearedFromStack16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocStackClearImpl(&swalloc);
}
// Allocate cb zeroed bytes, 16-byte aligned, from the world heap.
static inline void *AllocClearedFromWorldHeap16(int cb)
{
    SWALLOCC swalloc(cb, 0x10);
    return PvAllocSwClearImpl(&swalloc);
}
extern SHD *FindShader(int oid) __asm__("FindShader");
extern int AddShaderVifUnpackQword(SHD *pshd, int ishdp, int iqw, VIFS *pvifs) __asm__("AddShaderVifUnpackQword");
extern int AddSaaCopyRecord(SAA *psaa, int iqw, VIFS *pvifs) __asm__("AddSaaCopyRecord");
extern void RelocateShaderUploadRecords(int iFirst, int iEnd, int cb, void *pvOld, void *pvNew) __asm__("RelocateShaderUploadRecords");
extern int D_003DA790 __asm__("D_003DA790");


extern int D_002BE580 __asm__("D_002BE580");
extern int D_002DEB54 __asm__("D_002DEB54");
extern const float D_002A52A0 __asm__("D_002A52A0");   /* defined below where the ELF has it (.rodata order) */
extern char D_00003628[] __asm__("D_00003628");
extern char D_000037A8[] __asm__("D_000037A8");
extern char D_00002EF8[] __asm__("D_00002EF8");
extern char D_00002E20[] __asm__("D_00002E20");
extern char D_00002F80[] __asm__("D_00002F80");
extern char D_00002E68[] __asm__("D_00002E68");
extern char D_00002FC8[] __asm__("D_00002FC8");
extern char D_00002EB0[] __asm__("D_00002EB0");
extern char D_00003358[] __asm__("D_00003358");
extern char D_00002EE8[] __asm__("D_00002EE8");
// Number of texture words for a sub-glob: its binding count, capped at 4.
static inline int CwtFromSubglob(SUBGLOB *psubglob)
{
    int cbnd = psubglob->cbnd;
    int cwt = 4;
    if (cbnd < 4)
        cwt = cbnd;
    return cwt;
}
// If *ppb points into the old block [pbOld, pbOld+cb), rebase it into the new block at pbNew.
static inline void RelocPpv(char *pbOld, int cb, char *pbNew, char **ppb)
{
    if (ppb != 0 && *ppb >= pbOld && *ppb < pbOld + cb)
        *ppb += pbNew - pbOld;
}
// Return pb rebased from the old block [pbOld, pbOld+cb) into the new block at pbNew (unchanged if outside).
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
// Also known as: LoadGlobMesh0FromBrx (upstream readable / Sly 1 name;
//   LoadGlobMesh0FromBrx__FP7GLOBSETP18CBinaryInputStreamP4GLOBP11SUBGLOBLOADP8SUBGLOBXiT1).
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
    vifs.AllocSw(0x2000);
    QW *pqwChain = (QW *)vifs.m_pb;
    pglob->iUnkFirst = D_003DA790;
    if (psubglob->unknown02 & 1)
    {
        vifs.AddDmaCnt();
        pglobset->unknownBoundsFlag50 = 1;
    }
    else
    {
        vifsAux.AllocSw(0x800);
        if (IsBasicDerivedFrom((BASIC *)pglobset->owner, (CID)0x31) && !(psubglob->unknown02 & 4) && (uchar)(psubglob->unknown00 - 7) >= 2 /* not 7 or 8: the unsigned range test is what compiles to the target */ &&
            psubglob->unknown04 == D_002A52A0 && g_sw.unknown3E40 == 0)
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
    CalcRecipFitParams(psgl->fogIntensity, &g_cm, pgFit);

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
        vifsLit.AllocSw(0x2000);
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
            SAA *psaaGlob = pglob->psaa;
            if (psaaGlob != 0)
            {
                if (psaaGlob->DoesMatchOid(&pbatch->pshd->oid))
                    pbatch->psaa = pglob->psaa;
            }
            if (pbatch->psaa == 0)
            {
                SAA *psaaShd = pbatch->pshd->psaa;
                if (psaaShd != 0)
                {
                    if (psaaShd->DoesMatchOid(&pbatch->pshd->oid))
                        pbatch->psaa = pbatch->pshd->psaa;
                }
            }
            if (pbatch->psaa != 0)
            {
                if (pbatch->psaa->grf & 1)
                    pbatch->grf |= 0x10;
                if (pbatch->psaa->grf & 2)
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
            ppart->oidAltSat = pbatch->pshd->oidAltSat;
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
                        int ioid = pglobset->numShaderOids;
                        while (--ioid >= 0)
                            iqw += AddShaderVifUnpackQword(FindShader(pglobset->shaderOids[ioid]), ishdp, iqw, &vifs);
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
                vifsAux.UnpackHelper(UPK_V4_5, pbatch->cvtx, ppart->iqwVtx + 1, 0, (uint **)&pbatch->pbColorA);
                if ((pbatch->grf & 2) && pbatch->pshd->shdk != 3)
                    vifsAux.UnpackHelper(UPK_V4_5, pbatch->cvtx, ppart->iqwVtx + 2, 0, (uint **)&pbatch->pbColorB);
                if (fAlpha != 0 || pbatch->pbColorB != 0)
                {
                    vifsAux.AddVifStmask(0x3f3f3f3f);
                    if (fAlpha != 0)
                        vifsAux.UnpackHelper(UPK_S8, pbatch->cvtx, ppart->iqwVtx + 1, 0, (uint **)&pbatch->pbAlpha);
                    if (pbatch->pbColorB != 0)
                        vifsAux.UnpackHelper(UPK_S8, pbatch->cvtx, ppart->iqwVtx + 2, 0, (uint **)&pbatch->pbAlphaUnpack);
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
                    RGBA5551 *pcol = (RGBA5551 *)pbatch->pbColorA;
                    uchar *pbAlpha = pbatch->pbAlpha;
                    for (int iv = 0; iv < pbatch->cvtx; iv++)
                    {
                        RGBA rgbaA = argbaFile[pbVtxCur[iv]];
                        SHDRGBA rgbaB = pbatch->pshd->rgba;
                        uchar a = (rgbaA.alpha * rgbaB.a + 0xff) / 0x1fe;
                        RGBA5551 *pc = pcol++;
                        RGBA5551 col;
                        col.g = (uchar)((rgbaA.green * rgbaB.g + 0xff) / 256) >> 3;
                        col.r = (uchar)((rgbaA.red * rgbaB.r + 0xff) / 256) >> 3;
                        col.b = (uchar)((rgbaA.blue * rgbaB.b + 0xff) / 256) >> 3;
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
                            pun[5] = (uint)(pbatch->pbColorA + iofs * 2);
                            pun[6] = (uint)(pbatch->pbColorB + iofs * 2);
                            if (fAlpha != 0)
                                pun[8] = (uint)pbatch->pbAlpha + iofs;
                            else
                                pun[8] = 0;
                            pun[9] = (uint)(pbatch->pbAlphaUnpack + iofs);
                            *(float *)&pun[7] = (float)pbatch->pshd->oidAltSat * 0.000244140625f;
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
                                    SHDRGBA rgbaB = pbatch->pshd->rgba;
                                    rgba.red = (rgba.red * rgbaB.r + 0xff) / 256;
                                    rgba.green = (rgba.green * rgbaB.g + 0xff) / 256;
                                    rgba.blue = (rgba.blue * rgbaB.b + 0xff) / 256;
                                    rgba.alpha = (rgba.alpha * rgbaB.a + 0xff) / 0x1fe;
                                    *prgbaCol++ = rgba;
                                }
                            }
                            vifsLit.AddVifMscal(D_00000000 + 0x10);
                            AddDmaIntStore(&vifsLit, 0x90000000);
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
                    float *pg = (float *)(iv * 0x10 + (uptr)pbatch->pvPos);
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
                    ((ushort *)pbatch->pbColorA)[iv] = pcc->ucA;
                    ((ushort *)pbatch->pbColorB)[iv] = pcc->ucB;
                    ((uchar *)pbatch->pbAlphaUnpack)[iv] = pcc->bAlpha;
                    if (pbatch->pbAlpha != 0)
                    {
                        RGBA rgbaA = argbaFile[ivtxFile];
                        SHDRGBA rgbaB = pbatch->pshd->rgba;
                        pbatch->pbAlpha[iv] = (rgbaA.alpha * rgbaB.a + 0xff) / 0x1fe;
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
                int bRed = rgbaA.red;
                int bGreen = rgbaA.green;
                int bBlue = rgbaA.blue;
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
                    SHDRGBA rgbaB = pbatchFirst->pshd->rgba;
                    uchar *pbC = pbatchFirst->pbRgb + (ivtxFirst - pbatchFirst->ivtxOwn) * 4;
                    pbC[0] = (rgbaA.red * rgbaB.r + 0xff) / 512;
                    pbC[1] = (rgbaA.green * rgbaB.g + 0xff) / 512;
                    pbC[2] = (rgbaA.blue * rgbaB.b + 0xff) / 512;
                    pbC[3] = (rgbaA.alpha * rgbaB.a + 0xff) / 0x1fe;
                }
                if (psubglob->cbnd != 0)
                {
                    VECTOR *pposDst = (VECTOR *)(ivtxFirst * 0x10 + (uptr)pbatchFirst->pvPos);
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
        pglob->pqwChain = (QW *)pbNew;
    }
    else
    {
        vifs.Align(0);
        vifsAux.AddDmaRefs((vifs.m_pb - (uchar *)pqwChain) >> 4, pqwChain);
        vifsAux.AddDmaRet();
        vifs.DetachCopySw(&cqwMain, (QW **)&pbOld, (QW **)&pbNew);
        if (pglob->psaa != 0)
        {
            pglob->psaa->cqwMain = cqwMain;
            pglob->psaa->pvMain = pbNew;
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
        pglobset->relightStamp = g_sw.relightStamp;
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
        pbatchrecs->abatchrec = (BATCHREC *)PvAllocSw(cbatchAll * (int)sizeof(BATCHREC));
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
extern void *AllocClearedFromStack(ALLOCREQ *preq) __asm__("AllocClearedFromStack");
extern void ColorCorrectRgba16(ushort *pus) __asm__("ColorCorrectRgba16");
// Fill the request and allocate cb zeroed bytes of kind nKind from the frame stack.
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return AllocClearedFromStack(preq);
}
// Reads a sub-glob's CLUTSET (sly2/glob.h): u16-counted palettes of u16/u16/u8 colours, then the per-batch palette indices (0x1ED1A8)
// Also known as: LoadIdxTabFromBrx (upstream readable / Sly 1 name; LoadIdxTabFromBrx__FP18CBinaryInputStreamT0).
extern "C" CLUTSET *LoadClutsetFromBrx(void *pv, CBinaryInputStream *pbis)
{
    CLUTSET *pt = 0;
    int c = pbis->U16Read();
    if (c) {
        ALLOCREQ req;
        pt = (CLUTSET *)PvAllocReq(&req, sizeof(CLUTSET), 0x10);
        pt->cpal = c;
        pt->apal = (CLUTPAL *)PvAllocReq(&req, c * sizeof(CLUTPAL), 0x10);
        for (int i = 0; i < pt->cpal; i++) {
            CLUTPAL *pe = &pt->apal[i];
            int n = pbis->U16Read();
            pe->acolor = (CLUTCOLOR *)PvAllocReq(&req, n * sizeof(CLUTCOLOR), 0x10);
            for (int j = 0; j < n; j++) {
                CLUTCOLOR *p6 = &pe->acolor[j];
                ushort *pb = &p6->ucB;
                p6->ucA = pbis->U16Read();
                p6->ucB = pbis->U16Read();
                p6->bAlpha = pbis->U8Read();
                ColorCorrectRgba16(&p6->ucA);
                ColorCorrectRgba16(pb);
            }
        }
        int m = pbis->U16Read();
        pt->aipal = (int *)PvAllocReq(&req, m * 4, 0x10);
        for (int k = 0; k < m; k++)
            pt->aipal[k] = pbis->S16Read();
    }
    return pt;
}
} // namespace M_LoadClutsetFromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType2FromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
struct SPHERE;struct FACE;
struct HEADER {
    ushort flags, base, pad4, pad6, position, weights, color, end, extra, extra2;
    ushort segments[2], bones[4];
};
struct REQUEST {
    int bytes, alignment;
    // Allocation request: byte count and alignment.
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
};

void*STACKSCOPE_ctor(void*) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&) __asm__("AllocClearedFromWorldHeap");
SHD*GetShdByIndex(int) __asm__("GetShdByIndex");
int AddShaderVifUnpack(SHD*,int,int,VIFS*) __asm__("AddShaderVifUnpack");
void LoadSubglobInstancesFromBrx(GLOBSET*,SUBGLOB*,GLOB*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*) __asm__("LoadSubglobInstancesFromBrx");
void*memset(void*,int,uint) __asm__("memset");
extern QW D_002DEB60 __asm__("D_002DEB60");
extern uint D_002BE540[] __asm__("D_002BE540");
extern UPK D_002BE558[] __asm__("D_002BE558");
extern char D_00000000[] __asm__("D_00000000"),D_00001910[] __asm__("D_00001910"),D_00001A68[] __asm__("D_00001A68");

struct LEASE {
    // Open a stack-allocation scope (STACKSCOPE): allocations after it are freed by Release.
    LEASE() { STACKSCOPE_ctor(this); }
    // Release the stack allocations made since the scope opened.
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
// Allocate bytes zeroed, 16-byte aligned, from the world heap.
static inline void *Allocate(int bytes)
{
    return AllocClearedFromWorldHeap(REQUEST(bytes, 16));
}
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
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
// Also known as: LoadGlobMesh2FromBrx (upstream readable / Sly 1 name;
void LoadSubglobType2FromBrx(GLOBSET *p, SUBGLOB *desc, GLOB *entry, void *, SPHERE *sphere, int groups, CBinaryInputStream *stream) __asm__("LoadSubglobType2FromBrx"); void LoadSubglobType2FromBrx(GLOBSET *p, SUBGLOB *desc, GLOB *entry, void *, SPHERE *sphere, int groups,
                                        CBinaryInputStream *stream)
{
    int i = 0;
    p->grf50 |= 1UL << 33;
    VIFS vifs;
    STACKSCOPE outer;
    vifs.AllocSw(0x2000);
    vifs.AddDmaCnt();
    int count = stream->U8Read();
    desc->cbnd = count;
    entry->abnd = (BND *)Allocate((char)count * sizeof(BND));
    for (; i < desc->cbnd; i++) {
        BND *bone = &entry->abnd[i];
        bone->oid = (OID)stream->S32Read();
        bone->ixf = stream->S16Read();
    }
    int base = 0x1B;
    if (desc->cbnd) {
        desc->iqwBone = base;
        base += desc->cbnd * 4;
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
        if (desc->cbnd)
            header[0] |= 4;
        int current = base;
        header[1] = current;
        SHD *object = GetShdByIndex(stream->U16Read());
        for (int segment = 0; segment < object->cshdp; segment++) {
            HEADER *slot = (HEADER *)((uptr)header + segment * 2);
            slot->segments[0] = current;
            current += AddShaderVifUnpack(object, segment, current, &vifs);
        }
        int vertices = stream->U8Read();
        vifs.AddVifStcycl(1, 2);
        vifs.AddVifUnpack(UPK_V3_32, vertices, stream, current + 1);
        header[4] = current;
        current += vertices * 2;
        header[5] = current;
        if (desc->cbnd) {
            int bones = stream->U8Read();
            int k = 0;
            for (; k < bones; k++) {
                int bone = stream->U8Read();
                HEADER *slot = (HEADER *)((uptr)header + k * 2);
                slot->bones[0] = (ushort)desc->iqwBone + bone * 4;
            }
            int small = bones < 4;
            for (; k < 4; k++) {
                HEADER *slot = (HEADER *)((uptr)header + k * 2);
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
    vifs.DetachCopySw(&desc->cqw, &data, &entry->pqwChain);
    LoadSubglobInstancesFromBrx(p, desc, entry, sphere, groups, 0, 0, 0, stream);
}
} // namespace M_LoadSubglobType2FromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_LoadSubglobType3FromBrx {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
union U {
    qword q;
    float f[4];
};
struct SPHERE;struct FACE;
struct HEADER {
    ushort flags, base, vertices, pad6, pad8, padA, padC, padE, weight, end, segments[2], bones[4];
};
struct REQUEST {
    int bytes, alignment;
    // Allocation request: byte count and alignment.
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
    void *Allocate();
    void *AllocateZero();
};

void*STACKSCOPE_ctor(void*) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&) __asm__("AllocClearedFromWorldHeap");
void*AllocFromStack(const REQUEST&) __asm__("AllocFromStack");
void*func_002333B4(void*,int,uint) __asm__("memset");
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
SHD*GetShdByIndex(int) __asm__("GetShdByIndex");
int AddShaderVifUnpack(SHD*,int,int,VIFS*) __asm__("AddShaderVifUnpack");
void LoadSubglobInstancesFromBrx(GLOBSET*,SUBGLOB*,GLOB*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*) __asm__("LoadSubglobInstancesFromBrx");
/* GetVectorComponentX/E0/E8: the file-scope inline readers */


extern QW D_002DEB60 __asm__("D_002DEB60");
extern uint D_002BE540[] __asm__("D_002BE540");
extern UPK D_002BE558[] __asm__("D_002BE558");
extern char D_00000000[] __asm__("D_00000000"),D_00001910[] __asm__("D_00001910"),D_00001A68[] __asm__("D_00001A68");

struct MARK {
    // Open a stack-allocation scope (STACKSCOPE).
    MARK() { STACKSCOPE_ctor(this); }
    // Release the stack allocations made since the scope opened.
    ~MARK() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
struct LEASE {
    // Open a stack-allocation scope (STACKSCOPE).
    LEASE() { STACKSCOPE_ctor(this); }
    // Release the stack allocations made since the scope opened.
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
// Allocate this request from the frame stack.
inline void *REQUEST::Allocate()
{
    return AllocFromStack(*this);
}
// Allocate this request zeroed from the world heap.
inline void *REQUEST::AllocateZero()
{
    return AllocClearedFromWorldHeap(*this);
}
// Allocate bytes, 16-byte aligned, from the frame stack.
static inline void *Allocate(int bytes)
{
    return REQUEST(bytes, 16).Allocate();
}
// Allocate bytes zeroed, 16-byte aligned, from the world heap.
static inline void *AllocateZero(int bytes)
{
    return REQUEST(bytes, 16).AllocateZero();
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char HEADER_Size[sizeof(HEADER)==32?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char MARK_Size[sizeof(MARK)==1?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
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
// The constant V, laundered through an empty asm so the compiler materialises it in a register where the original
// does.
template <int V> static inline int Flags()
{
    int v = V;
    __asm__ __volatile__("" : "+r"(v));
    return v;
}
// Store the xyz components of value into destination.
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
// Also known as: LoadGlobMesh3FromBrx (upstream readable / Sly 1 name;
extern "C" void LoadSubglobType3FromBrx(GLOBSET *p, SUBGLOB *desc, GLOB *entry, SUBGLOBLOAD *param, SPHERE *sphere, int groups,
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
    desc->cbnd = count;
    entry->abnd = (BND *)AllocateZero((char)count * sizeof(BND));
    for (; i < desc->cbnd; i++) {
        BND *bone = &entry->abnd[i];
        bone->oid = (OID)input->S32Read();
        bone->ixf = input->S16Read();
    }
    int base = 0x1B;
    if (desc->cbnd) {
        desc->iqwBone = base;
        base += desc->cbnd * 4;
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
            if (param->batchMode != 2)
                break;
            header->flags |= 0x400;
            break;
        case 1: {
            HEADER *h = header;
            h->flags = Flags<2>();
        } break;
        }
        header->base = base;
        SHD *object = GetShdByIndex(input->U16Read());
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
        int bones = desc->cbnd < 4 ? desc->cbnd : 4;
        if (desc->cbnd) {
            for (int b = 0; b < 4; b++) {
                int bone = input->U8Read();
                HEADER *slot = (HEADER *)((uptr)header + b * 2);
                slot->bones[0] = (ushort)desc->iqwBone + bone * 4;
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
        for (int segment = 0; segment < object->cshdp; segment++) {
            HEADER *slot = (HEADER *)((uptr)header + segment * 2);
            slot->segments[0] = current;
            current += AddShaderVifUnpack(object, segment, current, &vifs);
        }
        ++current;
        header->weight = current;
        current += 2 * vertices;
        header->end = current;
        header->vertices = vertices;
        vifs.AddVifStcycl(1, 2);
        if (desc->cbnd) {
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
        if (desc->cbnd) {
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
    vifs.DetachCopySw(&desc->cqw, &data, &entry->pqwChain);
    LoadSubglobInstancesFromBrx(p, desc, entry, sphere, groups, 0, 0, 0, input);
}
} // namespace M_LoadSubglobType3FromBrx

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddFramePatch {
struct D { int a; char *b; int c; char *d; };
struct H { short a; short b; };
// Records a (framebuffer word index, qword offset+0x23) patch for the VU1 microprogram, which fills FRAME/TEX0 of the
// current buffer from VU qw 0xB/0xC. (inferred)
extern "C" void AddFramePatch(int n, struct D *pd, struct H **pph)
{
    struct H *ph = (*pph)++;
    ph->a = n;
    ph->b = ((pd->d - pd->b) >> 4) + 0x23;
}
} // namespace M_AddFramePatch

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BuildProjectedVolumeGifs {
extern void AddFramePatch(int n, GIFS *pgifs, short **pps) __asm__("AddFramePatch");
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
extern char D_0025C1C0[] __asm__("D_0025C1C0");
extern char D_0025E040[] __asm__("D_0025E040");
extern char D_00001AA0[] __asm__("D_00001AA0");


extern QW D_002DF800[] __asm__("D_002DF800");
extern int D_002DEB70[2] __asm__("D_002DEB70");
extern void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd) __asm__("EnsureVu1Code__FP4VIFSPvT1");
extern void AddFramePatch(int n, GIFS *pgifs, short **pps) __asm__("AddFramePatch");
extern void BuildProjectedVolumeGifs(GIFS *pgifs, uint grf, short **pps) __asm__("BuildProjectedVolumeGifs");
// Pass 6/7 begin (kinds 7/8): upload projected-volume microprogram D_0025C1C0..D_0025E040, build/kick (MSCAL 0x1AA0)
// the masked full-screen stencil-init strips. (inferred)
extern "C" void BeginRpProjVolume(VIFS *pvifs, int n1, int n2)
{
    EnsureVu1Code__FP4VIFSPvT1(pvifs, D_0025C1C0, D_0025E040);
    pvifs->AddDmaCnt();
    if (n2 != 7 && g_sw.unknown3E38) {
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
extern SW g_swSecondName __asm__("g_sw");   /* kept: g_sw under a second name keeps the two address loads apart (one name lets GCC share the base) */
extern int D_002DEB70[2] __asm__("D_002DEB70");


extern QW*D_002DEA4C __asm__("g_pfrmOpen");
extern QW D_002DEA50[] __asm__("D_002DEA50"),D_002DF940[] __asm__("D_002DF940"),D_002DFA80[] __asm__("D_002DFA80");
extern void AddFramePatch(int,GIFS*,COMMAND**) __asm__("AddFramePatch");
extern void BuildProjectedVolumeGifs(GIFS*,uint,COMMAND**) __asm__("BuildProjectedVolumeGifs");
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char COMMAND_Size[sizeof(COMMAND)==4?1:-1];
typedef char DMAS_m_ab_Offset[FIELD_OFFSET(DMAS,m_ab)==4?1:-1];
typedef char DMAS_m_pb_Offset[FIELD_OFFSET(DMAS,m_pb)==12?1:-1];
typedef char GIFS_m_ab_Offset[FIELD_OFFSET(GIFS,m_ab)==4?1:-1];
typedef char GIFS_m_pb_Offset[FIELD_OFFSET(GIFS,m_pb)==12?1:-1];
typedef char COMMAND_kind_Offset[FIELD_OFFSET(COMMAND,kind)==0?1:-1];
typedef char COMMAND_index_Offset[FIELD_OFFSET(COMMAND,index)==2?1:-1];
// Pass 6/7 end: apply pass reading the framebuffer as an indexed texture (TEX2 g_sw.tex2Background) with alpha blend
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
        gifs.PackAD(0x16, g_sw.tex2Background);
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
    SW *game = &g_swSecondName;
    if (game->unknown3E38) {
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
} // namespace M_EndRpProjVolume

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BeginRpGlob {
extern void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd) __asm__("EnsureVu1Code__FP4VIFSPvT1");
extern char D_0025E680[] __asm__("D_0025E680");
extern char D_00262320[] __asm__("D_00262320");
extern char D_00000000[] __asm__("D_00000000");
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
extern void *func_002333B4(void *pv, int n, int cb) __asm__("memset");
extern char D_00003AB0[] __asm__("D_00003AB0");
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
extern int D_003DCFA0 __asm__("D_003DCFA0");
extern QW *D_003DCFA8[] __asm__("D_003DCFA8");
extern void BeginRpGlob(VIFS *pvifs) __asm__("BeginRpGlob");
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
namespace M_EndRpBackground {
extern int D_003DCFA0 __asm__("D_003DCFA0");
extern char D_002DF010[] __asm__("D_002DF010");
void EndRpGlob(void *p) __asm__("EndRpGlob");
void AddVifDirectFlushed(void *p, int n, void *pv) __asm__("AddVifDirectFlushed");
// Pass 3 end: EndRpGlob + Z-only clear (D_002DF010) when per-frame uploads were made (textures parked in Z-buffer
// memory) (inferred)
extern "C" void EndRpBackground(void *p)
{
    EndRpGlob(p);
    if (D_003DCFA0 > 0)
        AddVifDirectFlushed(p, 0x25, D_002DF010);
}
} // namespace M_EndRpBackground

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
struct SPHERE;struct FACE;
struct HEADER {
    ushort flags, base, weight, pad6, position, positionEnd, color, end, bones[4];
};
struct REQUEST {
    int bytes, alignment;
    // Allocation request: byte count and alignment.
    REQUEST(int cb, int a) : bytes(cb), alignment(a) {}
};

void*STACKSCOPE_ctor(void*) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
void*AllocClearedFromWorldHeap(const REQUEST&) __asm__("AllocClearedFromWorldHeap");
void BuildCmFgfn(CM*,float*,float) __asm__("BuildCmFgfn");
void*func_002333B4(void*,int,uint) __asm__("memset");
void LoadSubglobInstancesFromBrx(GLOBSET*,SUBGLOB*,GLOB*,SPHERE*,int,int,const int*,FACE**,CBinaryInputStream*) __asm__("LoadSubglobInstancesFromBrx");
extern uint D_002DEB50 __asm__("D_002DEB50");
extern QW D_002DEB60 __asm__("D_002DEB60");
extern uint D_002BE540[] __asm__("D_002BE540");
extern UPK D_002BE558[] __asm__("D_002BE558");
extern char D_00000000[] __asm__("D_00000000"),D_00000738[] __asm__("D_00000738"),D_000007F0[] __asm__("D_000007F0");

struct LEASE {
    // Empty destructor (the scope is released explicitly).
    ~LEASE() {}
    // Open a stack-allocation scope (STACKSCOPE).
    void Acquire() { STACKSCOPE_ctor(this); }
    // Release the stack allocations made since the scope opened.
    void Release() { STACKSCOPE_ctor_1AE4A0(this, 2); }
};
// Allocate bytes zeroed, 16-byte aligned, from the world heap.
static inline void *Allocate(int bytes)
{
    return AllocClearedFromWorldHeap(REQUEST(bytes, 16));
}
typedef char HEADER_Size[sizeof(HEADER)==24?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==8?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char VIFS_Size[sizeof(VIFS)==36?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char MARK_Size[sizeof(STACKSCOPE)==1?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
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
// Sub-glob type 1 loader: vertices + colours, no shader (GIF prims); HYPOTHESIS shadow/cel-no-shader.
// Also known as: LoadGlobMesh1FromBrx (upstream readable / Sly 1 name;
void LoadSubglobType1FromBrx(GLOBSET *p, SUBGLOB *desc, GLOB *entry, SUBGLOBLOAD *param, SPHERE *sphere, int groups, CBinaryInputStream *stream) __asm__("LoadSubglobType1FromBrx"); void LoadSubglobType1FromBrx(GLOBSET *p, SUBGLOB *desc, GLOB *entry, SUBGLOBLOAD *param, SPHERE *sphere, int groups,
                                        CBinaryInputStream *stream)
{
    int i = 0;
    VIFS vifs;
    STACKSCOPE outer;
    vifs.AllocSw(0x2000);
    vifs.AddDmaCnt();
    float *environment;
    vifs.UnpackHelper(UPK_V4_32, 1, 9, 0, (uint **)&environment);
    BuildCmFgfn(&g_cm, environment, param->fogIntensity);
    int count = stream->U8Read();
    desc->cbnd = count;
    entry->abnd = (BND *)Allocate((char)count * sizeof(BND));
    for (; i < desc->cbnd; i++) {
        BND *bone = &entry->abnd[i];
        bone->oid = (OID)stream->S32Read();
        bone->ixf = stream->S16Read();
    }
    int base = 0x22;
    GIFS gifs;
    LEASE inner;
    gifs.AllocSw(8);
    gifs.AddPrimPack(0, 2, 0x1EUL);
    gifs.PackAD(0x42L, 0x44L);
    gifs.PackRGBA(D_002DEB50);
    gifs.EndPrim(1);
    vifs.AddVifUnpack(UPK_V4_32, ((uptr)gifs.m_pb - (uptr)gifs.m_ab) >> 4, gifs.m_ab, 0xD);
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
    if (desc->cbnd) {
        desc->iqwBone = base;
        base += desc->cbnd * 4;
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
        if (desc->cbnd) {
            part = stream->U8Read();
            int k = 0;
            for (; k < part; k++) {
                int bone = stream->U8Read();
                HEADER *slot = (HEADER *)((uptr)header + k * 2);
                slot->bones[0] = (ushort)desc->iqwBone + bone * 4;
            }
            int next = current + vertices;
            int small = part < 4;
            for (; k < 4; k++) {
                HEADER *slot = (HEADER *)((uptr)header + k * 2);
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
    vifs.DetachCopySw(&desc->cqw, &data, &entry->pqwChain);
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
struct LINK {
    int pad0;
    void *bounds;
    int pad8;
    SAA *object;
    int next;
};
typedef char LINK_Size[sizeof(LINK)==20?1:-1];
typedef char REQUEST_Size[sizeof(SWALLOC)==8?1:-1];
typedef char LINK_bounds_Offset[FIELD_OFFSET(LINK,bounds)==4?1:-1];
typedef char LINK_object_Offset[FIELD_OFFSET(LINK,object)==12?1:-1];
typedef char LINK_next_Offset[FIELD_OFFSET(LINK,next)==16?1:-1];

extern int D_003DA790 __asm__("D_003DA790");
extern LINK D_003DA798[] __asm__("D_003DA798");
void*AllocCopyFromWorldHeap(SWALLOC*,void*) __asm__("AllocCopyFromWorldHeap");
SAA*CloneSaa(SAA*) __asm__("CloneSaa");
void CopyRelocateDmaChain(SUBGLOB*,GLOB*,int,void*,int,void*,void*) __asm__("CopyRelocateDmaChain");
void CopyAb__FPvT0Ui(void*,const void*,uint) __asm__("CopyAb__FPvT0Ui");
void ClearSle(int*) __asm__("ClearSle");
void PrependSlEntry(void*,LINK*) __asm__("PrependSlEntry");
void RelocateShaderUploadRecords(int,int,int,void*,void*) __asm__("RelocateShaderUploadRecords");

// Allocate bytes, 16-byte aligned, from the world heap and copy source into them.
static inline void *Copy(int bytes, void *source)
{
    SWALLOC request;
    request.cb = bytes;
    request.cbAlign = 16;
    return AllocCopyFromWorldHeap(&request, source);
}
// The object's slot 52 (SO::CloneXfRef, named from CloneSoXfRef) with the stream as its first argument.
static inline void ReadTransform(::SO *object, STREAM *stream, XF **output)
{
    object->CloneXfRef((LO *)stream, output);
}
// If any of p's objects has flag 4, replace p's object array with a heap copy (so it can be edited without touching
// the shared one) (inferred).
static inline void CopyObjects(GLOBSET *p)
{
    for (int i = 0; i < p->cpsaa; i++) {
        if (p->apsaa[i]->grf & 4) {
            p->apsaa = (SAA **)Copy(p->cpsaa * 4, p->apsaa);
            return;
        }
    }
}
// Clones a globset for a cloned SO: copies entries/segments, re-resolves transforms, swaps cloned objects, relocates
// packet copies. (inferred)
extern "C" void CloneGlobset(GLOBSET *p, ::SO *owner, STREAM *stream)
{
    int bytes;
    p->owner = owner;
    p->pieces = (GLOB *)Copy(p->numPieces * sizeof(GLOB), p->pieces);
    p->pwrbgFirst = 0;
    CopyObjects(p);
    for (int i = 0; i < p->numPieces; i++) {
        SUBGLOB *descriptor = &p->subMeshes[i];
        GLOB *entry = &p->pieces[i];
        ReadTransform(owner, stream, &entry->transform);
        entry->abnd = (BND *)Copy(descriptor->cbnd * sizeof(BND), entry->abnd);
        for (int j = 0; j < descriptor->cbnd; j++) {
            char *pb = (char *)entry->abnd;   /* byte arithmetic, as the original */
            pb += j * sizeof(BND);
            ReadTransform(owner, stream, &((BND *)pb)->pxf);
        }
        int cloned = entry->psaa && (entry->psaa->grf & 4);
        if (cloned) {
            SAA *object = CloneSaa(entry->psaa);
            for (int j = 0; j < p->cpsaa; j++)
                if (p->apsaa[j] == entry->psaa) {
                    p->apsaa[j] = object;
                    break;
                }
            entry->psaa = object;
        }
        if (!descriptor->pvLit && !cloned)
            continue;
        int count = 0;
        void *old = 0;
        void *data = 0;
        if (descriptor->unknown02 & 1) {
            count = descriptor->cqw;
            old = entry->pqwChain;
            entry->pqwChain = (QW *)Copy(count * 16, old);
            data = entry->pqwChain;
        } else {
            if (entry->psaa) {
                count = entry->psaa->cqwMain;
                old = data = entry->psaa->pvMain;
                if (cloned) {
                    entry->psaa->pvMain = Copy(count * 16, data);
                    data = entry->psaa->pvMain;
                }
            }
            CopyRelocateDmaChain(descriptor, entry, descriptor->cqw, entry->pqwChain, count, data, old);
        }
        if (entry->psaa) {
            bytes = count * 16;
            int span = entry->iUnkEnd - entry->iUnkFirst;
            CopyAb__FPvT0Ui(&D_003DA798[D_003DA790], &D_003DA798[entry->iUnkFirst], span * sizeof(LINK));
            entry->iUnkFirst = D_003DA790;
            D_003DA790 += span;
            entry->iUnkEnd = D_003DA790;
            for (uint j = entry->iUnkFirst; j < entry->iUnkEnd; j++) {
                LINK *link = &D_003DA798[j];
                if (cloned) {
                    link->bounds = &entry->psaa->uOffset;
                    link->object = entry->psaa;
                }
                ClearSle(&link->next);
                PrependSlEntry(&link->object->instances, link);
            }
            RelocateShaderUploadRecords(entry->iUnkFirst, entry->iUnkEnd, bytes, old, data);
        }
    }
}
} // namespace M_CloneGlobset

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_ResetGlobAttachment {
struct V4 { float x, y, z, w; } __attribute__((aligned(16)));   /* attribute on the struct (C++) */
typedef struct { char pad[0x148]; unsigned long renderFlags; } A;   /* the attached ALO: its 64-bit flag word at +0x148 */
typedef struct { float x, y, z, w; int a, b; } E;
typedef struct { A *pa; int pad[3]; V4 offset; float radius; int pad2[8]; E *ae; } S;
extern V4 D_002A2DD0 __asm__("D_002A2DD0");
void RemoveLoFromRenderTree(A *pa) __asm__("RemoveLoFromRenderTree");
/* GetVectorComponentX/E0/E8: the file-scope inline readers */



// Resets an attachment record (clear flag on attached object, zero vector, radius 1e6) (inferred)
extern "C" void ResetGlobAttachment(S *p, int i)
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
} // namespace M_ResetGlobAttachment

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
extern int D_002BE580 __asm__("D_002BE580") = 0;
}
