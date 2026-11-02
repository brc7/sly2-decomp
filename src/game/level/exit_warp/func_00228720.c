void LoadXfmFromBrx(void *p, void *q);
void CBinaryInputStream_SkipStringSw(void *q);
/** @sly2-annot addr=0x228720
 * Purpose: LoadXfmFromBrx + 0x118420
 * Info: proposed name LoadWarpFromBrx (T1); class WARP slot14; TU exit_warp
 * Notes: CID42 WARP (crsmg/arsmg[4] at +0xE0) LoadFromBrx; r8 LoadCid42FromBrx | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: verify_r7r8_summary+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_00228720(void *p, void *q)
{
    LoadXfmFromBrx(p, q);
    CBinaryInputStream_SkipStringSw(q);
}
