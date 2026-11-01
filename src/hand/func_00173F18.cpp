typedef int qword __attribute__((mode(TI)));
struct M3 { qword row[3]; };
struct M4 { qword row[4]; };
struct VEC { qword q; } __attribute__((aligned(16)));
struct VECTOR4 { float x, y, z, w; } __attribute__((aligned(16)));
struct VU_FLOAT { qword qw; };
struct LM { float gMin, gMax; };
struct LO { char pad0[0x28]; unsigned long flags; char pad30[8]; float sRadius; char pad3C[0x144]; void *pv180; };
struct SEL { int kind; int unk4; LO *plo; char padC[8]; int fBusy; float tUntil; };
struct SET {
    char pad0[0x50]; int csel; SEL *asel; char pad58[0x38];
    int cplo; LO **aplo; int cpos; qword *apos;
};
struct CLOCK { float t, dt; };
struct CAMERA { char pad0[0x60]; qword dir; };
inline void *operator new(unsigned, void *p) { return p; }
struct FRAME { int unk[4]; };

extern "C" {
extern float D_002A53E0;
extern float D_002BE5E4;
extern float D_002BE5E8;
extern CLOCK D_002DDED0;
extern LM D_002BBB60;
extern CAMERA D_002DDF40;
extern qword D_002A2DC0;
FRAME *func_001AE478(FRAME *);
void func_001AE4A0(FRAME *, int);
void func_001B44D8(int *, void ***);
qword func_001A62E8(LO *);
M4 *func_001A6318(LO *);
int func_00173768(SET *, qword, int, void **, float);
float SMaxShape(LO *);
int func_001EA3B0(void *, float);
void EvaluateShapeS(LO *, qword *, qword *, float);
float GRandInRange__Fff(float, float);
float func_002458F0(float, float);
M3 func_0011B500(float, qword);
}

static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    // volatile: the constant is rebuilt at each use (loop.c must not hoist it)
    __asm__ __volatile__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwSubFrom(const qword &qw0, qword qw1)
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
static inline qword QwScaleX(qword qwVec, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline qword QwRotateVecAt(M3 *pmat, qword *pqwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(*pqwVec), "j"(pmat->row[0]), "j"(pmat->row[1]), "j"(pmat->row[2]));
    return qwResult;
}
static inline float SLengthXY(qword qw)
{
    qword qwT;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qwT) : "j"(qw));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwT) : "j"(qwT));
    return GXOfQw(qwT);
}

extern "C" SEL *func_00173F18(SET *pset, qword pos, qword *pposOut, M3 *pmatOut, int *pfFlip)
{
    FRAME frame;
    union { VECTOR4 v; qword q; } posBest;
    M3 matBest;
    int cplo;
    void **aplo;

    float sRadiusMin = D_002A53E0;
    for (int i = 0; i < pset->cplo; i++)
    {
        float s = pset->aplo[i]->sRadius;
        sRadiusMin = sRadiusMin < s ? sRadiusMin : s;
    }
    SEL *pselBest;
    int fFlip = 0;
    func_001AE478(&frame);
    func_001B44D8(&cplo, &aplo);
    pselBest = 0;
    float sBest = D_002A53E0;

    for (int i = 0; i < pset->csel; i++)
    {
        SEL *psel = &pset->asel[i];
        LO *plo;
        if (psel->fBusy)
            goto next;
        if (D_002DDED0.t < psel->tUntil)
            goto next;
        plo = psel->plo;
        if (!plo)
            goto next;
        {
            int fEnabled = (int)(plo->flags >> 60) & 1;
            if (!fEnabled)
                goto next;
        }
        switch (psel->kind)
        {
        case 0:
        case 2:
        {
            float sRadius;
            qword posObj;
            M3 mat;
            switch (psel->kind)
            {
            case 0:
                posObj = func_001A62E8(psel->plo);
                mat = *(M3 *)func_001A6318(psel->plo);
                sRadius = psel->plo->sRadius;
                break;
            case 2:
                sRadius = sRadiusMin;
                posObj = func_001A62E8(psel->plo);
                mat = *(M3 *)func_001A6318(psel->plo);
                break;
            }
            float sDist = SLengthXY(QwSubFrom(pos, posObj));
            if (sDist < D_002BE5E4 || sBest < sDist)
                goto next;
            for (int j = 0; j < pset->cpos; j++)
            {
                if (!func_00173768(pset, QwAdd(posObj, QwRotateVecAt(&mat, &pset->apos[j])), cplo, aplo, sRadius))
                    goto next;
            }
            sBest = sDist;
            pselBest = psel;
            posBest.q = posObj;
            matBest = mat;
            break;
        }
        case 1:
        {
            union { VECTOR4 v; qword q; } posCurve, dirCurve;
            int c = (int)(SMaxShape(plo) / D_002BE5E8);
            for (int k = 0; k < c; )
            {
                int kNext = k + 1;
                float u = D_002BE5E8 * ((float)k + GRandInRange__Fff(D_002BBB60.gMin, D_002BBB60.gMax));
                if (func_001EA3B0(plo->pv180, u))
                    goto nextSample;
                {
                    EvaluateShapeS(plo, &posCurve.q, &dirCurve.q, u);
                    float sDist = SLengthXY(QwSubFrom(pos, posCurve.q));
                    if (sDist < D_002BE5E4 || sBest < sDist)
                        goto nextSample;
                    {
                        int fFlipT = GXOfQw(QwDot(D_002DDF40.dir, dirCurve.q)) < 0.0f;
                        if (!fFlipT)
                        {
                            VU_FLOAT gNeg = VuFloatFromMemWord(0xBF800000);
                            dirCurve.q = QwScaleX(dirCurve.q, gNeg);
                        }
                        M3 matCurve = func_0011B500(func_002458F0(dirCurve.v.y, dirCurve.v.x), D_002A2DC0);
                        int j;
                        for (j = 0; j < pset->cpos; j++)
                        {
                            if (!func_00173768(pset, QwAdd(posCurve.q, QwRotateVecAt(&matCurve, &pset->apos[j])), cplo, aplo, sRadiusMin))
                                break;
                        }
                        if (j >= pset->cpos)
                        {
                            sBest = sDist;
                            fFlip = fFlipT;
                            pselBest = psel;
                            posBest.q = posCurve.q;
                            matBest = matCurve;
                        }
                    }
                }
            nextSample:
                k = kNext;
            }
            break;
        }
        }
    next:;
    }
    *pposOut = posBest.q;
    *pmatOut = matBest;
    *pfFlip = fFlip;
    func_001AE4A0(&frame, 2);
    return pselBest;
}
