struct S { char pad[0xEC]; void *p; int n; };
int FTaskComplete(void *p);
/** @sly2-annot addr=0x203758
 * Purpose: task done ? or local flag
 * Info: proposed name FReconTargetDone (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
int FReconTargetDone(struct S *p) { if (p->p) return FTaskComplete(p->p); return p->n; }
