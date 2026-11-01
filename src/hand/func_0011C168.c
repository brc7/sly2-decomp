typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
/** @sly2-annot addr=0x11C168
 * Purpose: 4x4 matrix product
 * Info: proposed name MultiplyMatrix4Matrix4 (T1); TU mat
 * Notes: operator* on MATRIX4 (4x4 by 4x4); Sly1 symbol __ml__FRC7MATRIX4T0 (was operator)
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: verify_A1+verdicts_A1
 * See: docs/engine/animation.md#devtools-findings-v3-2026-11-01, docs/engine/core-library.md#3-math,
 *   docs/engine/core-library.md#7-key-functions
 * @end-sly2-annot */
M4 *func_0011C168(M4 *pm, const M4 *a, const M4 *b)
{
    register qword v1 __asm__("vf1") = a->r[0];
    register qword v2 __asm__("vf2") = a->r[1];
    register qword v3 __asm__("vf3") = a->r[2];
    register qword v4 __asm__("vf4") = a->r[3];
    register qword v5 __asm__("vf5") = b->r[0];
    register qword v6 __asm__("vf6") = b->r[1];
    register qword v7 __asm__("vf7") = b->r[2];
    register qword v8 __asm__("vf8") = b->r[3];
    int t;
    __asm__ __volatile__("vcallms 0x60" : "=j"(v5), "=j"(v6), "=j"(v7), "=j"(v8)
        : "j"(v1), "j"(v2), "0"(v5), "1"(v6), "2"(v7), "3"(v8));
    __asm__ __volatile__("cfc2.i %0, $vi0" : "=r"(t) : "j"(v3), "j"(v4), "j"(v5), "j"(v6), "j"(v7), "j"(v8));
    pm->r[0] = v5; pm->r[1] = v6; pm->r[2] = v7; pm->r[3] = v8;
    return pm;
}
