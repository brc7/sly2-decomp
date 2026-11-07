/* include/sly2/wr.h -- space warps: the warp groups (WRBG) a space warp (WR, engine/anim/wr.cpp) moves, their vertex
 * batches and the weight envelope over the warp axis. Loaded by engine/graphics/pipeline/glob.cpp, updated by wr.cpp
 * (UpdateWrStateVectors). Field names are glob.cpp's (Sly 1's); the comments give their meaning. */
#ifndef SLY2_WR_H
#define SLY2_WR_H
#include "sly2/basic.h"
#include "sly2/lo.h"

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

/* ONZE, ONZ: a noise term and a noise channel, the weighted sum of up to four terms (Sly 1 names) */
struct ONZE { float amplitude; float frequency; float phase; float randomness; };
struct ONZ { float minValue; float maxValue; float bias; int octaveCount; ONZE octaves[4]; };
/* WRECIRCLE, WREBEND: the two kinds of warp entry: a circular wave, or a bend with bend and swivel noise */
struct WRECIRCLE { QWROW uvA, vectorB, uvB; int column; float spatialFrequency, frequency, phase; };
struct WREBEND { ONZ bend, swivel; };
union WREDATA { WRECIRCLE circle; WREBEND bend; };
/* WRE: a warp entry (0xD0): its kind (WREK), the warp/uv matrix pair whose columns it fills, its axis and data */
struct WRE
{
    int kind;               /* WREK */
    int matrixIndex;
    int unknown08[2];
    union { QWROW axis; VECTOR4 vecAxis; };
    QWROW vectorA;
    WREDATA data;
};
typedef char assert_WRE_size[sizeof(WRE) == 0xD0 ? 1 : -1];
enum WREK { WREK_Circle = 0, WREK_Bend = 1 };

/* 115 WR (0x900): a space warp (wr.cpp): up to eight warp entries packed two columns each into four warp/uv matrix
   pairs, and the warp groups (WRBG) it moves; vtable 0x2DBCE8 (class_registry.cpp constructs it) */
struct WR : LO
{
    float lastUpdateTime;   /* +0x50 clock time of the last matrix update */
    int entryCount;
    int matrixCount;
    int freeMatrixIndex;    /* +0x5C a matrix with a free column pair, or -1 */
    WRE entries[8];
    int currentEntryIndex;  /* +0x6E0 the last entry added */
    int pad6E4[3];
    MATRIX4Q warpMatrices[4];   /* +0x6F0 */
    MATRIX4Q uvMatrices[4];     /* +0x7F0 */
    SL warpGroups;          /* +0x8F0 its WRBG list (WRBG::pwrbgNextWr) */
    int valuesChanged;      /* +0x8FC */
    virtual void Init() __asm__("InitWr");   /* slot 7; key function (wr.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWrClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateWrMatrixes__FP2WR");   /* slot 16 */
};
typedef char assert_WR_warpMatrices[SLY2_OFFSETOF(WR, warpMatrices) == 0x6F0 ? 1 : -1];
typedef char assert_WR_valuesChanged[SLY2_OFFSETOF(WR, valuesChanged) == 0x8FC ? 1 : -1];
typedef char assert_WR_size[sizeof(WR) == 0x900 ? 1 : -1];   /* class descriptor cb */

#endif /* SLY2_WR_H */
