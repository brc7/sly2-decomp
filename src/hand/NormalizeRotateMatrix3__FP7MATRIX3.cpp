typedef int qword __attribute__((mode(TI)));
struct MATRIX3 { qword r[3]; };
static inline qword Normalize(qword v)
{
    qword d, one, l, r;
    __asm__("vmul.xyz %0, %1, %1\n\tvaddw.x $vf4, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, $vf4, %0z" : "=j"(d) : "j"(v) : "vf4");
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
/** @sly2-annot addr=0x11B3D0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name NormalizeRotateMatrix3__FP7MATRIX3 (EXISTING); TU mat
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void NormalizeRotateMatrix3(MATRIX3 *pmat)
{
    qword v0 = Normalize(pmat->r[0]);
    qword c = Cross(v0, pmat->r[1]);
    pmat->r[0] = v0;
    qword v2 = Normalize(c);
    c = Cross(v2, v0);
    pmat->r[2] = v2;
    pmat->r[1] = Normalize(c);
}
