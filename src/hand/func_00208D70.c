struct S { char pad[0x2C0]; long fl; char pad2[0xE64 - 0x2C8]; int nE64; };
long func_00207A20(int n);
/** @sly2-annot addr=0x208D70
 * Purpose: +0xE64 unless any of flags {0xB,0x1D,7,5,6,0x10,1,0x16,0x2A} is set
 * Info: proposed name FStepguardSensesEnabled (T1); TU stepguard
 * Notes: names_v2 C proposed SetCamsenCsdts__FP6CAMSEN5CSDTS (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7b_guard
 * See: docs/engine/guards-ai.md#5-perception,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00208D70(struct S *p)
{
    long m = func_00207A20(0xB) | func_00207A20(0x1D) | func_00207A20(7) | func_00207A20(5) | func_00207A20(6)
           | func_00207A20(0x10) | func_00207A20(1) | func_00207A20(0x16) | func_00207A20(0x2A);
    if (m & p->fl)
        return 0;
    return p->nE64;
}
