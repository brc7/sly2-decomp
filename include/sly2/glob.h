/* include/sly2/glob.h -- model (GLOB) records beyond the ones so.h defines: the per-sub-mesh extra data a GLOBSET
 * keeps for culling (SUBGLOBX), the sub-mesh loaders' shared state (SUBGLOBLOAD) and the per-vertex colour palettes
 * (CLUTSET).
 *
 * GLOB, SUBGLOB, POSE and GLOBSET themselves are in sly2/so.h; GLOBSET::asubglobx points at an array of SUBGLOBX, one
 * per GLOB (LoadGlobsetFromBrx in engine/graphics/pipeline/glob.cpp fills it). */
#ifndef SLY2_GLOB_H
#define SLY2_GLOB_H
#include "sly2/basic.h"

/* SUBGLOBX: culling data of one GLOB: bounding sphere (local), the distance beyond which it is not drawn, flags.
   Layout from the 12 identical copies (glob.cpp's names). */
struct SUBGLOBX
{
    VECTOR posCenter;          /* centre of the bounding sphere (local) */
    float sRadius;             /* radius of the bounding sphere */
    float maxRenderDistance;   /* FLT_MAX in the file: 1e10, or 2000 for cel-border edges (type 1) */
    uint flags;
};
typedef char assert_SUBGLOBX_maxRenderDistance[SLY2_OFFSETOF(SUBGLOBX, maxRenderDistance) == 0x10 ? 1 : -1];
typedef char assert_SUBGLOBX_size[sizeof(SUBGLOBX) == 0x18 ? 1 : -1];

/* SUBGLOBLOAD: load-time state of one sub-mesh, passed between LoadGlobsetFromBrx and the per-type sub-mesh loaders */
struct SUBGLOBLOAD
{
    float fogIntensity;     /* BuildCmFgfn's argument (the fog curve uploaded with the geometry) */
    int numLitQwords;       /* size of the lit-vertex DMA chain (set by the type-0 loader) */
    int batchMode;          /* 0 / 1 / 2: batch header flag 0x100 / 0x200 / 0x400 */
};

/* CLUTSET: the per-vertex colour palettes of a type-0 sub-glob (LoadClutsetFromBrx reads them; LoadSubglobType0FromBrx
   copies palette aipal[ibatch]'s colours into the batches flagged 0x20). Names ours. */
struct CLUTCOLOR
{
    ushort ucA;             /* a 16-bit colour (colour-corrected at load) */
    ushort ucB;             /* the second 16-bit colour */
    uchar bAlpha;
};
struct CLUTPAL
{
    int ccolor;             /* +0x00 colours (one per vertex of the file) */
    CLUTCOLOR *acolor;      /* +0x04 */
};
struct CLUTSET
{
    int cpal;               /* +0x00 */
    CLUTPAL *apal;          /* +0x04 */
    int *aipal;             /* +0x08 palette of each batch */
};
typedef char assert_CLUTCOLOR_size[sizeof(CLUTCOLOR) == 6 ? 1 : -1];
typedef char assert_CLUTPAL_size[sizeof(CLUTPAL) == 8 ? 1 : -1];
typedef char assert_CLUTSET_size[sizeof(CLUTSET) == 0xC ? 1 : -1];

#endif
