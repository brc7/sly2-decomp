struct S { char pad[0x110]; int x; };
void func_00231100(struct S *p);
/** @sly2-annot addr=0x227350
 * Purpose: InitXfm; state 0x110 = -1
 * Info: proposed name InitCid5 (T2); class CID5 slot7; TU vis
 * Notes: verifier PLAUSIBLE: InitCid5Zone -> InitCid5
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/rendering.md#12-source-files, docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#vis-0x227350-0x2275b8
 * @end-sly2-annot */
void func_00227350(struct S *p)
{
    func_00231100(p);
    p->x = -1;
}
