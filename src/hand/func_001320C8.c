struct S { char pad[0x148]; unsigned long grf; };
void func_001F2108(struct S *);
/** @sly2-annot addr=0x1320C8
 * Purpose: BARRIER vt7 Init: InitSo then set an SO flag bit (grf |= 1<<34)
 * Info: proposed name InitBarrier (T1); class BARRIER slot7; TU barrier
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/gameplay-objects.md#key-functions
 * @end-sly2-annot */
void func_001320C8(struct S *p) { func_001F2108(p); p->grf |= 0x8000L << 19; }
