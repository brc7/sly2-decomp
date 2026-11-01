/* func_001E8378 (Lilly's range): one RK4-style step of a spring rope (CHAIN, same struct as matched func_001E9090).
 * Matching tricks (T1869, see notes/190000-1F0000_lanes.md):
 *  - the 4 empty volatile asms at the end of the pass loop add 4 RTL insns (no code) so the hoisted start/startVel/end/endVel
 *    copies get vf12..14 in the game's order (global-alloc live length);
 *  - NormalizeV takes the previous length as a dummy "f" input and &fallback as a dummy "r" input: sched1 then issues
 *    mtc1, addiu, qmtc2 in the game's order;
 *  - proj2 is computed into a local first and stored into the U after the shared `damping` VU_FLOAT, so its home store has the
 *    higher LUID (sched1 tie-break) and issues after the damping store.
 * Repeated "j" inputs are allocation-only (local-alloc priority). */
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
    VU_FLOAT(const qword &v) { q = v; }
    VU_FLOAT(int w) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(w)); }
    VU_FLOAT(const float *pf) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(*(const int *)pf)); }  /* a float word from memory */
};
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
struct M4 { qword r[4]; };
struct SPACE { char pad[0x40]; M4 matrix; };
struct VT { short delta, pad; M4 *(*fn)(void *); };
struct PATH { char pad[0xC]; VT *vt; char pad10[0xE8 - 0x10]; int count; int padEC; float *dist; U *pos; };
struct LSG { qword apos[2]; qword anormal[2]; float au[2]; int lsgk; void *owner[2]; };
struct CHAIN
{
    qword force;
    qword pad10;
    qword start;
    qword end;
    SPACE *first;
    SPACE *last;
    PATH *path;
    int mode;
    int pad50;
    U *position;
    U *velocity;
    float *distance;
    float damping;
    float stretch, stretchDamping;
    float bend, bendDamping;
    float shape, shapeDamping;
};
struct CLOCK { float t, dt; };
extern CLOCK D_002DDED0;
extern float D_002C3658[4];
extern float D_002C3668[4];
extern qword D_002A2DA0;
extern qword D_002A2DC0;
extern "C" {
qword GetXfVelocity(SPACE *, SPACE *, qword);
void func_001AE5A0(void *, const void *, unsigned);
int func_0014C748(qword posCenter, float sRadius, qword pos0, qword pos1, int clsgMax, LSG *alsg);
M4 *InvertMatrix4Rigid(M4 *, M4 *);
void InvalidateShapeCache(PATH *);
void func_001E8378(CHAIN *);
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
/* float out of a VF register: asm mtc1, the compiler emits the qmfc2 (reload); the one-cost asm decides the sched2 ties */
static inline float GFromQw(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
static inline VV Sqrt(VV v) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(v)); return r; }
static inline VV Dot(VV a, VV b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(r), "=j"(one) : "j"(a), "j"(b));
    return r;
}
static inline VV Length(VV v) { return Sqrt(Dot(v, v)); }
static inline VV DivideQ(VV v, VV l) { qword r; __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v)); return r; }
static inline VV Scale(VV v, VV g) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(g)); return r; }
/* base + v * g */
static inline VV AddScaled(VV base, VU_FLOAT g, VV v)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v));
    return r;
}
/* AddScaled2 for the integration step: the extra base/g inputs are allocation only (refs -> local-alloc priority:
   k/2 before the velocity read, accP before it too -> k/2 vf2, accP vf3, velocity vf4 as in the game) */
static inline VV AddScaled2I(VV base, VU_FLOAT g, VV v1, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %4, %2x" : "=j"(r) : "j"(base), "j"(g.q), "j"(v1), "j"(v2), "j"(base), "j"(base), "j"(g.q), "j"(g.q), "j"(g.q));
    return r;
}
/* VecCombo3 for the two stretch springs; the repeated v2 / v1 input is allocation only (raises proj1 / dir2 in local-alloc's
   priority order so the stretch block gets the game's vf registers: proj1 vf4, dir2 vf3, g1 vf5) */
static inline VV VecCombo3S1(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2), "j"(v2));
    return r;
}
static inline VV VecCombo3S2(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2), "j"(v1));
    return r;
}
/* Sub with b repeated (allocation only): the newV[i] reload outranks the proj2 chain, so it gets vf1 and proj2 vf2 */
static inline VV SubX(VV a, VV b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b), "j"(b)); return r; }
/* base + v1 * g1 + v2 * g2 */
static inline VV VecCombo3(VV base, VU_FLOAT g1, VV v1, VU_FLOAT g2, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %5, %4x"
            : "=j"(r) : "j"(base), "j"(g1.q), "j"(v1), "j"(g2.q), "j"(v2));
    return r;
}
/* base + (v1 + v2) * g */
static inline VV AddScaled2(VV base, VU_FLOAT g, VV v1, VV v2)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %3, %2x\n\tvmaddx.xyzw %0, %4, %2x"
            : "=j"(r) : "j"(base), "j"(g.q), "j"(v1), "j"(v2));
    return r;
}
/* float -> VF without a VU_FLOAT object: no asm-output temp, so no home store (the game has none for the 2nd spring's g) */
static inline qword VfFromF(float f) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(f)); return r; }
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
/* normalize *pv in place (fallback when shorter than 0.0001); returns the original length */
static inline float Normalize(U *pv, const qword &fallback)
{
    VV len = Length(pv->q);
    if (GFromVf(len) < 0.0001f)
        pv->q = fallback;
    else
        pv->q = DivideQ(pv->q, len);
    return GFromQw(len);
}
/* Normalize for a vector re-read from memory after a join (dir2): the game loads it into a GPR (lq) and reload copies it into
   the dot's VF input (qmtc2, no RTL insn of its own); the dot hands the VF copy on to the divide as a tied output.
   Allocation only: the two "r"(v) operands keep v in a GPR (the target's lq $v0 + qmtc2 $v0,$vf3). */
