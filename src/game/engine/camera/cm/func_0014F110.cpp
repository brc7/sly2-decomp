// func_0014F110 (Claude agent, lane K5, 2026-10-24): MATCH. Built on laneK4v1 (K3/K4 fresh rewrite in the func_0014C748 style).
// Keys: the VU_FLOAT(const float *) ctor's word input has a never-chosen "V" alternative ("r,V"), so it stays a MEM until
// reload: the clock dt load is then an input reload (lw v0 for the vdiv; for the final AddScaled reload reuses the dying
// pclock register, lw s0,0x10(s0)), which also shortens the clock chain in sched1 so no extra operand is needed on the vdiv;
// v is computed before posMin so the spill slots come out v 0x30 / posMin 0x40. No register pins.
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" (non-offsettable mem) alternative: allocation-only (the word stays a MEM until reload) */ };
struct CLOCK { float t, dt, x8, xC, dtPrev; };
struct FRAME { int n; FRAME(); ~FRAME(); }; /* out-of-line ctor/dtor: STACKSCOPE_ctor / STACKSCOPE_ctor_1AE4A0(this, 2) */
extern "C" {
extern CLOCK D_002DDED0;
extern qword D_002BD8F0, D_002BD900;
extern float D_002BD8D8, D_002BD8DC;
extern char D_003DD490[];
void *STACKSCOPE_ctor(FRAME *p);
void STACKSCOPE_ctor_1AE4A0(FRAME *p, int n);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
qword SquishCmEye(void *p, int i, int j, void *pfn, qword a, qword v, float g, float h);
void FFilterCamera(void);
void SvSquishCmEyeConstraint(void);
qword func_0014F110(void *p, qword pos, qword posTarget);
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword DivideBy(const qword &v, const VU_FLOAT &g) { qword r; __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline qword AddScaled(qword base, const qword &vec, const VU_FLOAT &s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(base), "j"(vec), "j"(s.q));
    return r;
}
static inline qword QwDot(const qword &a, const qword &b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword QwSqrt(const qword &a) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(a)); return r; }
static inline float GXOfQw(const qword &q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static inline float Length(const qword &v) { return GXOfQw(QwSqrt(QwDot(v, v))); }

/** @sly2-annot addr=0x14F110
 * Purpose: vEye=(next-prev)/dtPrev; gather camera-colliding SOs in a box around prev; SquishCmEye; return
 *   prev + vClip*dt
 * Info: proposed name ClipCmEye (T1); TU cm
 * Notes: names_v2 v1 proposed SetupCmRotateToCam (0.005)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword func_0014F110(void *p, qword pos, qword posTarget)
{
    CLOCK *pclock = &D_002DDED0;
    int ii, jj;
    qword dpos = Sub(posTarget, pos);
    qword v = DivideBy(dpos, VU_FLOAT(&pclock->dtPrev));
    qword posMin = Add(pos, D_002BD8F0);
    qword posMax = Add(pos, D_002BD900);
    FRAME frame;
    IntersectSwBoundingBox(D_003DD490, posMin, posMax, (void *)FFilterCamera, 0, &ii, &jj);
    qword w = SquishCmEye(p, ii, jj, (void *)SvSquishCmEyeConstraint, pos, v, D_002BD8D8, Length(v) + D_002BD8DC);
    qword r = AddScaled(pos, w, VU_FLOAT(&pclock->dtPrev));
    return r;
}
