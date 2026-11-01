struct E { int a, b, c, d; };
void func_00207EB8(void *p, int mask, int b, int d, struct E *records);
/** @sly2-annot addr=0x207FF8
 * Purpose: 0x207EB8 on table +0xC10
 * Info: proposed name SetStepguardZapReactionsC10 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207FF8(char *p, int mask, int b, int d)
{
    func_00207EB8(p, mask, b, d, (struct E *)(p + 0xC10));
}
