typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct P { char pad[0xC]; float gC; char pad1[0x38 - 0x10]; float g38; float g3C; char pad2[0x48 - 0x40]; float g48; float g4C; float g50; float g54; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(void *pemitvx, float a, float b, struct P *plm, qword *pv, int n, qword *pvOut);
float func_0011D448(float a, float b);

/** @sly2-annot addr=0x15B0C8
 * Purpose: Chooses per-particle velocity (ChooseEmitVelocity) and age
 * Info: proposed name ChooseEmitvVelocityAge (T1); TU emitter
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015B0C8(struct P *p, void *pemitvx, int unused, int n, qword *ppos, qword *pv, qword *pvel, float *ptBirth, float *ptDeath)
{
    float t, g;
    ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(pemitvx, p->g38, p->gC, p, pv, n, pvel);
    t = D_002DDED0.t;
    t = t - func_0011D448(p->g50, p->g54);
    *ptBirth = t;
    *ptDeath = t + p->g3C;
    g = func_0011D448(p->g48, p->g4C);
    if (g != 0.0f) {
        U u;
        qword r;
        __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(g));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(*ppos), "j"(*pvel), "j"(u.q));
        *ppos = r;
    }
}
