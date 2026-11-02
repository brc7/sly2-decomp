struct O { char pad[0x108]; int n; };
struct S { char pad[0x30]; int c; struct O *a[1]; };
/** @sly2-annot addr=0x2022A0
 * Purpose: target +0x108 = 0
 * Info: proposed name ClearReconTargetMarks (T2); TU tu_201D78
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_002022A0(struct S *p) { int i; for (i = 0; i < p->c; i++) p->a[i]->n = 0; }
