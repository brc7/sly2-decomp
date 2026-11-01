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
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/cm.h"
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
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
      float gUnk10;
      uint grfUnk14;
};
struct SUBGLOBLOAD
{
    float gUnk00;
    int nUnk04;
    int nUnk08;
};
void LoadPoseSetsFromBrx(GLOBSET *pglobset, CBinaryInputStream *pbis);
struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
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
