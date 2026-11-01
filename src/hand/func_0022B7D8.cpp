// func_0022B7D8 = Sly 1's UpdateWrStateVectors (Sly 1 0x1F2840): for every warp group (WRBG) of a space warp (WR), the
// weight of each vertex (the WEKI envelope: a linear CLQ of the distance along the warp axis, limited to an LM) and the state
// vector every warp entry (WRE) keeps per vertex (circle: cos/sin of the wave phase at the vertex; bend: the vertex offset
// from the bend centre, w = 1). Struct names/layouts follow Lilly's Sly 2 headers (wr.h, glob.h, alo.h); our names otherwise.
// How it matched (Lilly agent, T1601): the vertex loops use the game's by-value VU_VECTOR / VU_FLOAT classes. Their
// constructor copies are what make the circle loop big enough (>= 97 RTL insns) for loop.c to leave its constants
// (2 pi, 1.0, 1) inside the loop like the game; the dot product's float comes out through an asm mtc1.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;

struct VECTOR
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct MATRIX4
{
    float mat[4][4];
} __attribute__((aligned(16)));

// A float in x of a VU0 register
struct VU_FLOAT
{
    qword q;
};

// A vector in a VU0 register (the game passes and returns it by value)
struct VU_VECTOR
{
    qword q;
    VU_VECTOR() {}
    VU_VECTOR(qword qw) { q = qw; }
    VU_VECTOR(const VECTOR &vec) { q = *(const qword *)&vec; }
};

// Quadratic curve g0 + x * (g1 + x * g2)
struct CLQ
{
    float g0, g1, g2, g3;
};

// Limits
struct LM
{
    float gMin, gMax;
};

struct SO;

struct XF
{
    MATRIX4 mat;
    MATRIX4 matWorld;
};

struct GLOB
{
    int oid;
    XF *pxf;
    MATRIX4 *pmatUnk08;
};

// Singly linked list
struct SL
{
    void *head;
    void *tail;
    int ibSle;
};

// The warp envelope: weight uInner at distance sInner, uOuter at sOuter (linear in between), measured in dmat's space
struct WEKI
{
    int wek;
    float sInner;
    float uInner;
    float sOuter;
    float uOuter;
    int unk14[3];
    MATRIX4 dmat;
};

// The state vector of one vertex for one warp matrix
struct WARPVTX
{
    float ag[4];
};

// A batch of the glob: its vertex positions and, per warp matrix, where the state vectors go
struct WRBGBATCH
{
    int cvtx;
    VECTOR *apos;
    WARPVTX *apwarpvtx[4];
    WRBGBATCH *pwrbgbatchNext;
};

struct WRBG
{
    SO *pso;
    GLOB *pglob;
    int unk08[10];
    WEKI weki;
    SL slBatch;
    WRBG *pwrbgNextGlobset;
    WRBG *pwrbgNextWr;
};

struct WRE
{
    int wrek;
    int imat;
    int unk08[2];
    VECTOR axis;
    int unk20[16];
    int column;
    float spatialFrequency;
    float frequency;
    float phase;
    int unk70[24];
};

struct WR
{
    int unk00[21];
    int cwre;
    int unk58[2];
    WRE awre[8];
    int unk6E0[132];
    WRBG *pwrbgFirst;
    int unk8F4[2];
    int fValuesChanged;
};

extern VECTOR D_002A2DA0; // (1, 0, 0, 0)
extern VECTOR D_002A2DB0; // (0, 1, 0, 0)
extern VECTOR D_002A2DC0; // (0, 0, 1, 0)
extern VECTOR D_002A2DD0; // (0, 0, 0, 0)

extern "C"
{
void *func_002333B4(void *pv, int b, uint cb);                       // memset
void func_001AE5A0(void *pvDst, const void *pvSrc, uint cb);         // CopyAb
qword func_001A6F00(SO *psoFrom, SO *psoTo, qword pos);              // ConvertLoPos
float RadNormalize__Ff(float rad);                                      // RadNormalize
void CalculateSinCos__FfPfT1(float rad, float *pgSin, float *pgCos);           // CalculateSinCos
MATRIX4 InvertMatrix4Rigid(const MATRIX4 *pmat);                          // the inverse of a 4x4 matrix
MATRIX4 __ml__FRC7MATRIX4T0(const MATRIX4 &matL, const MATRIX4 &matR);     // MATRIX4 operator*
}

// pos (w = 1) transformed by mat
static inline qword QwTransformPos(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}

