/* include/sly2/glob.h -- model (GLOB) records beyond the ones so.h defines: the per-sub-mesh extra data a GLOBSET
 * keeps for culling (SUBGLOBX).
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

#endif
