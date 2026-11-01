typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
qword ml(const M4 *m, qword v) __asm__("__ml__FRC7MATRIX4G10VU_VECTOR4");
/** @sly2-annot addr=0x11C1A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
qword ml(const M4 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmadday.xyzw $ACC, %3, %1y\n\tvmaddaz.xyzw $ACC, %4, %1z\n\tvmaddw.xyzw %0, %5, %1w"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
