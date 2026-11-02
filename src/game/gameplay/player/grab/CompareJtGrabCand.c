struct S { int a, b; float g; };
/** @sly2-annot addr=0x194F48
 * Purpose: qsort comparator: -1 if a->g < b->g else 1
 * Info: proposed name CompareJtGrabCand (T1); TU grab
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
int CompareJtGrabCand(struct S *a, struct S *b) { if (a->g < b->g) return -1; return 1; }
