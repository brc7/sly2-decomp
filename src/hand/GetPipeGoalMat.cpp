// GetPipeGoalMat (lane 6, 2026-10-24): builds an orientation basis (M3) for a position `distance` along a path.
// It samples the path tangent at the start, then tries sample distances distance, +10, -10, +20, -20 ... (wrapped
// for cyclic paths, rejected outside [low,high] otherwise) until the direction between the samples at +-50 is within
// ~25 degrees of the tangent, rotates the path axis by the tangent->direction rotation, and builds the basis
// (side, up) from that.  Optionally flips side/up (forward == 0) and copies the basis to *out.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct PATH { char pad0[0xE4]; int cyclic; char padE8[0xD8]; qword axis; };
extern "C" {
extern qword D_002A2DA0, D_002A2DC0;   // fallback unit vectors
float SMinShape(PATH *);           // low distance bound
float SMaxShape(PATH *);           // high distance bound
float UMinShape(PATH *);           // start distance
void EvaluateShapeU(PATH *, float, qword *, qword *);
float GModPositive__Fff(float, float);     // wrap into range
float SClampShape(PATH *, float);
void EvaluateShapeS(PATH *, float, qword *, qword *);
M3 BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(qword, qword);
qword ConvertAloVec(PATH *, PATH *, qword);
qword GetNormalVector(qword);
}

union U { qword q; float f[4]; };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword operator*(VU_FLOAT f, VU_VECTOR v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q)); return r; }

static inline float X(qword q) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q)); return f; }
static inline qword Length(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(d));
    return len;
}
static inline qword Unit(qword v, qword len)
{
    qword r;
    __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(v), "j"(len));
    return r;
}
static inline qword Normalize(qword v, qword fallback)
{
    qword len = Length(v);
    if (X(len) < 0.0001f)
        return fallback;
    return Unit(v, len);
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    return f;
}
// earlyclobber output: the result must not share a register with the matrix rows
static inline qword Transform(const M3 &m, qword q)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=&j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(q));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

struct WORK { M3 basis; qword tangent, first, direction; };

extern "C" void GetPipeGoalMat(PATH *p, float distance, int forward, M3 *out)
{
    WORK w;
    U fwd;   // declared outside the inner block on purpose: the stack slot of `back` (declared inside) is freed at the
             // end of that block and is reused by the VU_FLOAT temporary of the final negate, like the original.
    float low = SMinShape(p), high = SMaxShape(p);
    float t0 = UMinShape(p);
    EvaluateShapeU(p, t0, 0, &w.tangent);
    w.tangent = Normalize(w.tangent, D_002A2DA0);
    {
        U back;
        M3 rot;
        for (int i = 0;; ++i) {
            float offset = (i & 1) ? 10.0f * (float)(i + 1) : (float)i * -10.0f;
            float sample = distance + offset;
            if (!p->cyclic) {
                if (sample < low || high < distance)
                    continue;
            } else
                sample = low + GModPositive__Fff(sample - low, high - low);
            float r = 50.0f;
            EvaluateShapeS(p, SClampShape(p, sample + r), &fwd.q, 0);
            EvaluateShapeS(p, SClampShape(p, sample - r), &back.q, 0);
            w.direction = Normalize(Sub(fwd.q, back.q), D_002A2DC0);
            if (i == 0)
                w.first = w.direction;
            if (Dot(w.tangent, w.direction) > -0.9f)
                break;
        }
        rot = BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(w.tangent, w.direction);
        qword ref = ConvertAloVec(p, 0, p->axis);
        fwd.q = Transform(rot, ref);
        qword up = Normalize(w.first, D_002A2DC0);
        w.basis.r[2] = up;
        back.q = Cross(up, fwd.q);
        w.basis.r[1] = back.q;
        qword len = Length(back.q);
        w.basis.r[1] = Unit(back.q, len);
        if (X(len) < 0.0001f)
            w.basis.r[1] = GetNormalVector(up);
        w.basis.r[0] = Cross(w.basis.r[1], w.basis.r[2]);
    }
    if (!forward) {
        w.basis.r[1] = VU_FLOAT(-1.0f) * VU_VECTOR(w.basis.r[1]);
        w.basis.r[2] = VU_FLOAT(-1.0f) * VU_VECTOR(w.basis.r[2]);
    }
    if (out)
        *out = w.basis;
}
