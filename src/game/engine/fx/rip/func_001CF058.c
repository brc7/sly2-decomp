typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x1CF058
 * Purpose: helper: |v|^2 < 25
 * Info: proposed name FVecShorterThan5 (T2); TU rip
 * Notes: names_v2 v1 proposed InitDroplet__FP7DROPLETP6VECTORfP2SO (0.023)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int func_001CF058(qword v)
{
    float g;
    float lim;
    __asm__ __volatile__("qmtc2.ni %0, $vf3" : : "r"(v));
    lim = 25.0f;
    __asm__ __volatile__("vmul.xyz $vf1, $vf3, $vf3" : : "f"(lim));
    __asm__ __volatile__("vaddw.x $vf2, $vf0, $vf0w");
    __asm__ __volatile__("vadday.x ACC, $vf1, $vf1y");
    __asm__ __volatile__("vmaddz.x $vf1, $vf2, $vf1z");
    __asm__ __volatile__("qmfc2.ni %0, $vf1" : "=r"(g));
    return g < lim;
}
