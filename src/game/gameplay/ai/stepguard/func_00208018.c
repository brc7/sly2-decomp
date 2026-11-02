struct E { int a, b, c, d; };
void func_00207F50(void *p, int mask, int b, int d, struct E *records);
/** @sly2-annot addr=0x208018
 * Purpose: 0x207F50 on table +0xC10
 * Info: proposed name SetStepguardZapReactionsC10Resolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208018(char *p, int mask, int b, int d)
{
    func_00207F50(p, mask, b, d, (struct E *)(p + 0xC10));
}
