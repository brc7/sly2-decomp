struct E { int a, b, c, d; };
void func_00207EB8(void *p, int mask, int b, int d, struct E *records);
/** @sly2-annot addr=0x208078
 * Purpose: 0x207EB8 on table +0x8D0
 * Info: proposed name SetStepguardZapReactions8D0 (T2); TU stepguard
 * Notes: verifier PLAUSIBLE: Offset-suffixed placeholder. SetZapReactionBindings(p, mask, b, d, p+0x8D0);
 *   caller splice wrapper. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208078(char *p, int mask, int b, int d)
{
    func_00207EB8(p, mask, b, d, (struct E *)(p + 0x8D0));
}
