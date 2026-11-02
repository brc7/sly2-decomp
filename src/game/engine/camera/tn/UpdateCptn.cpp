// UpdateCptn: ported from Lilly's personal Sly 2 decomp (src/P2/cplcy.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DA0 (D_002A2DA0), D_002C4118 (D_002C4118), D_002C41B0 (D_002C41B0), D_002C41D8 (D_002C41D8), D_002C41DC (D_002C41DC), D_002C41E0 (D_002C41E0), D_002C41F0 (D_002C41F0), D_002C4200 (D_002C4200), D_002C4208 (D_002C4208), D_002C4220 (D_002C4220), D_002C4230 (D_002C4230), D_002C4240 (D_002C4240), D_002C4250 (D_002C4250), D_002C4260 (D_002C4260), D_002C426C (D_002C426C), D_002C4270 (D_002C4270), D_002C4274 (D_002C4274), D_002C4278 (D_002C4278), D_002C427C (D_002C427C), D_002C4280 (D_002C4280), D_002C41EC (g_SheetCameraEngineCameraDamping), D_002C41E4 (g_SheetCameraEngineCameraHorizontalSpeed), D_002C41E8 (g_SheetCameraEngineCameraVerticalSpeed), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DEB30 (g_game), D_002BBB60 (g_lmZeroOne), D_002A2DC0 (g_normalZ), D_002A2DD0 (g_qwZero)
// 0x00221DD8 Update__7CMPART2P6CPDEFIP3JOY  (MATCHES Sly 2; T1253, 1604 instructions). The frame is the game's 0x230 (0x00 posFocus, 0x10 posHang,
// 0x20 vecTangent, 0x30 dposCrv / vFocus, 0x40..0x60 the free camera's temporaries, 0x70 smp, 0x80 vecHeading / smpPull, 0x90..0xEF six untouched
// locals, 0xF0.. the view floats, 0x124 / 0x128 the pull flags; T1203). How the last 16.5% -> MATCH went (re/notes/camera-updatecm.md "T1253"):
//  - The T1203 blocker (hi(g_clock) in $fp, plo spilled) is a global.c priority TIE, not a spelling of the clock reads: the merged hi(g_clock)
//    pseudo (13 refs, REG_EQUIV so its live length is doubled) had priority (int)(3*13/2404*10000) = 162 against plo's (int)(2*4/495*10000) = 161.
//    With equal priorities the lower pseudo (plo) is allocated first. haifa-sched recomputes REG_LIVE_LENGTH after sched1, so the lengths are
//    in the scheduled order. The free camera's turn written as a running product (dradPanFree *= ...) with GEvaluateClqLm limiting in place
//    shortens plo's life to 491 -> 162 = tie -> plo gets $fp and every clock read re-does lui / addiu, as in the game.
//  - Then: `CM *pcm = &g_cm` + a fallback pointer for vecView (the game's early `addiu v1, s1, 0x2DA0`); `qword *pdposFocus = &g_cm.dposFocus`
//    (lqc2 0(v0) after addiu -0x1DE0); the pull's length through GXOfQw (reload's qmfc2 + asm mtc1 schedule after the clqPull address);
//    ptn->pshape read only after the fEntered test; tManual stored before fArrived; the idle ramp's 0 in an else arm; the enter ramp's product
//    in its own variable (gEnter); the time-since-manual factor through GLimitRange (gMin in f6, gMax in f7).
// Everything else was found in T1146 / T1203: the two normalise shapes (A: register result, fallback lq before the branch, QwLengthUntied; B: QwToVu,
// the arms store to the VECTOR4), GMin for min.s, the switch on nMode, the chain assignments, g_clock.dt re-read at every use, the pull-vFocus
// fallback g_qwZero.
// What it does, in order:
//  1. UpdateCmLineOfSight; the tunnel's flags (grftnm: 1 forward view allowed, 2 backward view, 4 the player may turn; no tunnel = 4 only);
//     UpdateCmProgress + CalcCpdFocus give the focus; a player character whose grab record focuses (jtgrab.jtfock Focus, STEPPLAYER+0x4F4: sliding along a
//     hide outline in the grab's own tunnel, SetJtHfdk 0x1940B0) springs g_cm.dposFocus toward (jtgrab.qwPosFocus (+0x500) - focus);
//     posHang = focus + g_cm.dposFocus is what the camera looks at.
//  2. uTunnel (how much the tunnel steers) smooths toward CPD::uTunnel, or 0 while the focus is not moving by itself (CPDEFI bit 1; a tunnel with
//     ploPull forces the bit off); tMoveLast keeps the focus's latest tMoveLast.
//  3. In a tunnel with a forward / backward view: CalculateTnCrv gives the focus's offsets from the curve (cylindrical: the pan is carried from last
//     frame limited by TN::radPanLimit); CalculateTnPos with the CPD's offsets (ahead by dsOffset / behind) gives the two views as pan / tilt /
//     distance from posHang (DecomposeSphere) -> radPanFwd.. / radPanRev..
//  4. A cut (g_cm.nCut): nCut 2 (CutCmPanTilt) continues from g_cm.radCutPan / radCutTilt (less the framing tilt); outside a tunnel view the pan is
//     CPDEFI::radHome (the hero's heading) and the tilt CPD::lmTilt.gMin; else the nearer tunnel view becomes the mode (1 / 2). A mode its tunnel
//     does not allow falls back to 4 (free).
//  5. vFocus = the focus's velocity (CPDEFI::vBase), or for a tunnel with ploPull the unit direction to that object (z dropped) scaled by
//     clqPull(distance) limited by lmPull.
//  6. Player control (grftnm & 4) when the right stick is pushed or still turning or recentering: the stick's x|x| * speed and y|y| * speed become
//     the turn rates, changing at most damping * dt per frame; a stick within 15 deg of horizontal does not tilt, within 15 deg of vertical does not
//     pan; game event 6 at most every 5 s (GAME::tCmTurned, 0x2DEB34). fRecenter: RadSmoothA toward radPanRecenter / radTiltRecenter (SMPA 0x2C4250 while the
//     player's +0x10E4 is 4, else 0x2C4240) until both arrive. Taking over from a tunnel view plays sound 3. Then nMode = 4, fArrived, tManual = now.
//  7. Otherwise, in the free mode (4): the hero's movement turns the camera: vFocus dotted with the turn axis (TN::nTurnAxis / CPDEFI::nTurnAxis: 0 = up x
//     the direction camera -> posHang with its z scaled by 0.3, 1 = that direction; both normalised, (1,0,0) when degenerate), over |that direction|,
//     limited to 2.5, times uTunnel, times CLQ {1,-2,-2}(left stick y) in 1..2, times the ramp 0..1 of the seconds since the player let go, times dt.
//     The tilt eases back into CPD::lmTilt (RadSmooth, SMP {0.75,0,2} x speed) once the hero moves faster than 60 u/s (speed / 600 capped at 1 > 0.1).
//     With tunnel views: the view to pull toward is the forward one (plain zones, mode 1; the player's own tunnel +0x528 with blend 3.0), the backward
//     one (mode 2), else by the hero's facing (PmatLoWorld row 0, z dropped) against the tunnel's direction (XfmVecLoWorld): blend = g^2 (3 - 2|g|),
//     backward when the backward view is allowed and g <= 0. The pull strength = (speed / 600, or / 200 while +0x4CC == -1 / +0x4C8, capped at 1)
//     x CPD::gTurn; fPullIdle raises it to gPullIdle x ramps of (idle 0.75 s, no player turn 10 s) over dtPullIdle; fPullEnter (until fArrived) to
//     gPullEnter x the ramp since the tunnel was entered (tTns) over dtPullEnter. The pulled pan / tilt = RadSmooth toward the view (SMP {0.5,0,0.5} x
//     strength), or the free turn's if that is already nearer; the result blends free -> pulled by the blend. Reaching the view (crossing it, within
//     90 deg) sets nMode 1 / 2 and fArrived (not on the first frame, nor in a shape tunnel with +0xE0 == 0 and +0xE8 > 2).
//  8. The output pan / tilt / distance / framing: mode 1 / 2 take the tunnel view and CPD::uFrameX (negated backward); mode 4 the turned pan / tilt with
//     |CPD::dsOffset| as the distance outside tunnel views, else distance / uFrameX interpolated between the backward and forward views by where the
//     pan lies between their pans (smoothstep, pans unwrapped). The distance and uFrameY spring to the wanted values (k = CPD::kDist * 15 * dt,
//     c = CPD::cDist * -8 * dt + 1) unless cutting (velocities zeroed).
//  9. posEyeWant = posHang + VuvecSphere(pan, tilt, dist); FreeCamClip on a cut, ClipCmEye normally (D_002C4118 = no clipping; a tunnel's fNoCamClip
//     skips both); the angles are re-derived from the clipped eye; the eye handed to SetCmPanTilt is pushed back by sNearClip along the view;
//     the framing offsets atan(uFrameX * gTanHalfX) / -atan(uFrameY * gTanHalfY) plus the look offsets (first frame: the difference between the camera's
//     current pan / tilt (DecomposeRotateMatrixPanTilt of g_cm.matRot) and the new view, so there is no jump; later sprung back to 0 with k -10, c -5).
// 10. The members keep the frame's pan / tilt / distance / framing and the two tunnel views; fEntered is cleared.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/cm.h"
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
qword PosLoWorld(LO *plo);
qword XfmVecLoWorld(LO *ploFrom, LO *ploTo, qword qwVec);
MATRIX4 *PmatLoWorld(LO *plo);
void DecomposeRotateMatrixPanTilt(MATRIX3 *pmat, float *pradPan, float *pradTilt);
static inline float GEvaluateClq(CLQ *pclq, float x)
{
    return pclq->g0 + x * (pclq->g1 + x * pclq->g2);
}
extern LM D_002BBB60;
float RadNormalize(float rad);
float GLimitAbs(float g, float absLimit);
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
float RadSmooth(float radCur, float radTarget, float dt, SMP *psmp, float *pdradNext);
float RadSmoothA(float radCur, float dradCur, float radTarget, float dt, SMPA *psmpa, float *pdradNext);
float GLimitLm(LM *plm, float g);
struct GAME
{
      float tIdleEvent;
      float tCmTurned;
      int nTaskStateDenied;
      int idGadgetLast;
      int oidJobCur;
      int oidTaskCur;
      int oidTaskPending;
};
extern GAME D_002DEB30;
SND *MakeSnd(int sfxid);
void SetSndOwner(SND *psnd, LO *ploOwner);
extern CLOCK D_002DDED0;
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
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct RUMINS
{
      byte fHighSpeedMotor;
      byte bLowSpeedMotor;
      byte unk1;
      byte unk2;
      float dt;
};
struct RUMPAT
{
      int crumins;
      RUMINS arumins[32];
};
struct RUMBLE
{
      int nPort;
      int nSlot;
      RUMS rums;
      RUMPAT *prumpat;
      int irumins;
      float dtRumble;
      float dtRumins;
};
struct JOY
{
      byte abPadDma[0x1000];
      int nPort;
      int nSlot;
      int term;
      JOYS joys;
      JOYK joyk;
      float tJoys;
      float tRead;
      float tActive;
      int dtIdle;
      undefined4 unk_0x1024;
      undefined4 unk_0x1028;
      undefined4 unk_0x102c[4];
      float dxLeft;
      float dyLeft;
      float uDeflectLeft;
      byte bXLeft;
      byte bYLeft;
      ushort unk_0x104a;
      int fStickMovedLeft;
      LM almDeflect[4];
      float dxRight;
      float dyRight;
      float sRight;
      byte bXRight;
      byte bYRight;
      ushort unk_0x107e;
      int fStickMovedRight;
      LM almDeflect2[4];
      float dxdtLeft;
      float dydtLeft;
      float dxdtRight;
      float dydtRight;
      uint grfbtn;
      uint grfbtnPressed;
      uint grfbtnLatched;
      byte mpbtnpb[12];
      RUMBLE rumble;
      int fRumbleEnabled;
      float uDpad;
      undefined4 unk_0x10f0[4];
};
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
extern CM D_002DDF40;
void SpringCm(CM *pcm, qword *pqwPos, VECTOR *pvVel, qword qwTarget, float k, float c);
void SetCmPanTilt(CM *pcm, qword posEye, qword posCenter, float radPan, float radTilt);
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
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwLengthUntied(qword v)
{
    qword qwSq;
    qword vfOne;
    qword qwLen;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qwSq), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLen) : "j"(qwSq));
    return qwLen;
}
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline float GXOfQw(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
static inline qword QwCross(qword a, qword b)
{
    qword qw;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}
struct CPD
{
      CID cid;
      float dsOffset;
      undefined4 unk_0x08[(0x10 - 0x08) / 4];
      CLQ aclqOffset[3];
      LM almOffset[3];
      float uFrameX;
      float uFrameY;
      float gWeight;
      float uTunnel;
      float gTurn;
      undefined4 unk_0x6c;
      float agRate[3];
      undefined4 unk_0x7c;
      float kDist;
      float cDist;
      LM lmTilt;
} __attribute__((aligned(16)));
void UpdateCmProgress(CMPART2 *pcmpart, CPDEFI *pcpdefi);
void CalcCpdFocus(CPD *pcpd, CPDEFI *pcpdefi, qword *ppos, TN *ptn);
void CalculateTnCrv(TN *ptn, qword qwPos, VECTOR4 *pdposCrv, VECTOR4 *pvecTangent);
void CalculateTnPos(TN *ptn, VECTOR4 *pdposCrv, float sBase, float dsOffset, CLQ *aclq, LM *alm, FTND ftnd, qword *pposOther);
extern "C" {
float atanf(float x);
}
static inline float GLimitRange(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
float atan2f(float y, float x);
void DecomposeSphere(qword v, float *prad, float *pradTilt, float *psLen);
extern "C" qword VuvecSphere(float radPan, float radTilt, float s);
extern float D_002C41DC;
extern VECTOR D_002A2DC0;
extern qword D_002A2DD0;
struct KEY
{
    static void *operator new(uint cb)
    {
        SWALLOC swalloc;
        swalloc.cb = cb;
        SWALLOC *pswalloc = &swalloc;
        pswalloc->cbAlign = 0x10;
        return PvAllocSwClearImpl(pswalloc);
    }
};
struct CHN
{
      int oid : 20;
      int ixf : 10;
      uint fStripped : 1;
      uint fScalar : 1;
      KEY *pkey;
};
struct OVR
{
      int id;
      OID oid;
      KEY *pkey;
};
struct EASND
{
      int sfxid;
      OID oid;
      int sNear;
      int sFar;
      float gPitch;
      float gUnk14;
      float gUnk18;
};
struct EA17
{
      int n0;
      int n4;
      int n8;
      float gC;
};
struct EA20
{
      int n0;
      int n4;
      float g8;
};
struct EA24
{
      RAW8 a[3];
};
struct MRSGC
{
      int ib;
      int cmrsg;
      MRSG *amrsg;
};
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
void OnGameEvent(GAME *pgame, int nEvent, LO *plo);
void DecomposeCylind(qword v, float *prad, float *psXY, float *pz);
qword ClipCmEye(CM *pcm, qword posFrom, qword posTo);
void FreeCamClip(CMPART2 *pcmpart, qword posFocus, qword posEye, qword *pposOut);
void UpdateCmLineOfSight(CMPART2 *pcmpart);
extern qword D_002A2DA0;
extern int D_002C4118;
extern float D_002C41D8;
extern float D_002C41E0;
extern CLQ D_002C41F0;
extern LM D_002C4200;
extern SMP D_002C4208;
extern CLQ D_002C4220;
extern SMP D_002C4230;
extern SMPA D_002C4240;
extern SMPA D_002C4250;
extern SMP D_002C4260;
extern float D_002C426C;
extern float D_002C4270;
extern float D_002C4274;
extern float D_002C4278;
extern float D_002C427C;
extern float D_002C4280;
extern LM D_002C41B0;
extern float D_002C41E4;
extern float D_002C41E8;
extern float D_002C41EC;
static inline qword QwToVu(qword qw)
{
    qword qwOut;
    __asm__("qmtc2 %1, %0" : "=j"(qwOut) : "r"(qw));
    return qwOut;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = GEvaluateClq(pclq, x);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    if (g < gMin)
        g = gMin;
    else if (gMax < g)
        g = gMax;
    return g;
}
static inline float GMin(float a, float b)
{
    return a < b ? a : b;
}
static inline float GSmoothStep(float u)
{
    return u * u * (3.0f - (u + u));
}
void CMPART2::Update(CPDEFI *pcpdefi, JOY *pjoy)
{
    LO *plo = (LO *)pvHero;
    TN *ptnCur = ptn;
    float dt = D_002DDED0.dtCm;
    LO *ploPlayer = 0;
    if (FIsBasicDerivedFrom((BASIC *)plo, (CID)0x34))
        ploPlayer = plo;
    UpdateCmLineOfSight(this);
    int grftnm;
    int fPullEnter;
    int fPullIdle;
    if (ptnCur != 0)
    {
        grftnm = ptnCur->grftnm;
        fPullEnter = ptnCur->fPullEnter;
        fPullIdle = ptnCur->fPullIdle;
    }
    else
    {
        fPullIdle = 0;
        grftnm = 4;
        fPullEnter = 0;
    }
    UpdateCmProgress(this, pcpdefi);
    VECTOR4 posFocus;
    CalcCpdFocus(pcpd, pcpdefi, (qword *)&posFocus, ptnCur);
    if (ploPlayer != 0 && ((STEPPLAYER *)ploPlayer)->jtgrab.jtfock == JTFOCK_Focus)
        SpringCm(&D_002DDF40, &D_002DDF40.dposFocus, &D_002DDF40.vFocus, QwSub(((STEPPLAYER *)ploPlayer)->jtgrab.qwPosFocus, *(qword *)&posFocus), 8.0f, -6.0f);
    VECTOR4 posHang;
    qword *pdposFocus = &D_002DDF40.dposFocus;
    *(qword *)&posHang = QwAdd(*(qword *)&posFocus, *pdposFocus);
    if (ptnCur != 0 && ptnCur->ploPull != 0)
        pcpdefi->grfcpd &= ~1;
    float uTunnelWant;
    if ((pcpdefi->grfcpd & 1) == 0)
        uTunnelWant = pcpd->uTunnel;
    else
        uTunnelWant = 0.0f;
    uTunnel = GSmooth(uTunnel, uTunnelWant, D_002DDED0.dt, &D_002C4230, 0);
    tMoveLast = tMoveLast > pcpdefi->tMoveLast ? tMoveLast : pcpdefi->tMoveLast;
    float radPanFwdNew = 0.0f;
    float radTiltFwdNew = 0.0f;
    float sDistFwd;
    float radPanRevNew = 0.0f;
    float radTiltRevNew = 0.0f;
    float sDistRev;
    VECTOR4 vecTangent;
    if (ptnCur != 0 && (grftnm & 3) != 0)
    {
        VECTOR4 dposCrv;
        CalculateTnCrv(ptnCur, *(qword *)&posFocus, &dposCrv, &vecTangent);
        if (ptnCur->fCylind)
        {
            DecomposeCylind(*(qword *)&dposCrv, &dposCrv.x, &dposCrv.y, &dposCrv.z);
            if (!fEntered)
                dposCrv.x += GLimitAbs(RadNormalize(radPanCrv - dposCrv.x), ptnCur->radPanLimit);
            radPanCrv = dposCrv.x;
        }
        if (grftnm & 1)
        {
            VECTOR4 posView;
            CalculateTnPos(ptnCur, &dposCrv, ptnCur->uProgress, pcpd->dsOffset, pcpd->aclqOffset, pcpd->almOffset, FTND_Forward, (qword *)&posView);
            DecomposeSphere(QwSub(*(qword *)&posView, *(qword *)&posHang), &radPanFwdNew, &radTiltFwdNew, &sDistFwd);
        }
        if (grftnm & 2)
        {
            VECTOR4 posView;
            CalculateTnPos(ptnCur, &dposCrv, ptnCur->uProgress, pcpd->dsOffset, pcpd->aclqOffset, pcpd->almOffset, FTND_Reverse, (qword *)&posView);
            DecomposeSphere(QwSub(*(qword *)&posView, *(qword *)&posHang), &radPanRevNew, &radTiltRevNew, &sDistRev);
        }
    }
    if (fEntered)
        nMode = 4;
    if (D_002DDF40.nCut != 0)
    {
        if (D_002DDF40.nCut == 2)
        {
            nMode = 4;
            radPan = D_002DDF40.radCutPan;
            float radTiltCut = D_002DDF40.radCutTilt - atanf(pcpd->uFrameY * D_002DDF40.gTanHalfY);
            radTilt = GLimitLm(&D_002C41B0, radTiltCut);
        }
        else if ((grftnm & 3) == 0)
        {
            nMode = 4;
            radPan = pcpdefi->radHome;
            radTilt = pcpd->lmTilt.gMin;
        }
        else if ((grftnm & 2) == 0)
        {
            nMode = 1;
        }
        else if ((grftnm & 1) == 0)
        {
            nMode = 2;
        }
        else
        {
            float dradFwd = RadNormalize(radPanFwdNew - pcpdefi->radHome);
            float dradRev = RadNormalize(radPanRevNew - pcpdefi->radHome);
            if (fabsf(dradFwd) < fabsf(dradRev))
                nMode = 1;
            else
                nMode = 2;
        }
        tMoveLast = D_002DDED0.t;
        tManual = D_002DDED0.t - D_002C41DC;
    }
    if ((nMode & grftnm) == 0 && nMode > 0 && nMode < 3)
        nMode = 4;
    VECTOR4 vFocus;
    if (ptnCur != 0 && ptnCur->ploPull != 0)
    {
        VECTOR4 dpos;
        *(qword *)&dpos = QwSub(PosLoWorld(ptnCur->ploPull), *(qword *)&pcpdefi->posBase);
        dpos.z = 0.0f;
        qword *pqwFallback = &D_002A2DD0;
        qword qwDir = QwToVu(*(qword *)&dpos);
        qword qwLen = QwLengthUntied(qwDir);
        if (GFromVf(qwLen) < 0.0001f)
        {
            *(qword *)&dpos = *pqwFallback;
        }
        else
        {
            __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLen));
            qword qwNormal;
            __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwNormal) : "j"(qwDir));
            *(qword *)&dpos = qwNormal;
        }
        float gPull = GEvaluateClqLm(&ptnCur->clqPull, &ptnCur->lmPull, GXOfQw(qwLen));
        *(qword *)&vFocus = QwMulX(*(qword *)&dpos, VuFloat(gPull));
    }
    else
    {
        *(qword *)&vFocus = *(qword *)&pcpdefi->vBase;
    }
    float radPanNew = radPan;
    float radTiltNew = radTilt;
    if ((grftnm & 4) != 0 && (fRecenter != 0 || pjoy->sRight > 0.0f || dradPan != 0.0f || dradTilt != 0.0f))
    {
        if (nMode != 4)
        {
            SND *psnd = MakeSnd(3);
            SetSndOwner(psnd, 0);
        }
        if (pjoy->sRight > 0.0f)
            fRecenter = 0;
        if (fRecenter != 0)
        {
            SMPA *psmpa;
            if (ploPlayer != 0 && (*(  int  *)((uint8_t *)( ploPlayer ) + (  0x10E4 )))  == 4)
                psmpa = &D_002C4250;
            else
                psmpa = &D_002C4240;
            radPanNew = RadSmoothA(radPanNew, dradPan, radPanRecenter, dt, psmpa, &dradPan);
            radTiltNew = RadSmoothA(radTiltNew, dradTilt, radTiltRecenter, dt, psmpa, &dradTilt);
            if (radPanNew == radPanRecenter && radTiltNew == radTiltRecenter)
                fRecenter = 0;
        }
        else
        {
            float dradMax = D_002C41EC * dt;
            float dradPanWant = pjoy->dxRight * fabsf(pjoy->dxRight) * D_002C41E4;
            float dradTiltWant = pjoy->dyRight * fabsf(pjoy->dyRight) * D_002C41E8;
            dradPan = GLimitRange(dradPanWant, dradPan - dradMax, dradPan + dradMax);
            dradTilt = GLimitRange(dradTiltWant, dradTilt - dradMax, dradTilt + dradMax);
            float radStick = atan2f(fabsf(pjoy->dyRight), fabsf(pjoy->dxRight));
            if (radStick < 0.2617994f)
                dradTilt = 0.0f;
            else if (radStick > 1.3089969f)
                dradPan = 0.0f;
            radPanNew = RadNormalize(radPanNew + dradPan * dt);
            radTiltNew = RadNormalize(radTiltNew + dradTilt * dt);
            radTiltNew = GLimitLm(&D_002C41B0, radTiltNew);
            if (D_002DDED0.tCm - D_002DEB30.tCmTurned > 5.0f)
            {
                D_002DEB30.tCmTurned = D_002DDED0.tCm;
                OnGameEvent(&D_002DEB30, 6, 0);
            }
        }
        nMode = 4;
        tManual = D_002DDED0.t;
        fArrived = 1;
    }
    else if (nMode == 4)
    {
        VECTOR4 vecStick;
        vecStick.x = pjoy->dxLeft;
        vecStick.y = pjoy->dyLeft;
        vecStick.z = 0.0f;
        qword qwFallbackStick = D_002A2DA0;
        qword qwStick = *(qword *)&vecStick;
        qword qwLenStick = QwLengthUntied(qwStick);
        if (GFromVf(qwLenStick) < 0.0001f)
        {
            qwStick = qwFallbackStick;
        }
        else
        {
            __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLenStick));
            __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwStick) : "j"(qwStick));
        }
        *(qword *)&vecStick = qwStick;
        VECTOR4 vecView;
        CM *pcm = &D_002DDF40;
        qword *pqwFallbackView = &D_002A2DA0;
        *(qword *)&vecView = QwSub(*(qword *)&posHang, *(qword *)&pcm->pos);
        vecView.z *= 0.3f;
        qword qwView = QwToVu(*(qword *)&vecView);
        qword qwLenView = QwLengthUntied(qwView);
        if (GFromVf(qwLenView) < 0.0001f)
        {
            *(qword *)&vecView = *pqwFallbackView;
        }
        else
        {
            __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLenView));
            qword qwNormal;
            __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwNormal) : "j"(qwView));
            *(qword *)&vecView = qwNormal;
        }
        float sView = GFromVf(qwLenView);
        int nTurnAxis;
        if (ptnCur != 0 && ptnCur->nTurnAxis != -1)
            nTurnAxis = ptnCur->nTurnAxis;
        else
            nTurnAxis = pcpdefi->nTurnAxis;
        VECTOR4 vecAxis;
        switch (nTurnAxis)
        {
        case 0:
            *(qword *)&vecAxis = QwCross(*(qword *)&D_002A2DC0, *(qword *)&vecView);
            break;
        case 1:
            vecAxis = vecView;
            break;
        }
        qword qwFallbackAxis = D_002A2DA0;
        qword qwAxis = *(qword *)&vecAxis;
        qword qwLenAxis = QwLengthUntied(qwAxis);
        if (GFromVf(qwLenAxis) < 0.0001f)
        {
            qwAxis = qwFallbackAxis;
        }
        else
        {
            __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLenAxis));
            __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwAxis) : "j"(qwAxis));
        }
        *(qword *)&vecAxis = qwAxis;
        float gTurn = GLimitAbs(GFromVf(QwDot(*(qword *)&vFocus, qwAxis)) / sView, D_002C41E0);
        float dradPanFree = gTurn * uTunnel;
        dradPanFree *= GEvaluateClqLm(&D_002C41F0, &D_002C4200, vecStick.y);
        dradPanFree *= GLimitRange(GEvaluateClq(&D_002C4220, D_002DDED0.t - tManual), D_002BBB60.gMin, D_002BBB60.gMax);
        float radPanFree = RadNormalize(radPanNew + dradPanFree * D_002DDED0.dt);
        float uSpeed = GMin(GFromVf(QwLengthXY(*(qword *)&vFocus)) * 0.0016666667f, 1.0f);
        float radTiltFree;
        SMP smp;
        if (uSpeed > 0.1f)
        {
            smp.svFast = D_002C4260.svFast * uSpeed;
            smp.svSlow = D_002C4260.svSlow * uSpeed;
            smp.dtFast = D_002C4260.dtFast;
            radTiltFree = RadSmooth(radTiltNew, GLimitLm(&pcpd->lmTilt, radTiltNew), D_002DDED0.dt, &smp, 0);
        }
        else
        {
            radTiltFree = radTiltNew;
        }
        if ((grftnm & 3) == 0)
        {
            radPanNew = radPanFree;
            radTiltNew = radTiltFree;
        }
        else
        {
            float gBlend;
            float radPanView;
            float radTiltView;
            if (ptnCur->pshape == 0 && ptnCur->fCircle == 0)
            {
                gBlend = 1.0f;
                radPanView = radPanFwdNew;
                radTiltView = radTiltFwdNew;
            }
            else if (ploPlayer != 0 && ptnCur == (*(  TN *  *)((uint8_t *)( ploPlayer ) + (  0x528 ))) )
            {
                gBlend = 3.0f;
                radPanView = radPanFwdNew;
                radTiltView = radTiltFwdNew;
            }
            else if (nMode == 1)
            {
                gBlend = 1.0f;
                radPanView = radPanFwdNew;
                radTiltView = radTiltFwdNew;
            }
            else if (nMode == 2)
            {
                gBlend = 1.0f;
                radPanView = radPanRevNew;
                radTiltView = radTiltRevNew;
            }
            else
            {
                VECTOR4 vecHeading;
                *(qword *)&vecHeading = *(qword *)PmatLoWorld(plo)->mat[0];
                vecHeading.z = 0.0f;
                qword qwFallbackHeading = D_002A2DA0;
                qword qwHeadingRaw = *(qword *)&vecHeading;
                qword qwLenHeading = QwLengthUntied(qwHeadingRaw);
                qword qwHeading;
                if (GFromVf(qwLenHeading) < 0.0001f)
                {
                    qwHeading = qwFallbackHeading;
                }
                else
                {
                    __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLenHeading));
                    __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwHeading) : "j"(qwHeadingRaw));
                }
                *(qword *)&vecHeading = qwHeading;
                *(qword *)&vecTangent = XfmVecLoWorld(ptnCur, 0, *(qword *)&vecTangent);
                float gAlong = GFromVf(QwDot(*(qword *)&vecHeading, *(qword *)&vecTangent));
                gBlend = gAlong * gAlong * (3.0f - (fabsf(gAlong) + fabsf(gAlong)));
                if ((grftnm & 2) == 0 || 0.0f < gAlong)
                {
                    radPanView = radPanFwdNew;
                    radTiltView = radTiltFwdNew;
                }
                else
                {
                    radPanView = radPanRevNew;
                    radTiltView = radTiltRevNew;
                }
            }
            float gPull;
            if (ploPlayer != 0)
            {
                if ((*(  int  *)((uint8_t *)( ploPlayer ) + (  0x4CC )))  != -1 && (*(  int  *)((uint8_t *)( ploPlayer ) + (  0x4C8 )))  == 0)
                    gPull = GMin(GFromVf(QwLengthXY(*(qword *)&vFocus)) * 0.005f, 1.0f);
                else
                    gPull = GMin(GFromVf(QwLengthXY(*(qword *)&vFocus)) * 0.0016666667f, 1.0f);
            }
            else
            {
                gPull = 0.0f;
            }
            gPull *= pcpd->gTurn;
            if (fPullIdle)
            {
                float gRamp;
                float dtIdle = D_002DDED0.t - (tMoveLast + D_002C41D8);
                float dtQuiet = D_002DDED0.t - (tManual + D_002C41DC);
                if (ptnCur->dtPullIdle > 0.0f)
                    gRamp = GLimitLm(&D_002BBB60, dtIdle / ptnCur->dtPullIdle) * GLimitLm(&D_002BBB60, dtQuiet / ptnCur->dtPullIdle);
                else if (dtIdle > 0.0f && dtQuiet > 0.0f)
                    gRamp = 1.0f;
                else
                    gRamp = 0.0f;
                gRamp *= ptnCur->gPullIdle;
                if (gPull * gBlend < gRamp)
                {
                    gBlend = 1.0f;
                    gPull = gRamp;
                }
            }
            if (fPullEnter && fArrived == 0)
            {
                float gRamp;
                if (ptnCur->dtPullEnter > 0.0f)
                    gRamp = GLimitLm(&D_002BBB60, (D_002DDED0.t - ptnCur->tTns) / ptnCur->dtPullEnter);
                else
                    gRamp = 1.0f;
                float gEnter = gRamp * ptnCur->gPullEnter;
                if (gPull * gBlend < gEnter)
                {
                    gBlend = 1.0f;
                    gPull = gEnter;
                }
            }
            SMP smpPull;
            smpPull.svFast = D_002C4208.svFast * gPull;
            smpPull.svSlow = D_002C4208.svSlow * gPull;
            smpPull.dtFast = D_002C4208.dtFast;
            float radPanPull = RadSmooth(radPanNew, RadNormalize(radPanNew + RadNormalize(radPanView - radPanNew)), D_002DDED0.dt, &smpPull, 0);
            float radTiltPull = RadSmooth(radTiltNew, RadNormalize(radTiltNew + RadNormalize(radTiltView - radTiltNew)), D_002DDED0.dt, &smpPull, 0);
            if (fabsf(RadNormalize(radPanView - radPanPull)) > fabsf(RadNormalize(radPanView - radPanFree)))
                radPanPull = radPanFree;
            if (fabsf(RadNormalize(radTiltView - radTiltPull)) > fabsf(RadNormalize(radTiltView - radTiltFree)))
                radTiltPull = radTiltFree;
            radPanNew = RadNormalize(radPanNew + (1.0f - gBlend) * RadNormalize(radPanFree - radPanNew) + gBlend * RadNormalize(radPanPull - radPanNew));
            radTiltNew = RadNormalize(radTiltNew + (1.0f - gBlend) * RadNormalize(radTiltFree - radTiltNew) + gBlend * RadNormalize(radTiltPull - radTiltNew));
            if (!fEntered && !(ptn->pshape != 0 && (*(  int  *)((uint8_t *)( ptn->pshape ) + (  0xE0 )))  == 0 && (*(  int  *)((uint8_t *)( ptn->pshape ) + (  0xE8 )))  > 2))
            {
                if ((grftnm & 1) != 0 &&
                    (RadNormalize(radPan - radPanFwd) * RadNormalize(radPanNew - radPanFwdNew) <= 0.0f || fabsf(RadNormalize(radPanNew - radPanFwdNew)) < 0.0001f) &&
                    fabsf(RadNormalize(radPanNew - radPanFwdNew)) < 1.5707964f &&
                    (RadNormalize(radTilt - radTiltFwd) * RadNormalize(radTiltNew - radTiltFwdNew) <= 0.0f || fabsf(RadNormalize(radTiltNew - radTiltFwdNew)) < 0.0001f) &&
                    fabsf(RadNormalize(radTiltNew - radTiltFwdNew)) < 1.5707964f)
                {
                    nMode = 1;
                    fArrived = 1;
                }
                else if ((grftnm & 2) != 0 &&
                         (RadNormalize(radPan - radPanRev) * RadNormalize(radPanNew - radPanRevNew) <= 0.0f || fabsf(RadNormalize(radPanNew - radPanRevNew)) < 0.0001f) &&
                         fabsf(RadNormalize(radPanNew - radPanRevNew)) < 1.5707964f &&
                         (RadNormalize(radTilt - radTiltRev) * RadNormalize(radTiltNew - radTiltRevNew) <= 0.0f || fabsf(RadNormalize(radTiltNew - radTiltRevNew)) < 0.0001f) &&
                         fabsf(RadNormalize(radTiltNew - radTiltRevNew)) < 1.5707964f)
                {
                    nMode = 2;
                    fArrived = 1;
                }
            }
        }
    }
    VECTOR4 avecUnused[6];
    float radPanOut;
    float radTiltOut;
    float sDistOut;
    float uFrameYWant;
    float uFrameXWant;
    switch (nMode)
    {
    case 1:
        radPanOut = radPanFwdNew;
        radTiltOut = radTiltFwdNew;
        sDistOut = sDistFwd;
        uFrameXWant = pcpd->uFrameX;
        uFrameYWant = pcpd->uFrameY;
        break;
    case 2:
        radPanOut = radPanRevNew;
        radTiltOut = radTiltRevNew;
        sDistOut = sDistRev;
        uFrameXWant = -pcpd->uFrameX;
        uFrameYWant = pcpd->uFrameY;
        break;
    case 4:
        radPanOut = radPanNew;
        radTiltOut = radTiltNew;
        if ((grftnm & 3) == 0)
        {
            uFrameYWant = pcpd->uFrameY;
            sDistOut = fabsf(pcpd->dsOffset);
            uFrameXWant = pcpd->uFrameX;
        }
        else
        {
            float u;
            if ((grftnm & 2) == 0)
            {
                u = 1.0f;
            }
            else if ((grftnm & 1) == 0)
            {
                u = 0.0f;
            }
            else
            {
                if (radPanFwdNew > radPan && radPanRevNew > radPan)
                {
                    if (radPanRevNew < radPanFwdNew)
                        radPanFwdNew -= 6.2831855f;
                    else
                        radPanRevNew -= 6.2831855f;
                }
                else if (radPanFwdNew < radPan && radPanRevNew < radPan)
                {
                    if (radPanFwdNew < radPanRevNew)
                        radPanFwdNew += 6.2831855f;
                    else
                        radPanRevNew += 6.2831855f;
                }
                u = (radPan - radPanRevNew) / (radPanFwdNew - radPanRevNew);
                u = GSmoothStep(u);
            }
            uFrameYWant = pcpd->uFrameY;
            sDistOut = u * sDistFwd + (1.0f - u) * sDistRev;
            uFrameXWant = u * pcpd->uFrameX + (1.0f - u) * -pcpd->uFrameX;
        }
        break;
    }
    if (D_002DDF40.nCut != 0)
    {
        dsDist = duFrameY = 0.0f;
    }
    else
    {
        float c = pcpd->cDist * D_002C426C * D_002DDED0.dt + 1.0f;
        float k = pcpd->kDist * D_002C4270 * D_002DDED0.dt;
        dsDist = c * dsDist + (sDistOut - sDist) * k;
        sDistOut = sDist + dsDist * D_002DDED0.dt;
        duFrameY = c * duFrameY + (uFrameYWant - uFrameY) * k;
        uFrameYWant = uFrameY + duFrameY * D_002DDED0.dt;
    }
    VECTOR4 dposEye;
    *(qword *)&dposEye = VuvecSphere(radPanOut, radTiltOut, sDistOut);
    VECTOR4 posEyeWant;
    *(qword *)&posEyeWant = QwAdd(*(qword *)&posHang, *(qword *)&dposEye);
    if (ptnCur == 0 || ptnCur->fNoCamClip == 0)
    {
        if (D_002DDF40.nCut != 0)
            FreeCamClip(this, *(qword *)&posHang, *(qword *)&posEyeWant, (qword *)&posEyeWant);
        else if (D_002C4118 == 0)
            *(qword *)&posEyeWant = ClipCmEye(&D_002DDF40, posEye, *(qword *)&posEyeWant);
    }
    *(qword *)&dposEye = QwSub(*(qword *)&posEyeWant, *(qword *)&posHang);
    DecomposeSphere(*(qword *)&dposEye, &radPanOut, &radTiltOut, &sDistOut);
    posEye = *(qword *)&posEyeWant;
    *(qword *)&posEyeWant = QwAdd(*(qword *)&posHang, QwMulX(*(qword *)&dposEye, VuFloat((sDistOut + D_002DDF40.sNearClip) / sDistOut)));
    float radLookX = atanf(uFrameXWant * D_002DDF40.gTanHalfX);
    float radLookY = -atanf(uFrameYWant * D_002DDF40.gTanHalfY);
    if (fEntered)
    {
        if (D_002DDF40.nCut != 0)
        {
            radPanLook = radTiltLook = 0.0f;
        }
        else
        {
            float sXY;
            float z;
            DecomposeCylind(*(qword *)&dposEye, 0, &sXY, &z);
            float radPanCm;
            float radTiltCm;
            DecomposeRotateMatrixPanTilt(&D_002DDF40.matRot, &radPanCm, &radTiltCm);
            float radPanView = RadNormalize(radPanOut + 3.1415927f + radLookX);
            float radTiltView = RadNormalize(atan2f(-z, sXY) + radLookY);
            radPanLook = RadNormalize(radPanCm - radPanView);
            radTiltLook = RadNormalize(radTiltCm - radTiltView);
        }
        dradPanLook = dradTiltLook = 0.0f;
    }
    else
    {
        dradPanLook += (radPanLook * D_002C4274 + dradPanLook * D_002C4278) * D_002DDED0.dt;
        dradTiltLook += (radTiltLook * D_002C427C + dradTiltLook * D_002C4280) * D_002DDED0.dt;
        radPanLook += dradPanLook * D_002DDED0.dt;
        radTiltLook += dradTiltLook * D_002DDED0.dt;
    }
    SetCmPanTilt(&D_002DDF40, *(qword *)&posEyeWant, *(qword *)&posHang, radLookX + radPanLook, radLookY + radTiltLook);
    uFrameY = uFrameYWant;
    radPan = radPanOut;
    radTilt = radTiltOut;
    sDist = sDistOut;
    radPanFwd = radPanFwdNew;
    radTiltFwd = radTiltFwdNew;
    radPanRev = radPanRevNew;
    radTiltRev = radTiltRevNew;
    fEntered = 0;
}
