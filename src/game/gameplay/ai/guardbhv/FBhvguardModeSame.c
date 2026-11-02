struct S { int k; int a[3]; int n10; int b[2]; int n1C; };
/** @sly2-annot addr=0x177DC0
 * Purpose: Compare two mode records (kind plus the kind's key field)
 * Info: proposed name FBhvguardModeSame (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+verdicts_followups+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvguardModeSame(struct S *a, struct S *b) {
    if (a->k != b->k) return 0;
    switch (a->k) {
    case 9: case 10: case 11: return a->n1C == b->n1C;
    case 4: return a->n10 == b->n10;
    case 5: return a->n10 == b->n10;
    case 3: return a->n10 == b->n10;
    case 13: return a->n10 == b->n10;
    case 2: return a->n10 == b->n10;
    case 14: return a->n10 == b->n10;
    case 18: return a->n10 == b->n10;
    case 6: return a->n10 == b->n10;
    }
    return 1;
}
