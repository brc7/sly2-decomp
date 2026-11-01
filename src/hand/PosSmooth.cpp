// PosSmooth: ported from Lilly's personal Sly 2 decomp (src/P2/util.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x0011F088 PosSmooth__FUxUxfP3SMPP6VECTOR  (T1258)
// GSmooth for a position (a 128-bit qword): the distance from posCur to posTarget is what GSmooth eases to zero (psmp's pace, dt seconds), the direction stays.
// Returns the new position (posCur + dpos * (the distance moved / the distance)); *pv (optional) gets the velocity it arrives with, in the same direction
// (dpos * (GSmooth's speed / the distance)). Closer than 0.0001: the position is posTarget and *pv is all zero (g_qwZero).
// Callers: STEPPLAYER::GetCpdefi 0x141640 (the focus offset at JT+0x1070 eases back to zero), UpdateJtAirInput (dposUnk1010).
// What decided the match (T1258, search over spellings; the first draft, a plain qword dpos, was 4%): dpos is a VU_VECTOR4 VALUE returned by SubPos (a memory
// temp) and qwDpos is a qword copy of it that every later use reads; the length is the dot product with the vector passed twice (QwLengthDot - with QwLength
// it is 51%): that gives the pseudo enough references for the register allocator to keep it in vf3 with a caller-save (sqc2 / lqc2 sp+0x40) around the
// GSmooth call, while the temp stays in memory (sp+0x30) for the reloads. Using dpos.qw again after the call, or without the copy, is 7%.
#include "sly2/basic.h"
struct VU_VECTOR4
{
    qword qw;
};
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline qword QwLengthDot(qword v)
{
    qword qw = QwDot(v, v);
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
extern qword D_002A2DD0;
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
    return qw;
}
static inline VU_VECTOR4 SubPos(qword a, qword b)
{
    VU_VECTOR4 r;
    r.qw = QwSub(a, b);
    return r;
}
qword PosSmooth(qword posCur, qword posTarget, float dt, SMP *psmp, VECTOR *pv) __asm__("PosSmooth");
qword PosSmooth(qword posCur, qword posTarget, float dt, SMP *psmp, VECTOR *pv)
{
    VU_VECTOR4 dpos = SubPos(posTarget, posCur);
    qword qwDpos = dpos.qw;
    float sDist = GFromVf(QwLengthDot(qwDpos));
    if (sDist > 0.0001f)
    {
        float svNext;
        float sNext = GSmooth(0.0f, sDist, dt, psmp, &svNext);
        if (pv != 0)
        {
            *(qword *)pv = QwMulX(qwDpos, VuFloat(svNext / sDist));
        }
        return QwAddScaled(posCur, qwDpos, VuFloat(sNext / sDist));
    }
    if (pv != 0)
    {
        *(qword *)pv = D_002A2DD0;
    }
    return posTarget;
}
