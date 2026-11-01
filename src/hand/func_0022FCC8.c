typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct O { char pad[0x40]; M3 mat; };
void func_0011B340(M3 *pd, M3 *ps);
static inline qword Mul3(const M3 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]));
    return r;
}
/** @sly2-annot addr=0x22FCC8
 * Purpose: transforms a direction between XF spaces
 * Info: proposed name ConvertXfVec (T1); TU xf
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#xf-0x22c818-0x231100,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
qword func_0022FCC8(struct O *a, struct O *b, qword v)
{
    if (a) v = Mul3(&a->mat, v);
    if (b) {
        M3 m;
        func_0011B340(&m, &b->mat);
        v = Mul3(&m, v);
    }
    return v;
}
