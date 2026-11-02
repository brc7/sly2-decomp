void LoadXfmFromBrx(void *p, void *q);
void CBinaryInputStream_SkipStringSw(void *q);
/** @sly2-annot addr=0x228140
 * Purpose: LoadXfmFromBrx + 0x118420 extra read
 * Info: proposed name LoadCid41FromBrx (T2); class CID41 slot14; TU exit_warp
 * Notes: names_v2 B proposed PostStepguardLoadCallback__FP9STEPGUARD5MSGIDPv (0.951) | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static naming pass; sources: r8_21BB88+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_00228140(void *p, void *q)
{
    LoadXfmFromBrx(p, q);
    CBinaryInputStream_SkipStringSw(q);
}
