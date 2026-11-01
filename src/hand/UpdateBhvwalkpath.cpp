// UpdateBhvwalkpath (Claude agent, lane G13, 2026-10-24): laneG10v2 (no pins) + VU_FLOAT(float) qmtc2 constructor building the speed in place, and the 1e-4 length test read with GXOfQw (gcc emits the qmfc2, asm mtc1).
typedef int qword __attribute__((mode(TI)));
struct MATRIX3 { float mat[3][4]; };
struct CLQ { float g0, g1, g2; };
struct LM { float gMin, gMax; };
struct ACTOR { char pad[0xFD0]; char control[1]; };
struct MODEL { char pad[0x10]; qword velocity; };
struct CONTROL { char pad[0x24]; void *animation; };
struct CHOICE { int kind; MODEL *model; CONTROL *control; };
struct DRIVER { char pad[0x40]; CHOICE *choice; };
struct P {
    char pad[0x10]; ACTOR *actor; int pad14; DRIVER *driver;
    char pad1C[0x4C - 0x1C]; unsigned int mode; char pad50[0x80 - 0x50]; qword target;
    char pad90[0xC4 - 0x90]; float rate; char padC8[8]; qword cached; float timeout;
};
struct CLOCK { float t, dt; };
struct VU_FLOAT { qword qw; VU_FLOAT() {} VU_FLOAT(float g) { __asm__("qmtc2 %1, %0" : "=j"(qw) : "r"(g)); } };
extern "C" {
extern CLQ D_002BE6A0;
extern LM D_002BBB60;
extern float D_002BE694, D_002BE6B0;
extern CLOCK D_002DDED0;
extern qword D_002A2DC0, D_002A2DD0;
void UpdateBhvpath(P *);
qword func_001A62E8(ACTOR *);
MATRIX3 *func_001A6318(ACTOR *);
void func_00131A00(void *, float);
float func_002458F0(float, float);
void SetStepguardMoverRadTarget(void *, float);
void func_0020CB28(CONTROL *, qword);
}
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline float GXOfQw(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
static inline float GYOfQw(qword qw)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(qw));
    return y;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
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
static inline qword QwProjectOut(qword v, qword n)
{
    qword d = QwDot(v, n);
    qword al, ac;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(al) : "j"(d), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(ac) : "j"(v), "j"(al));
    return ac;
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
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
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
static inline qword QwLengthDot(qword v)
{
    qword qw = QwDot(v, v);
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLength(qword v) { return GFromVf(QwLength(v)); }
static inline float GLengthXY(qword v) { return GFromVf(QwLengthXY(v)); }
static inline qword QwScaleX(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline qword QwDotRowsMatP(MATRIX3 *pmat, qword qwV)
{
    qword qwLocal;
    qword qwTmp0, qwTmp1, qwTmp2, qwOnes;
    __asm__("vmul.xyzw %2, %5, %8\n\tvmaxw.xyzw %1, $vf0, $vf0w\n\tvmul.xyzw %3, %6, %8\n\tvmul.xyzw %4, %7, %8\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %0, %1, %2z\n\tvaddax.y $ACC, %3, %3x\n\tvmaddz.y %0, %1, %3z\n\tvaddax.z $ACC, %4, %4x\n\tvmaddy.z %0, %1, %4y"
            : "=j"(qwLocal), "=&j"(qwOnes), "=&j"(qwTmp0), "=&j"(qwTmp1), "=&j"(qwTmp2)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(qwV));
    return qwLocal;
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
static inline int FMoved(qword qwPos, qword qwPosLast)
{
    return D_002BE6B0 < GLength(QwSub(qwPos, qwPosLast));
}

extern "C" void UpdateBhvwalkpath(P *p)
{
    UpdateBhvpath(p);
    if (p->mode < 2)
    {
        qword pos = func_001A62E8(p->actor);
        qword dpos = QwProjectOut(QwSub(p->target, pos), D_002A2DC0);
        float g = GEvaluateClqLm(&D_002BE6A0, &D_002BBB60, GXOfQw(QwLengthXY(dpos)));
        CHOICE *pchoice = p->driver->choice;
        switch (pchoice->kind)
        {
        case 0:
            func_00131A00(pchoice->control, p->rate * g);
            SetStepguardMoverRadTarget(p->actor->control, func_002458F0(GYOfQw(dpos), GXOfQw(dpos)));
            break;
        case 3:
        {
            qword qwLocal = QwDotRowsMatP(func_001A6318(p->actor), dpos);
            qword qwFallback = D_002A2DD0;
            float gSpeed = p->rate * g;
            gSpeed *= GXOfQw(QwLengthXY(pchoice->model->velocity));
            qword qwLen = QwLengthUntied(qwLocal);
            VU_FLOAT vuSpeed(gSpeed);
            qword qwDir;
            if (GXOfQw(qwLen) < 0.0001f)
            {
                qwDir = qwFallback;
            }
            else
            {
                __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLen));
                __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwDir) : "j"(qwLocal));
            }
            func_0020CB28(pchoice->control, QwScaleX(qwDir, vuSpeed));
            func_00131A00(pchoice->control->animation, g);
            break;
        }
        }
        if (FMoved(pos, p->cached))
        {
            p->cached = pos;
            p->timeout = D_002BE694;
        }
        else
        {
            p->timeout -= D_002DDED0.dt;
        }
    }
}