static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.q) : "r"(g));
    return vu;
}

static inline qword QwScale(qword qw, VU_FLOAT g)
{
    qword qwResult;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwResult) : "j"(qw), "j"(g.q));
    return qwResult;
}

static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qw));
    return g;
}

// |v| (x, y, z) in x
static inline qword QwLength(qword v)
{
    qword qw, vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}

static inline float GMin(float g0, float g1)
{
    return g0 < g1 ? g0 : g1;
}

static inline float GMax(float g0, float g1)
{
    return g0 > g1 ? g0 : g1;
}

static inline float GLimit(float g, float gMin, float gMax)
{
    if (g < gMin)
        g = gMin;
    else if (gMax < g)
        g = gMax;
    return g;
}

static inline void SetLm(LM *plm, const float &g0, const float &g1)
{
    plm->gMin = GMin(g0, g1);
    plm->gMax = GMax(g0, g1);
}

// The line through (s0, u0) and (s1, u1)
static inline void FitLinearClq(CLQ *pclq, const float &s0, const float &u0, const float &s1, const float &u1)
{
    pclq->g1 = (u0 - u1) / (s0 - s1);
    pclq->g0 = u0 - pclq->g1 * s0;
    pclq->g2 = 0.0f;
}

// pos (w = 1) transformed by mat
static inline VU_VECTOR operator*(const MATRIX4 &mat, VU_VECTOR pos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(VU_VECTOR(*(VECTOR *)mat.mat[0]).q), "j"(VU_VECTOR(*(VECTOR *)mat.mat[1]).q),
              "j"(VU_VECTOR(*(VECTOR *)mat.mat[2]).q), "j"(VU_VECTOR(*(VECTOR *)mat.mat[3]).q), "j"(pos.q));
    return VU_VECTOR(qw);
}

static inline VU_VECTOR operator-(VU_VECTOR vec0, VU_VECTOR vec1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(vec0.q), "j"(vec1.q));
    return VU_VECTOR(qw);
}

// vec0 . vec1 (x, y, z) in x
static inline VU_FLOAT VuDot(VU_VECTOR vec0, VU_VECTOR vec1)
{
    VU_FLOAT g;
    qword qw, vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(vec0.q), "j"(vec1.q));
    g.q = qw;
    return g;
}

static inline VU_VECTOR Normalize(VU_VECTOR vec)
{
    qword qwLength = VuDot(vec, vec).q, qwNormal;
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLength) : "0"(qwLength));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qwLength), "j"(vec.q));
    return VU_VECTOR(qwNormal);
}

// x of a VU_FLOAT as a float, through an asm mtc1 (the compiler's mtc1 schedules the circle case differently)
static inline float GFromVu(VU_FLOAT g)
{
    register int n asm("$3"); // the one pin: local-alloc gives this block-local scratch $v0, the game has $v1
    float gResult;
    __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(g.q));
    __asm__("mtc1 %1, %0" : "=f"(gResult) : "r"(n));
    return gResult;
}

