/* func_001E9090: one simulation step of a rope-like path chain (wave-4 lane 2, from w3lane9's rewrite).
 *
 * The first point is the start offset transformed by the first space's matrix; its velocity comes from
 * GetXfVelocity. Every later point is integrated (accel = force - damping * v, v += accel * dt,
 * pos += (v_old + v_new) * dt/2), then pulled back to the rest distance |dist[i] - dist[i-1]| from the previous
 * point along the normalized displacement (fallback direction D_002A2DA0 when shorter than 0.0001), and the
 * part of the relative velocity along that direction is removed. With mode 2 the last point is pinned to the
 * end offset in the last space and func_001E95B0 extends the path. Finally every point is transformed into
 * the path's local space (inverse of its world matrix, virtual slot 31) and InvalidateShapeCache refreshes the path.
 *
 * Two register-choice devices (everything else is plain C++ plus one asm per VU operation):
 *  - DotVf1 hard-wires the dot product's 1.0 scratch to $vf1 (clobbered), used for the velocity projection only.
 *    With an allocated "=j" scratch, local-alloc gives the tied difference/result quantity (10 refs over 5
 *    insns, priority 3.0) vf1 before the scratch (priority 2.0); the target has them the other way round. This
 *    stands in for the function's one register pin.
 *  - AddScaled lists its vector input twice (allocation only): the velocity read then outranks the tied
 *    force/acceleration quantity (8 vs 6 refs), so it gets vf1 as in the target.
 */
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct M4 { qword r[4]; };
struct SPACE { char pad[0x40]; M4 matrix; };
struct VT { short delta, pad; M4 *(*fn)(void *); };
struct PATH { char pad[0xC]; VT *vt; char pad10[0xE8 - 0x10]; int count; int padEC; float *dist; U *pos; };
struct CHAIN
{
    qword force;
    qword pad10;
    qword start;            // offset in the first space
    qword end;              // offset in the last space
    SPACE *first;
    SPACE *last;
    PATH *path;
    int mode;               // 2: the last point is pinned to the end offset
    int pad50;
    U *position;
    U *velocity;
    float *distance;        // rest distance along the chain
    float damping;
};
struct CLOCK { float t, dt; };
extern CLOCK D_002DDED0;    // g_clock
extern qword D_002A2DA0;    // fallback direction
extern "C" {
qword GetXfVelocity(SPACE *, SPACE *, qword);
M4 *InvertMatrix4Rigid(M4 *, M4 *);   // invert
void func_001E95B0(CHAIN *, float);
void InvalidateShapeCache(PATH *);
void func_001E9090(CHAIN *);
}

static inline VV TransformPoint(const M4 &m, VV v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]), "j"(v));
    return r;
}
static inline VV Sub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline float GFromVf(VV v) { float g; __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(v)); return g; }
static inline float GFromQw(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
static inline VV Sqrt(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
/* Dot with the 1.0 scratch in $vf1 (see the header comment) */
static inline VV DotVf1(VV a, VV b)
{
    qword r;
    __asm__("vmul.xyz %0, %1, %2\n\tvaddw.x $vf1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, $vf1, %0z"
            : "=j"(r) : "j"(a), "j"(b) : "vf1");
    return r;
}
static inline VV Length(VV v) { return Sqrt(Dot(v, v)); }
static inline VV DivideQ(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
/* base + v * g */
static inline VV AddScaled(VV base, VU_FLOAT g, VV v)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x"
            : "=j"(r) : "j"(base), "j"(g.q), "j"(v), "j"(v));   /* second "j"(v): allocation only */
    return r;
}
static inline VV NormalizeSafe(VV v, VV fallback)
{
    VV len = Length(v);
    if (GFromVf(len) < 0.0001f)
        return fallback;
    return DivideQ(v, len);
}
/* base + v1 * g1 + v2 * g2 */
static inline VV VecCombo3(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2));
    return r;
}
static inline float Abs(float g) { return __builtin_fabsf(g); }

/** @sly2-annot addr=0x1E9090
 * Purpose: One integration step of the rope chain (Verlet-like with distance constraints, pinned ends)
 * Info: proposed name StepChain (T2); TU shape
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E9090(CHAIN *p)
{
    float dt = D_002DDED0.dt;
    VV prev = TransformPoint(p->first->matrix, p->start);
    int n = p->path->count;
    p->position[0].q = prev;
    U *velocity = p->velocity;
    VV prevVel = GetXfVelocity(p->first, 0, prev);
    velocity[0].q = prevVel;
    int i;
    for (i = 1; i < n; i++)
    {
        VV accel = AddScaled(p->force, VU_FLOAT(-p->damping), p->velocity[i].q);
        VV vel = AddScaled(p->velocity[i].q, VU_FLOAT(dt), accel);
        VV pos = VecCombo3(p->position[i].q, VU_FLOAT(dt * 0.5f), p->velocity[i].q, VU_FLOAT(dt * 0.5f), vel);
        VV delta = Sub(pos, prev);
        VV dir = NormalizeSafe(delta, D_002A2DA0);
        float proj = -GFromQw(DotVf1(Sub(vel, prevVel), dir));
        float d = Abs(p->distance[i] - p->distance[i - 1]);
        prev = AddScaled(prev, VU_FLOAT(d), dir);
        prevVel = AddScaled(vel, VU_FLOAT(proj), dir);
        p->position[i].q = prev;
        p->velocity[i].q = prevVel;
    }

    if (p->mode == 2)
    {
        p->position[n - 1].q = TransformPoint(p->last->matrix, p->end);
        func_001E95B0(p, p->distance[n - 2] + GFromVf(Length(Sub(p->position[n - 2].q, p->position[n - 1].q))));
        n = p->path->count;
    }
    M4 inverse;
    {
        PATH *path = p->path;
        VT *e = &path->vt[31];
        InvertMatrix4Rigid(&inverse, e->fn((char *)path + e->delta));
    }
    for (i = 0; i < n; i++)
    {
        PATH *path = p->path;
        path->pos[i].q = TransformPoint(inverse, p->position[i].q);
    }
    InvalidateShapeCache(p->path);
}
