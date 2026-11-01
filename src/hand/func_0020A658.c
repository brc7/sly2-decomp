struct S { char pad[0x28]; int c; int a[1]; };
/** @sly2-annot addr=0x20A658
 * Purpose: Index of guard in the record's guard list (+0x28 count, +0x2C)
 * Info: proposed name IguardFindTargetRecord (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A658(struct S *p, int n) { int i; for (i = 0; i < p->c; i++) if (n == p->a[i]) return i; return -1; }
