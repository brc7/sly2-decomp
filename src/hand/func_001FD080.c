struct T { char pad[0x44]; struct T *pnext; };
struct S { char pad[0xF0]; struct T *pt; struct T *pt2; };
/** @sly2-annot addr=0x1FD080
 * Purpose: 1 if record is in list A, 0 if it is the ambience record, else 2: picks volume channel
 * Info: proposed name ChannelFromPsndstream (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FD080(struct S *p, struct T *x) { struct T *pt; for (pt = p->pt; pt; pt = pt->pnext) if (pt == x) return 1; return p->pt2 != x ? 2 : 0; }
