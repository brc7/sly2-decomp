/* include/sly2/wr.h -- space warps: the warp groups (WRBG) a space warp (WR, engine/anim/wr.cpp) moves, their vertex
 * batches and the weight envelope over the warp axis. Loaded by engine/graphics/pipeline/glob.cpp, updated by wr.cpp
 * (UpdateWrStateVectors). Field names are glob.cpp's (Sly 1's); the comments give their meaning. */
#ifndef SLY2_WR_H
#define SLY2_WR_H
#include "sly2/basic.h"

struct WR;

/* WEK: the envelope kind (-1: none; 0..2 the warp axis x/y/z, wr.cpp's UpdateWrStateVectors) */
enum WEK
{
};

/* WEKI: a warp group's weight envelope: a linear weight from (sInner, uInner) to (sOuter, uOuter) along the warp axis
   (FitLinearClq), limited to [uInner, uOuter], in the space of dmat */
struct WEKI
{
    WEK wek;
    float sInner;           /* inner distance */
    float uInner;           /* weight at the inner distance */
    float sOuter;           /* outer distance */
    float uOuter;           /* weight at the outer distance */
    undefined1 unknown[12];
    MATRIX4 dmat;           /* +0x20 */
};
typedef char assert_WEKI_size[sizeof(WEKI) == 0x60 ? 1 : -1];

/* WRBGBATCH: one vertex batch of a warp group: its positions and the per-vertex warp state (four float4 arrays) */
struct WRBGBATCH
{
    int cvtx;               /* vertex count */
    void *pvPos;            /* the positions */
    void *apvWarp[4];       /* the warp vertex arrays */
    WRBGBATCH *pwrbgbatchNext;
};

/* WRBG: a warp group: the vertices of a GLOB that a space warp moves */
struct WRBG
{
    SO *pso;
    GLOB *pglob;
    OID oid;
    WR *pwr;                /* +0x0C the warp (AddWrWarpGroup) */
    int cmat;
    int fDpos;
    int fDuv;
    ushort grf;
    int iqw;
    undefined1 unknown[12];
    WEKI weki;              /* +0x30 */
    SL slBatch;             /* +0x90 its WRBGBATCH list */
    WRBG *pwrbgNextGlobset;
    WRBG *pwrbgNextWr;      /* next group of the same warp */
};
typedef char assert_WRBG_size[sizeof(WRBG) == 0xB0 ? 1 : -1];

#endif /* SLY2_WR_H */