extern "C" void func_0022B7D8(WR *pwr)
{
    MATRIX4 dmat;
    CLQ clq;
    LM lm;
    qword posBend;
    qword posWorldBend;

    for (WRBG *pwrbg = pwr->pwrbgFirst; pwrbg; pwrbg = pwrbg->pwrbgNextWr)
    {
        // the envelope's space and the bend centre in it
        if (pwrbg->weki.wek == -1)
        {
            func_002333B4(&dmat, 0, sizeof(MATRIX4));
            dmat.mat[0][0] = dmat.mat[1][1] = dmat.mat[2][2] = dmat.mat[3][3] = 1.0f;
            posBend = *(qword *)&D_002A2DD0;
        }
        else
        {
            dmat = pwrbg->weki.dmat;
            SetLm(&lm, pwrbg->weki.uInner, pwrbg->weki.uOuter);
            FitLinearClq(&clq, pwrbg->weki.sInner, pwrbg->weki.uInner, pwrbg->weki.sOuter, pwrbg->weki.uOuter);
            switch (pwrbg->weki.wek)
            {
            case 4:
                posBend = QwScale(*(qword *)&D_002A2DA0, VuFloat((pwrbg->weki.sInner + pwrbg->weki.sOuter) * 0.5f));
                break;
            case 5:
                posBend = QwScale(*(qword *)&D_002A2DB0, VuFloat((pwrbg->weki.sInner + pwrbg->weki.sOuter) * 0.5f));
                break;
            case 6:
                posBend = QwScale(*(qword *)&D_002A2DC0, VuFloat((pwrbg->weki.sInner + pwrbg->weki.sOuter) * 0.5f));
                break;
            default:
                posBend = *(qword *)&D_002A2DD0;
                break;
            }
            MATRIX4 matInverse;
            matInverse = InvertMatrix4Rigid(&dmat);
            posBend = QwTransformPos(matInverse, posBend);
            if (pwrbg->pglob->pmatUnk08)
                dmat = __ml__FRC7MATRIX4T0(pwrbg->weki.dmat, *pwrbg->pglob->pmatUnk08);
        }

        for (WRBGBATCH *pwrbgbatch = (WRBGBATCH *)pwrbg->slBatch.head; pwrbgbatch; pwrbgbatch = pwrbgbatch->pwrbgbatchNext)
        {
            VECTOR apos[256];
            float au[256];
            func_001AE5A0(apos, pwrbgbatch->apos, pwrbgbatch->cvtx * sizeof(VECTOR));
            VECTOR *pposEnd = apos + pwrbgbatch->cvtx;
            VECTOR *ppos = apos;
            float *pu = au;

            // the weight of every vertex
            for (; ppos < pposEnd; ppos++, pu++)
            {
                VECTOR pos;
                float s;
                *(qword *)&pos = QwTransformPos(dmat, *(qword *)ppos);
                switch (pwrbg->weki.wek)
                {
                case -1:
                    *pu = 1.0f;
                    continue;
                case 0:
                    s = GFromVf(QwLength(*(qword *)&pos));
                    break;
                case 1:
                    pos.z = 0.0f;
                    s = GFromVf(QwLength(*(qword *)&pos));
                    break;
                case 2:
                    pos.y = 0.0f;
                    s = GFromVf(QwLength(*(qword *)&pos));
                    break;
                case 3:
                    pos.x = 0.0f;
                    s = GFromVf(QwLength(*(qword *)&pos));
                    break;
                case 4:
                    s = pos.x;
                    break;
                case 5:
                    s = pos.y;
                    break;
                case 6:
                    s = pos.z;
                    break;
                default:
                    *pu = 1.0f;
                    continue;
                }
                *pu = GLimit(clq.g0 + s * (clq.g1 + s * clq.g2), lm.gMin, lm.gMax);
            }

            MATRIX4 matWorld = pwrbg->pglob->pxf->matWorld;
            if (pwrbg->pglob->pmatUnk08)
                matWorld = __ml__FRC7MATRIX4T0(matWorld, *pwrbg->pglob->pmatUnk08);
            posWorldBend = func_001A6F00(pwrbg->pso, 0, posBend);

            // the state vectors of every warp entry
            for (int iwre = 0; iwre < pwr->cwre; iwre++)
            {
                WRE *pwre = &pwr->awre[iwre];
                WARPVTX *pwarpvtx = pwrbgbatch->apwarpvtx[pwre->imat];
                ppos = apos;
                pu = au;
                for (; ppos < pposEnd; ppos++, pwarpvtx++, pu++)
                {
                    VECTOR posWorld;
                    *(VU_VECTOR *)&posWorld = matWorld * VU_VECTOR(*ppos);
                    switch (pwre->wrek)
                    {
                    case 0:
                    {
                        int is = pwre->column;
                        float gSin, gCos;
                        VU_FLOAT gDot = VuDot(VU_VECTOR(posWorld), Normalize(VU_VECTOR(pwre->axis)));
                        float rScale = pwre->spatialFrequency * 6.2831855f;
                        float rad = GFromVu(gDot) * rScale;
                        CalculateSinCos__FfPfT1(RadNormalize__Ff(rad), &gSin, &gCos);
                        pwarpvtx->ag[is] = gCos * *pu;
                        pwarpvtx->ag[is + 1] = gSin * *pu;
                        break;
                    }
                    case 1:
                    {
                        VECTOR dpos;
                        *(VU_VECTOR *)&dpos = VU_VECTOR(posWorld) - VU_VECTOR(posWorldBend);
                        pwarpvtx->ag[0] = *pu * dpos.x;
                        pwarpvtx->ag[1] = *pu * dpos.y;
                        pwarpvtx->ag[2] = *pu * dpos.z;
                        pwarpvtx->ag[3] = 1.0f;
                        break;
                    }
                    }
                }
            }
        }
    }
    pwr->fValuesChanged = 0;
}
