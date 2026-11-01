struct S { char pad[0xe8]; int fe8; };
extern char D_003DD490[];
void PostSoLoad(struct S *);
void RecalcSwOxfFilterForObject(void *, struct S *);
/** @sly2-annot addr=0x132100
 * Purpose: BARRIER vt15 PostLoad: PostSoLoad, then if it has physics (+0xE8) register it with g_sw (0x134320,
 *   pairs with every other registered SO)
 * Info: proposed name PostBarrierLoad (T1); class BARRIER slot15; TU barrier
 * Notes: names_v2 B proposed CloneBarrier__FP7BARRIERT0 (0.808)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570
 * @end-sly2-annot */
void PostBarrierLoad(struct S *p) { PostSoLoad(p); if (p->fe8) RecalcSwOxfFilterForObject(D_003DD490, p); }
