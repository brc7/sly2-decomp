// func_002236E8 = UpdateCmLineOfSight__FP7CMPART2 (Sly2Port KB name): the follow camera's line-of-sight test.
// Every D_002C4110 (0.5) seconds it looks at five points around the hero (PosLoWorld + back * x + side * y + up * z
// for the five offsets in D_002C4290; side = normalize(up x (hero - eye)) or (1, 0, 0) when the hero is straight
// above). Each point is projected onto the screen, moved to the near clip depth and the segment between the two is
// tested against the camera hulls (FFindCmClearInterval). The first visible point resets tLastSeen; when the hero
// has been hidden for more than D_002C4114 (2.0) seconds the camera cuts (CutCm).
// MATCH notes (Lilly's agent, T1578): STACKSCOPE needs its real out-of-line ctor/dtor (an inline wrapper calling
// func_001AE478 makes gcc keep hi(g_clock) in $fp). Loop 1 keeps a pointer to the current sample and loop 2 starts
// from `pqw -= 4`: loop.c emits the pointer's final value after loop 1 and cse2 folds the rewind to sp+0x30, which
// is why the game sets the loop-2 pointer in the STACKSCOPE constructor's delay slot. Any `&aqw[3]` written after
// loop 1 is hoisted above loop 1 by gcse's PRE instead.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
struct LO;
struct SO;
struct SOPART;
struct SW;
struct TN { char pad0[0x18C]; int fNoCamClip; };
struct CMPART2 { LO *pvHero; char pad4[0xC]; TN *ptn; char pad14[0x6C]; float tLastSeen; };
struct VECTOR4 { float x, y, z, w; };
struct CM { char pad0[0x4C]; float sNearClip; char pad50[0x40]; qword pos; char padA0[0x260]; int nCut; };
struct CLOCK { float t; float dt; };
extern CM D_002DDF40;
extern CLOCK D_002DDED0;
extern SW D_003DD490;
extern int D_002DFBC4;
extern int D_002C4118;
extern float D_002C4110;
extern float D_002C4114;
extern qword D_002A2DC0;
extern qword D_002A2DA0;
extern qword D_002C4290[5];
extern "C" {
qword func_001A62E8(LO *plo);
int func_0014F030(void *pv, SO *pso);
void IntersectSwBoundingBox(SW *psw, qword posMin, qword posMax, int (*pfnFilter)(void *, SO *), int nUnk, int *pcpso, SOPART ***papso);
qword func_0014F308(CM *pcm, qword pos);
qword func_0014F268(CM *pcm, qword ndc);
int func_0014EA08(int cpsopart, SOPART **apsopart, qword posFrom, qword posTo, float duMin, float *puEnter, float *puExit);
void func_0014D8C0(CM *pcm);
}
struct STACKSCOPE {
    STACKSCOPE();  // 0x1AE478
    ~STACKSCOPE(); // 0x1AE4A0
};
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
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qw));
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
static inline qword QwCross(qword a, qword b)
{
    qword qw;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}
// v * (1 / |v|), with |v| in x of qwLen
static inline qword QwScaleByInverse(qword v, qword qwLen)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLen));
    __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qw) : "j"(v));
    return qw;
}
static inline qword QwMiniXyz(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwMaxXyz(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
// back * v.x + side * v.y + up * v.z
static inline qword QwAxesXyz(qword qwV, qword qwBack, qword qwSide, qword qwUp)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z" : "=j"(qw) : "j"(qwV), "j"(qwBack), "j"(qwSide), "j"(qwUp));
    return qw;
}

/** @sly2-annot addr=0x2236E8
 * Purpose: every 0.5 s tests 5 points around hero for visibility; cut after 2 s hidden
 * Info: proposed name UpdateCmLineOfSight (T1); TU tn
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
extern "C" void func_002236E8(CMPART2 *pcmpart)
{
    LO *plo = pcmpart->pvHero;
    if (D_002DFBC4 & 2)
    {
        pcmpart->tLastSeen = D_002DDED0.t;
        return;
    }
    if (pcmpart->ptn != 0 && pcmpart->ptn->fNoCamClip != 0)
        return;
    if (D_002C4118 != 0)
        return;
    if (D_002DDED0.t - pcmpart->tLastSeen < D_002C4110)
        return;
    if (D_002DDF40.nCut != 0)
        return;
    qword aqw[11]; // box min 0 / max 1, near-plane point 2, samples 3..7, back 8, side 9, up 10
    uint i;
    aqw[0] = aqw[1] = D_002DDF40.pos;
    qword qwUp = D_002A2DC0;
    aqw[10] = qwUp;
    qword qwToHero = QwSub(func_001A62E8(plo), D_002DDF40.pos);
    qword qwCross = QwCross(qwUp, qwToHero);
    qword qwFallback = D_002A2DA0;
    qword qwLen = QwLengthUntied(qwCross);
    qword qwSide;
    if (GFromVf(qwLen) < 0.0001f)
    {
        qwSide = qwFallback;
    }
    else
    {
        qwSide = QwScaleByInverse(qwCross, qwLen);
    }
    aqw[9] = qwSide;
    aqw[8] = QwCross(aqw[10], qwSide);
    qword *pqw; // the current sample
    for (i = 0; i < 5; i++)
    {
        qword pos = QwAdd(func_001A62E8(plo), QwAxesXyz(D_002C4290[i], aqw[8], aqw[9], aqw[10]));
        aqw[0] = QwMiniXyz(aqw[0], pos);
        aqw[1] = QwMaxXyz(aqw[0], pos);
        pqw = &aqw[3 + i];
        *pqw = pos;
    }
    pqw -= 4; // back to the first sample
    STACKSCOPE scope;
    int cpso;
    SOPART **apso;
    IntersectSwBoundingBox(&D_003DD490, aqw[0], aqw[1], func_0014F030, 0, &cpso, &apso);
    for (i = 0; i < 5; i++, pqw++)
    {
        aqw[2] = func_0014F308(&D_002DDF40, *pqw);
        ((VECTOR4 *)&aqw[2])->z = D_002DDF40.sNearClip;
        aqw[2] = func_0014F268(&D_002DDF40, aqw[2]);
        float uExit;
        if (func_0014EA08(cpso, apso, *pqw, aqw[2], 0.0f, 0, &uExit) && 1.0f <= uExit)
        {
            pcmpart->tLastSeen = D_002DDED0.t;
            break;
        }
    }
    if (D_002DDED0.t - pcmpart->tLastSeen > D_002C4114)
        func_0014D8C0(&D_002DDF40);
}
