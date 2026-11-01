struct G;
extern struct G D_003DD490;
struct S { char pad[0xFC8]; int n; };
int func_0020F1B8(struct G *p);
/** @sly2-annot addr=0x209098
 * Purpose: +0xFC8 or FSwInputLock(g_sw)
 * Info: proposed name FStepguardHandsOff (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00209098(struct S *p) { if (p->n != 0) return 1; return func_0020F1B8(&D_003DD490); }
