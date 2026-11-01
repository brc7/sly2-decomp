extern int D_003E1080;
extern int D_00443740;
/** @sly2-annot addr=0x20E040
 * Purpose: True if D_003E1080!=0 or D_00443740==1 (checkpoint-manager state)
 * Info: proposed name FSwTransitionBusy (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020E040(void) { if (D_003E1080 != 0) return 1; return D_00443740 == 1; }
