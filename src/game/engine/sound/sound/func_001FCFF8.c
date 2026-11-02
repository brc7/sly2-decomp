struct T { char pad[0x40]; int n; struct T *pnext; };
struct S { char pad[0xF0]; struct T *pt; };
/** @sly2-annot addr=0x1FCFF8
 * Purpose: Find SNDSTREAM by id (music list)
 * Info: proposed name PsndstreamFindMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct T *func_001FCFF8(struct S *p, int n) { struct T *pt; for (pt = p->pt; pt; pt = pt->pnext) if (pt->n == n) return pt; return 0; }