static inline float NormalizeV(U *pv, const qword &fallback, float prev)
{
    qword v = pv->q;
    qword d, one, vf;
    __asm__("vmul.xyz %1, %3, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
            : "=j"(one), "=j"(d), "=j"(vf) : "2"(v), "r"(v), "r"(v), "r"(&fallback), "f"(prev));   /* "r"(v) x2: allocation only */
    VV len = Sqrt(d);
    if (GFromVf(len) < 0.0001f)
        pv->q = fallback;
    else
        pv->q = DivideQ(vf, len);
    return GFromQw(len);
}
static inline float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
static inline int FWithin(VV a, VV b, float s) { return GFromQw(Length(Sub(a, b))) < s; }

static inline void Store(U *pd, VV v) { pd->q = v; }
static inline const qword &Q(const U *p) { return p->q; }
static inline VV SubP(const U *a, const U *b) { return Sub(a->q, b->q); }
static inline void Blend(U *pd, const U *pa, const qword &b, float t) { pd->q = VecCombo(VU_FLOAT(1.0f - t), VU_VECTOR(pa->q), VU_FLOAT(t), VU_VECTOR(b)); }
static inline void LerpP(U *pd, const U *pa, const U *pb, float t) { pd->q = VecCombo(VU_FLOAT(1.0f - t), VU_VECTOR(pa->q), VU_FLOAT(t), VU_VECTOR(pb->q)); }

