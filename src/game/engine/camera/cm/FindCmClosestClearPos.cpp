typedef int qword __attribute__((mode(TI)));
extern "C" {
extern int D_002BD8B0[];
qword SetVectorSphere(float pan, float tilt, float s);
int FFindCmClearInterval(void *p, void *x, qword a, qword b, float *pt, float *pu, float g);
qword FindCmClosestClearPos(void *p, void *x, qword pos, float g);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}

static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2, VU_FLOAT f3, VU_VECTOR v3)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmaddax.xyzw $ACC, %2, %5x\n\tvmaddx.xyzw %0, %3, %6x"
            : "=j"(r) : "j"(v1.q), "j"(v2.q), "j"(v3.q), "j"(f1.q), "j"(f2.q), "j"(f3.q));
    return r;
}

/** @sly2-annot addr=0x14EB70
 * Purpose: Samples 9 tilts x D_002BD8B0[i] pans at radius g, padding 5/g, returns closest clear position
 * Info: proposed name FindCmClosestClearPos (T1); TU cm
 * Notes: names_v2 v1 proposed UpdateCmFade (0.200)
 * Confidence: high [T1 0.90] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword FindCmClosestClearPos(void *p, void *x, qword pos, float g)
{
    float t, u;
    qword best;
    float dg = 5.0f / g;
    float gBest = 3.40282346638528859812e+38f;
    int i, j;

    best = pos;
    for (i = 0; i < 9; i++) {
        float tilt = (float)(i - 4) * 0.3926991f;
        int c = D_002BD8B0[i];
        for (j = 0; j < c; j++) {
            qword dir = SetVectorSphere((float)j * 3.1415927f / (float)c, tilt, 1.0f);
            qword test = AddScaled(pos, dir, VU_FLOAT(g));
            if (FFindCmClearInterval(p, x, pos, test, &t, &u, dg + dg)) {
                if (0.0f < t)
                    t += dg;
                if (t < gBest) {
                    gBest = t;
                    best = VecCombo(VU_FLOAT(1.0f - gBest), pos, VU_FLOAT(gBest), test, VU_FLOAT(-1.0f), dir);
                }
            }
        }
    }
    return best;
}
