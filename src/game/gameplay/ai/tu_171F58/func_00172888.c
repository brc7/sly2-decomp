struct S { char pad[0x10]; float g10; };
/** @sly2-annot addr=0x172888
 * Purpose: Sort predicate on heading
 * Info: proposed name FCompareSearchCandidates (T2); TU tu_171F58
 * Notes: names_v2 C proposed GetAloThrobInColor__FP3ALOP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
int func_00172888(struct S *a, struct S *b) { return a->g10 < b->g10; }
