// func_001B20B8: ported from Lilly's personal Sly 2 decomp (src/P2/accmiss.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002DDED0 (g_clock)
// 0x001B20B8 VUnk69__6ACCMISS  vtable slot 69
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
extern CLOCK D_002DDED0;
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct BOMB : public SO
{
      undefined4 unk_0x150;
      float tDetonate;
      float svDetonate;
      undefined4 unk_0x15c[(0x1e8 - 0x15c) / 4];
      int nUnk1e8;
      undefined4 unk_0x1ec[(0x1f4 - 0x1ec) / 4];
      int fCustomXps;
      undefined4 unk_0x1f8[2];
      RWM *prwmOwner;
      undefined4 unk_0x204[3];
};
struct MISSILE : public BOMB
{
      LO *ploTarget;
      undefined4 unk_0x214[3];
      qword qwTarget;
      undefined4 unk_0x230[(0x260 - 0x230) / 4];
      float svLaunch;
      int nUnk264;
      undefined4 unk_0x268[2];
};
struct ACCMISS : public MISSILE
{
      float svTarget;
      float svFast;
      undefined4 unk_0x278[2];
    virtual void VUnk69() __asm__("func_001B20B8");
};
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
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLength(qword v)
{
    return GFromVf(QwLength(v));
}
extern "C" {
qword QwApplyConstrInXf(CONSTR *pconstr, XF *pxf, qword qw);
}
void AddSoVelocity(SO *pso, qword qwVel);
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
static inline qword QwDivFloat(qword qw, float g)
{
    VU_FLOAT vuf;
    qword qwRet;
    __asm__("qmtc2 %1, %0" : "=j"(vuf.qw) : "r"(g));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwRet) : "j"(vuf.qw), "j"(qw));
    return qwRet;
}
static inline qword QwDivBits(qword qw, int nBits)
{
    VU_FLOAT vuf;
    qword qwRet;
    __asm__("qmtc2 %1, %0" : "=j"(vuf.qw) : "rV"(nBits));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwRet) : "j"(vuf.qw), "j"(qw));
    return qwRet;
}
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
void ACCMISS::VUnk69()
{
    if (nUnk1e8 == 0)
    {
        qword qwVel = GetVelocity();
        float sv = GLength(qwVel);
        qword qwDir = QwDivFloat(qwVel, sv);
        SMP smp = { svFast, 0.0f, 1.0f };
        float svNew = GSmooth(sv, svTarget, D_002DDED0.dt, &smp, 0);
        qword qwDv = QwDivBits(QwSub(QwMulX(qwDir, VuFloat(svNew)), qwVel), *(int *)&D_002DDED0.dt);
        AddSoVelocity(this, QwApplyConstrInXf((CONSTR *)((char *)psophys + 0xB0), pxfUnk58, qwDv));
    }
}
