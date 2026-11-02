// RenderWaypt (cid 107 slot 39): fades the job marker in/out by the world clock, draws its three child SOs
// scaled by the camera distance, and the job's name plate above it. Member names are descriptive guesses.
// What decided the match: tStart reused for both fade times (one pseudo -> the game's f4/f3 registers), clamp
// with a single result variable and by-value limits, `dz = p ? .. : 0.0f` (splits cse so g_normalZ's hi part is
// copied into s4), the name plate drawn through a by-value RGBA inline (copy addresses in s0/s1), and the
// float argument of func_001CD780 second (invisible in the callee, decides the f12 move's slot).
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned char uchar;
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
struct MATRIX3 { VECTOR mat[3]; };
struct MATRIX4 { VECTOR mat[3]; VECTOR pos; };
struct VU_FLOAT { qword qw; };
struct CLQ { float g0, g1, g2, gUnused; };
struct LM { float gMin, gMax; };
struct RGBA
{
    uchar r, g, b, a;
    RGBA(uint bits) { *(uint *)this = bits; }
};
struct LO;
struct TASK { char pad[0x6C]; int idLabelName; };
struct ATTACH { char pad[0x7C]; TASK *ptask; };
struct SO { char pad[0xA0]; float sRadius; };
struct WAYPT
{
    char pad[0x150];
    int nMode;              // 0x150
    char pad154[0xC];
    ATTACH *pattach;        // 0x160
    int unk164;
    LO *ploSource;          // 0x168
    SO *psoMarker;          // 0x16C
    SO *psoBeam;            // 0x170
    SO *psoBase;            // 0x174
    int unk178, unk17C;
    float sHeight;          // 0x180
    int ipukReq;            // 0x184
    void *pcond;            // 0x188
};
struct SW { char pad[0x3B0C]; int fUnk3B0C; char pad3B10[0xC40]; float tUnk4750, tUnk4754; char pad4758[0xC]; int cUnk4764, cUnk4768; };
struct CM { char pad[0x90]; qword pos; };
struct CLOCK { float t; };
extern SW D_003DD490;      // g_sw
extern CM D_002DDF40;      // g_cm
extern CLOCK D_002DDED0;   // g_clock
extern qword D_002A2DC0;   // g_normalZ
extern LM D_002BBB60;      // g_lmZeroOne
extern float D_002C4494, D_002C4498, D_002C449C, D_002C44D8;
extern CLQ D_002C44A0, D_002C44B0, D_002C44C0;
extern LM D_002C44D0;
extern "C" {
void RenderSoSelf(WAYPT *pwaypt);
int func_001DAEA8(int ipuk);
int FJobComplete(void *pcond);
void func_0022A220(WAYPT *pwaypt);
qword func_001A62E8(LO *plo);
void FindClosestPointOnLineSegment(qword pos, qword p0, qword p1, qword *ppos, float *pt, float *pdist);
MATRIX4 *func_001A6318(SO *pso);
VECTOR *func_00256D18(VECTOR *pv, qword qw);
void func_001A5EE8(SO *pso, MATRIX4 *pmat, float a, float b);
void func_001A5FB8(SO *pso, qword pos0, qword pos1, float s, float a);
char *PchzFindSwLangstr__FP2SWi(SW *psw, int id);
void func_001CD780(qword pos, float s, char *pchz, const RGBA *prgba0, const RGBA *prgba1);
}
static inline void DrawLabel(qword pos, RGBA rgba0, RGBA rgba1, int id, float s) { func_001CD780(pos, s, PchzFindSwLangstr__FP2SWi(&D_003DD490, id), &rgba0, &rgba1); }

static inline float GEvaluateClq(CLQ *pclq, float x) { return pclq->g0 + x * (pclq->g1 + x * pclq->g2); }
static inline float GLimitG(float g, float gMin, float gMax)
{
    float gRet;
    if (g < gMin)
        gRet = gMin;
    else if (gMax < g)
        gRet = gMax;
    else
        gRet = g;
    return gRet;
}
static inline float GMax(float a, float b) { return a > b ? a : b; }
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(g.qw));
    return qw;
}
// The same sum with the scalar as the first asm input: the result then takes the dying scalar's vf register (name plate offset)
static inline qword QwAddScaledG(qword qwBase, qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %2, $vf0\n\tvmaddx.xyzw %0, %3, %1x" : "=j"(qw) : "j"(g.qw), "j"(qwBase), "j"(qwVec));
    return qw;
}
static inline qword QwMulX(qword qwRow, VU_FLOAT g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(g.qw), "j"(qwRow));
    return qw;
}
inline MATRIX3 operator*(const MATRIX3 &mat, VU_FLOAT g) return matRes
{
    MATRIX3 matT;
    *(qword *)&matT.mat[0] = QwMulX(*(qword *)&mat.mat[0], g);
    *(qword *)&matT.mat[1] = QwMulX(*(qword *)&mat.mat[1], g);
    *(qword *)&matT.mat[2] = QwMulX(*(qword *)&mat.mat[2], g);
    matRes = matT;
}

