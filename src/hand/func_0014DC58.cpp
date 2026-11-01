// func_0014DC58: ported from Lilly's personal Sly 2 decomp (src/P2/cm.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DE0 (D_002A2DE0), D_002BD850 (D_002BD850), D_002BD854 (SW_CmJolt), D_002DDED0 (g_clock), D_002BBB60 (g_lmZeroOne), D_002BD860 (s_clqUToUJolt), D_002BD870 (s_dposJolt), D_002BD880 (s_rotJolt), D_002BD8A0 (s_vecOffset), D_002BD890 (s_vecRange)
// 0x0014DC58 UpdateCmMat4__FP2CM  (T1386; Sly 1 never matched it)
// The camera's placement CM::mat (3 rotation rows, row 3 = the position; SetCmMat4 / SetCmPanTilt / the policies set it) or its projection changed:
//  1. the world -> clip matrix matViewProj = CombineEyeLookAtProj(eye, matProj), where eye is CM::mat itself, or, for 0.5 s after a camera jolt
//     (tJolt, set by 0x14F750), CM::mat times a small shake matrix (rotation about x of 0.05 rad * jolt, a position kick) and a projection whose scale
//     and offset the jolt changes (the kick, scale and offset data are zero in the ELF: only the rotation shakes). jolt = sin(50 * dt) * (1 - dt / 0.5)^2;
//  2. matViewProjInv = FInvertMatrix4(matViewProj);
//  3. the four frustum side planes (BuildFrustrum) of the UNshaken CM::mat: aplaneFrustum for the real fov and aplaneFrustumWide for tan(x) * 7.93 and
//     tan(y) * 18.1, each also stored transposed (x, y, z of the four normals = agFrustumXyz / agFrustumWideXyz, the VU0 cull microprogram's vf10-vf12);
//  4. UpdateCmVisZone picks the vis zone the camera is in.
// How it matches: the jolt's float is moved into a GPR once (VuFloatOfFloat is CSEd) and spilled across the calls; the shake matrix is built in
// matEye (rotation rows from LoadRotateMatrix, the position row through func_00256D18, w column last) and multiplied in place (a temp + copy); the
// frustum copy loops index the four planes with separate pointers; the wide copy continues at agFrustumXyz[4 * i] (agFrustumWideXyz directly follows
// agFrustumXyz) with the first loop's counter, so loop.c, not gcse, makes its start address (a plain pcm->agFrustumWideXyz is hoisted into an s register
// by PRE); the pointers of the wide planes are the PRE-hoisted copies the game has before the first BuildFrustrum call.
#include "sly2/basic.h"
#include "sly2/aseg.h"
#include "sly2/cm.h"
MATRIX3 LoadRotateMatrix(qword vec);
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR);
bool FInvertMatrix4(MATRIX4 *pmatSrc, MATRIX4 *pmatDst);
void BuildSimpleProjectionMatrix(float rx, float ry, float dxOffset, float dyOffset, float sFar, float sNear, MATRIX4 *pmat);
extern LM D_002BBB60;
extern CLOCK D_002DDED0;
struct CAMEVENT
{
      uint32_t   _pad_376    [ 3 ] ;
};
struct CAMPOSE
{
      VECTOR4 pos;
      VECTOR4 eul;
      float radFov;
};
struct CAMMOVE
{
      CAMPOSE *pposeFrom;
      CAMPOSE *pposeTo;
      float dt;
      int fCutOnLeave;
      int iCurve;
      CAMEVENT *pevent;
      int nUnk18;
};
extern "C" void CombineEyeLookAtProj(MATRIX4 *pmatEye, MATRIX4 *pmatProj, MATRIX4 *pmatOut);
extern "C" void BuildFrustrum(float gTanX, float gTanY, MATRIX4 *pmat, qword *paplane);
void UpdateCmVisZone(CM *pcm);
struct VU_FLOAT
{
    qword qw;
};
static inline qword QwAddScaled(qword qwBase, qword qwD, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwD), "j"(g.qw));
    return qw;
}
extern float D_002BD850;
extern float D_002BD854;
extern CLQ D_002BD860;
extern qword D_002BD870;
extern qword D_002BD880;
extern qword D_002BD890;
extern qword D_002BD8A0;
extern qword D_002A2DE0;
extern float *func_00256D18(float *pg, qword qw);
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
static inline VU_FLOAT VuFloatOfFloat(float g)
{
    VU_FLOAT v;
    __asm__("mfc1 %0, %1" : "=r"(v.qw) : "f"(g));
    return v;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
void UpdateCmMat4(CM *pcm) __asm__("func_0014DC58");
void UpdateCmMat4(CM *pcm)
{
    MATRIX4 matEye;
    float dtJolt = D_002DDED0.t - pcm->tJolt;
    if (dtJolt > D_002BD850)
    {
        CombineEyeLookAtProj(&pcm->mat, &pcm->matProj, &pcm->matViewProj);
    }
    else
    {
        float gEnv = GEvaluateClqLm(&D_002BD860, &D_002BBB60, dtJolt / D_002BD850);
        float gJolt = sinf(dtJolt * D_002BD854) * gEnv;
        {
            MATRIX3 matRot = LoadRotateMatrix(QwMulX(D_002BD880, VuFloatOfFloat(gJolt)));
            func_00256D18(matEye.mat[3], QwMulX(D_002BD870, VuFloatOfFloat(gJolt)));
            *(MATRIX3 *)&matEye = matRot;
        }
        matEye.mat[3][3] = 1.0f;
        matEye.mat[2][3] = 0.0f;
        matEye.mat[1][3] = 0.0f;
        matEye.mat[0][3] = 0.0f;
        matEye = pcm->mat * matEye;
        VECTOR4 vecRange;
        VECTOR4 vecOffset;
        MATRIX4 matProj;
        *(qword *)&vecRange = QwAddScaled(D_002A2DE0, D_002BD890, VuFloatOfFloat(gJolt));
        *(qword *)&vecOffset = QwMulX(D_002BD8A0, VuFloatOfFloat(gJolt));
        BuildSimpleProjectionMatrix(vecRange.x * 512.0f / (pcm->gTanHalfX * 4096.0f), vecRange.y * 224.0f / (pcm->gTanHalfY * 4096.0f),
                                    vecOffset.x, vecOffset.y, pcm->sNearClip, pcm->sFarClip, &matProj);
        CombineEyeLookAtProj(&matEye, &matProj, &pcm->matViewProj);
    }
    FInvertMatrix4(&pcm->matViewProj, &pcm->matViewProjInv);
    qword *paplane = pcm->aplaneFrustum;
    qword *paplaneWide = pcm->aplaneFrustumWide;
    float *pgWide1 = (float *)&paplaneWide[1];
    float *pgWide2 = (float *)&paplaneWide[2];
    BuildFrustrum(pcm->gTanHalfX, pcm->gTanHalfY, &pcm->mat, paplane);
    float *pgWide3 = (float *)&paplaneWide[3];
    float *pg0 = (float *)&paplane[0];
    float *pg3 = pg0 + 12;
    float *pgDst = pcm->agFrustumXyz;
    float *pg2 = pg0 + 8;
    float *pg1 = pg0 + 4;
    int i;
    for (i = 0; i < 3; i++)
    {
        pgDst[0] = *pg0++;
        pgDst[1] = *pg1++;
        pgDst[2] = *pg2++;
        pgDst[3] = *pg3++;
        pgDst += 4;
    }
    BuildFrustrum(pcm->gTanHalfX * 7.93f, pcm->gTanHalfY * 18.1f, &pcm->mat, paplaneWide);
    float *pgDstWide = &pcm->agFrustumXyz[4 * i];
    float *pgW3 = pgWide3;
    float *pgW2 = pgWide2;
    float *pgW1 = pgWide1;
    float *pgW0 = (float *)&paplaneWide[0];
    i = 2;
    do
    {
        pgDstWide[0] = *pgW0++;
        pgDstWide[1] = *pgW1++;
        pgDstWide[2] = *pgW2++;
        pgDstWide[3] = *pgW3++;
        pgDstWide += 4;
    } while (--i >= 0);
    UpdateCmVisZone(pcm);
}
