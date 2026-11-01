// Follow an animated target: sample the target's pose (LoadIdentityMatrix4), blend the end velocity of a Hermite step from
// the position delta, the current velocity and the acceleration, run the step (EvaluateBezierPos), then spin the
// orientation about its axis in proportion to the elapsed time.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct M4 { qword r[4]; };
union U { qword q; float f; };
struct X { char pad[0x60]; void *data; };
struct S {
    char pad[0x14]; float start, duration; X *x; char pad20[0x40 - 0x20];
    M3 mat; qword pos, prev, vel, accel; char padB0[0xF0 - 0xB0]; qword axis; float angle;
};
extern struct CLOCK { float t, dt; } D_002DDED0;
extern "C" {
void LoadIdentityMatrix4(void *, int, M4 *, M4 *, float);
void EvaluateBezierPos(float, float, float, qword, qword, qword, qword, qword *, qword *, qword *);
M3 *func_0011B500(M3 *, qword, float);
void func_001CFE88(S *);
}
// returns an M3 built by func_0011B500 straight into the (named) return slot
static inline M3 Rotation(qword axis, float angle) return result { func_0011B500(&result, axis, angle); }
// broadcast a float into a VU register; the union local is the dead scratch store the original left on the stack
static inline qword Splat(float f) { U u; qword q; __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); u.q = q; return q; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Scale(qword a, qword s) { __asm__("vmulx.xyzw %0,%0,%1x" : "+j"(a) : "j"(s)); return a; }
// d*s1.x + v*s2.x + a*s3.x
static inline qword Blend(qword d, qword v, qword a, qword s1, qword s2, qword s3)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%1,%4x\n\tvmaddax.xyzw $ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x"
            : "=j"(r) : "j"(d), "j"(v), "j"(a), "j"(s1), "j"(s2), "j"(s3));
    return r;
}
static inline float Max(float a, float b) { return a < b ? b : a; }
// a (3x3 rotation) * b, row by row
static inline M3 Mul(const M3 &a, const M3 &b) return r {
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z" : "=j"(r.r[0]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[0]));
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z" : "=j"(r.r[1]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[1]));
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z" : "=j"(r.r[2]) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[2]));
}
/** @sly2-annot addr=0x1CFE88
 * Purpose: MATCH Project: Hermite-follow target pose
 * Info: proposed name ProjectMatchTransform (T1); TU rip
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001CFE88(S *p)
{
    CLOCK *clock = &D_002DDED0;
    if (clock->t < p->start) return;
    M4 m;
    M3 spin;
    p->prev = p->pos;
    float remain = Max(clock->dt, p->start + p->duration - clock->t);
    LoadIdentityMatrix4(p->x->data, 0, &m, 0, remain);
    qword delta = Sub(m.r[3], p->pos);
    qword w1 = Splat(1.5f / remain);
    qword w2 = Splat(-0.5f);
    qword w3 = Splat(0.25f);
    qword dt = Splat(remain);
    float cdt = clock->dt;
    qword v = p->vel;
    qword endvel = Blend(delta, v, Scale(p->accel, dt), w1, w2, w3);
    EvaluateBezierPos(remain, cdt, 1.0f, p->pos, v, m.r[3], endvel, &p->pos, &p->vel, 0);
    spin = Rotation(p->axis, p->angle * remain / p->duration);
    p->mat = Mul(spin, *(M3 *)&m);
}

#define OFF(t, m) ((int)&((t *)0)->m)
typedef char follow_layout[(sizeof(qword) == 16 && sizeof(M3) == 0x30 && sizeof(M4) == 0x40 && sizeof(U) == 16 && OFF(CLOCK, t) == 0 && OFF(CLOCK, dt) == 4 &&
                            OFF(X, data) == 0x60 && OFF(S, start) == 0x14 && OFF(S, duration) == 0x18 && OFF(S, x) == 0x1C && OFF(S, mat) == 0x40 && OFF(S, pos) == 0x70 &&
                            OFF(S, prev) == 0x80 && OFF(S, vel) == 0x90 && OFF(S, accel) == 0xA0 && OFF(S, axis) == 0xF0 && OFF(S, angle) == 0x100) ? 1 : -1];
