struct S { char pad[0x1D0]; int c; int a[1]; };
/** @sly2-annot addr=0x1FE990
 * Purpose: Index of id in a small int table (-1)
 * Info: proposed name IambFind (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FE990(struct S *p, int n) { int i; for (i = 0; i < p->c; i++) if (p->a[i] == n) return i; return -1; }