/** @sly2-annot addr=0x1E8378
 * Purpose: One RK4-style step of the spring rope chain
 * Info: proposed name StepChainRk4 (T2); TU shape
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E8378(CHAIN *p)
{
    U newP[64], newV[64], accP[64], accV[64], force[64];
    int i;
    int count = p->path->count;
    VV start = TransformPoint(p->first->matrix, p->start);
    VV startVel = GetXfVelocity(p->first, 0, start);
    VV end = TransformPoint(p->last->matrix, p->end);
    VV endVel = GetXfVelocity(p->last, 0, end);
    if ((unsigned)count > 64)
        return;
    func_001AE5A0(accP, p->position, count * 16);
    func_001AE5A0(accV, p->velocity, count * 16);
    for (int pass = 0; pass < 4; pass++) {
        float w = D_002C3658[pass];
        float h = w * D_002DDED0.dt;
        Blend(&newP[0], &p->position[0], start, w);
        Blend(&newV[0], &p->velocity[0], startVel, w);
        Blend(&newP[count - 1], &p->position[count - 1], end, w);
        Blend(&newV[count - 1], &p->velocity[count - 1], endVel, w);
        for (i = 1; i < count - 1; i++) {
            Store(&newV[i], AddScaled(p->velocity[i].q, VU_FLOAT(h), force[i].q));
            Store(&newP[i], AddScaled2(p->position[i].q, VU_FLOAT(h * 0.5f), p->velocity[i].q, newV[i].q));
        }
        float scale = 0.75f - 0.25f / (count - 2);
        float rest = p->distance[count - 1];
        VV chord = SubP(&newP[0], &newP[count - 1]);
        float amplitude = scale * SqrtF(GMax(rest * rest - GFromQw(Dot(chord, chord)), 0.0f));
        for (i = 1; i < count - 1; i++) {
            VV F = AddScaled(p->force, VU_FLOAT(-p->damping), newV[i].q);
            if (p->stretch != 0.0f || p->stretchDamping != 0.0f) {
                U dir1, dir2, proj1, proj2;
                dir1.q = SubP(&newP[i - 1], &newP[i]);
                dir2.q = SubP(&newP[i + 1], &newP[i]);
                float len1 = Normalize(&dir1, D_002A2DA0);
                float len2 = NormalizeV(&dir2, D_002A2DA0, len1);
                float rest1 = p->distance[i] - p->distance[i - 1];
                float rest2 = p->distance[i + 1] - p->distance[i];
                proj1.q = Scale(dir1.q, Dot(SubP(&newV[i - 1], &newV[i]), dir1.q));
                VV pr2 = Scale(dir2.q, Dot(SubX(newV[i + 1].q, newV[i].q), dir2.q));
                VU_FLOAT g1(p->stretch * (len1 - rest1));
                /* damping built per spring: cse merges the two "r" qmtc2s and reload_cse drops the 2nd home store (same vf, same slot);
                   that dropped store still lengthens the g1 store chain in sched1, so g1 is born before the dir2 reload like the game */
                VU_FLOAT damping(&p->stretchDamping);
                proj2.q = pr2;
                F = VecCombo3S1(F, g1, dir1.q, damping, proj1.q);
                F = VecCombo3S2(F, VU_FLOAT(VfFromF(p->stretch * (len2 - rest2))), dir2.q, damping, proj2.q);
            }
            if (p->bend != 0.0f || p->bendDamping != 0.0f) {
                U mid, midV, dir, proj;
                float r = (p->distance[i] - p->distance[i - 1]) / (p->distance[i + 1] - p->distance[i - 1]);
                LerpP(&mid, &newP[i - 1], &newP[i + 1], r);
                LerpP(&midV, &newV[i - 1], &newV[i + 1], r);
                dir.q = SubP(&mid, &newP[i]);
                float len = Normalize(&dir, D_002A2DC0);
                proj.q = Scale(dir.q, Dot(SubP(&midV, &newV[i]), dir.q));
                F = VecCombo3(F, VU_FLOAT(p->bend * len), dir.q, VU_FLOAT(&p->bendDamping), proj.q);
            }
            if (p->shape != 0.0f || p->shapeDamping != 0.0f) {
                U mid, midV, dir, proj;
                float r = p->distance[i] / p->distance[count - 1];
                LerpP(&mid, &newP[0], &newP[count - 1], r);
                LerpP(&midV, &newV[0], &newV[count - 1], r);
                dir.q = SubP(&mid, &newP[i]);
                float len = Normalize(&dir, D_002A2DC0);
                proj.q = Scale(dir.q, Dot(SubP(&midV, &newV[i]), dir.q));
                float r4 = r * 4.0f;
                F = VecCombo3(F, VU_FLOAT(p->shape * (len - amplitude * r4 * (1.0f - r))), dir.q, VU_FLOAT(&p->shapeDamping), proj.q);
            }
            float wk = D_002C3668[pass];
            float k = wk * D_002DDED0.dt;
            Store(&accV[i], AddScaled(accV[i].q, VU_FLOAT(k), F));
            Store(&accP[i], AddScaled2I(accP[i].q, VU_FLOAT(k * 0.5f), p->velocity[i].q, newV[i].q));
            Store(&force[i], F);
        }
        /* 4 empty asms: +4 RTL insns in the pass loop (live-length probe, emits nothing) */
        __asm__ __volatile__("");
        __asm__ __volatile__("");
        __asm__ __volatile__("");
        __asm__ __volatile__("");
    }
    if (p->mode == 3) {
        LSG lsg;
        int j0 = 1;
        p->position[0].q = start;
        p->velocity[0].q = startVel;
        for (i = 1; i < count - 1; i++) {
            float step = p->distance[i] - p->distance[i - 1];
            if (FWithin(end, p->position[i - 1].q, step))
                break;
            for (int j = j0; j < count; j++) {
                float t;
                if (!func_0014C748(p->position[i - 1].q, step, Q(&accP[j - 1]), Q(&accP[j]), 1, &lsg)) {
                    p->position[i] = accP[j - 1];
                    t = 0.0f;
                } else if (lsg.au[1] < 1.0f) {
                    t = lsg.au[1];
                    p->position[i].q = lsg.apos[1];
                } else
                    continue;
                p->path->dist[i] = step;
                j0 = j;
                LerpP(&p->velocity[i], &accV[j - 1], &accV[j], t);
                break;
            }
        }
        p->position[i].q = end;
        p->velocity[i].q = endVel;
        count = i + 1;
        p->distance[i] = p->distance[i - 1] + GFromQw(Length(Sub(p->position[i].q, p->position[i - 1].q)));
        p->path->dist[i] = p->distance[i];
        p->path->count = count;
    } else {
        p->position[0].q = start;
        p->velocity[0].q = startVel;
        p->position[count - 1].q = end;
        p->velocity[count - 1].q = endVel;
        func_001AE5A0(&p->position[1], &accP[1], (count - 2) * 16);
        func_001AE5A0(&p->velocity[1], &accV[1], (count - 2) * 16);
    }
    M4 inverse;
    {
        PATH *path = p->path;
        VT *e = &path->vt[31];
        InvertMatrix4Rigid(&inverse, e->fn((char *)path + e->delta));
    }
    for (i = 0; i < count; i++)
    {
        PATH *path = p->path;
        path->pos[i].q = TransformPoint(inverse, p->position[i].q);
    }
    if (p->mode != 3) {
        p->path->dist[0] = 0.0f;
        for (i = 1; i < count; i++)
            p->path->dist[i] = p->path->dist[i - 1] + GFromQw(Length(Sub(p->path->pos[i - 1].q, p->path->pos[i].q)));
    }
    InvalidateShapeCache(p->path);
}
