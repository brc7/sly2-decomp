extern char D_003DD490[];
void FAddRsmg__FP2SWP4RSMGiPii3OIDN25(void *pq, void *a, int n, void *b, int c, int d, int e, int f);
/** @sly2-annot addr=0x2288D8
 * Purpose: FAddRsmg(g_sw, p+0xE4, 4, p+0xE0, ...) when fOnTrigger
 * Info: proposed name SetWarpRsmg (T1); TU exit_warp
 * Notes: verifier WRONG: SetCid41Rsmg -> SetWarpRsmg | Sly1 SetWarpRsmg; class is CID 0x2A=42 WARP (r8
 *   SetCid41Rsmg was CID 0x29)
 * Confidence: high [T1 0.80] - evidence: Sly 1 / Lilly reference; sources:
 *   verify_r7r8+CONFLICTS#11+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void SetWarpRsmg(char *p, int c, int d, int e, int f)
{
    if (c) FAddRsmg__FP2SWP4RSMGiPii3OIDN25(D_003DD490, p + 0xE4, 4, p + 0xE0, c, d, e, f);
}