/** @sly2-annot addr=0x22A908
 * Purpose: fades the job marker, draws 3 child SOs scaled by distance, job name plate
 * Info: proposed name RenderWaypt (T1); class WAYPT slot19; TU waypoint
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#waypoint-0x229ef0-0x22b040
 * @end-sly2-annot */
extern "C" void func_0022A908(WAYPT *pwaypt)
{
    RenderSoSelf(pwaypt);
    float tStart = GMax(D_003DD490.tUnk4750 + D_002C4494, D_003DD490.tUnk4754 + D_002C4498);
    float uShow = GLimitG(GEvaluateClq(&D_002C44A0, D_002DDED0.t - tStart), D_002BBB60.gMin, D_002BBB60.gMax);
    tStart = D_003DD490.tUnk4750 + D_002C449C;
    float uLabel = GLimitG(GEvaluateClq(&D_002C44B0, D_002DDED0.t - tStart), D_002BBB60.gMin, D_002BBB60.gMax);
    switch (pwaypt->nMode)
    {
    case 0:
        if (D_003DD490.fUnk3B0C)
            uShow = uLabel = 0.0f;
        break;
    case 2:
        if (D_003DD490.cUnk4764 < D_003DD490.cUnk4768 || D_003DD490.fUnk3B0C ||
            (pwaypt->ipukReq != -1 && func_001DAEA8(pwaypt->ipukReq)) ||
            (pwaypt->pcond && !FJobComplete(pwaypt->pcond)))
            uShow = uLabel = 0.0f;
        break;
    case 1:
        if (!D_003DD490.fUnk3B0C)
            uShow = uLabel = 0.0f;
        break;
    }
    if (uShow == 0.0f)
        return;

    func_0022A220(pwaypt);
    qword posCam = D_002DDF40.pos;
    qword pos = func_001A62E8(pwaypt->ploSource);
    qword posTop = QwAddScaled(pos, D_002A2DC0, VuFloatFromMemWord(*(int *)&pwaypt->sHeight));
    float sDist;
    FindClosestPointOnLineSegment(posCam, pos, posTop, 0, 0, &sDist);
    float rScale = GLimitG(GEvaluateClq(&D_002C44C0, sDist), D_002C44D0.gMin, D_002C44D0.gMax);
    qword posEnd = QwAddScaled(posTop, D_002A2DC0, VuFloat(rScale * D_002C44D8));

    MATRIX4 mat;
    if (pwaypt->psoMarker)
    {
        MATRIX3 matScaled = *(MATRIX3 *)func_001A6318(pwaypt->psoMarker) * VuFloat(rScale);
        MATRIX4 *pmat = &mat;
        func_00256D18(&mat.pos, posEnd);
        *(MATRIX3 *)pmat = matScaled;
        mat.mat[2].w = 0.0f;
        mat.mat[1].w = 0.0f;
        mat.mat[0].w = 0.0f;
        pmat->pos.w = 1.0f;
        func_001A5EE8(pwaypt->psoMarker, pmat, uShow, uShow);
    }
    if (pwaypt->psoBeam)
        func_001A5FB8(pwaypt->psoBeam, pos, posEnd, rScale, uShow);
    if (pwaypt->psoBase)
    {
        MATRIX3 matScaled = *(MATRIX3 *)func_001A6318(pwaypt->psoBase) * VuFloat(rScale);
        MATRIX4 *pmat = &mat;
        func_00256D18(&mat.pos, pos);
        *(MATRIX3 *)pmat = matScaled;
        mat.mat[2].w = 0.0f;
        mat.mat[1].w = 0.0f;
        mat.mat[0].w = 0.0f;
        pmat->pos.w = 1.0f;
        func_001A5EE8(pwaypt->psoBase, pmat, uShow, uShow);
    }

    if (0.0f < uLabel && pwaypt->pattach && pwaypt->pattach->ptask && pwaypt->pattach->ptask->idLabelName != -1)
    {
        float dz = pwaypt->psoMarker ? rScale * pwaypt->psoMarker->sRadius : 0.0f;
        qword posLabel = QwAddScaledG(posEnd, D_002A2DC0, VuFloat(dz));
        RGBA rgbaBack(0x80603030);
        RGBA rgbaText(0xFFC0C0C0);
        rgbaBack.a = (uchar)(int)(uLabel * rgbaBack.a);
        rgbaText.a = (uchar)(int)(uLabel * rgbaText.a);
        DrawLabel(posLabel, rgbaBack, rgbaText, pwaypt->pattach->ptask->idLabelName, rScale);
    }
}
