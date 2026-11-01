struct S { char pad[0x2C0]; long fl; char pad2[0xE64 - 0x2C8]; int nE64; };
long GrfFromSgf(int n);
/** @sly2-annot addr=0x208D70
 * Purpose: +0xE64 unless any of flags {0xB,0x1D,7,5,6,0x10,1,0x16,0x2A} is set
 * Info: proposed name FStepguardSensesEnabled (T1); TU stepguard
 * Notes: names_v2 C proposed SetCamsenCsdts__FP6CAMSEN5CSDTS (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7b_guard
 * See: docs/engine/guards-ai.md#5-perception,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int FStepguardSensesEnabled(struct S *p)
{
    long m = GrfFromSgf(0xB) | GrfFromSgf(0x1D) | GrfFromSgf(7) | GrfFromSgf(5) | GrfFromSgf(6)
           | GrfFromSgf(0x10) | GrfFromSgf(1) | GrfFromSgf(0x16) | GrfFromSgf(0x2A);
    if (m & p->fl)
        return 0;
    return p->nE64;
}
