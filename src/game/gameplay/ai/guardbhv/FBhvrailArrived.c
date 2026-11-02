struct T { char pad[0xF88]; float gF88; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; float g3C; };
/** @sly2-annot addr=0x17CE30
 * Purpose: |param - goal| < 50
 * Info: proposed name FBhvrailArrived (T1); TU guardbhv
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvrailArrived(struct S *p) { return __builtin_fabsf(p->pt->gF88 - p->g3C) < 50.0f; }
