extern char D_002DEB30[];
void func_00161F10(void *pq, void *a, int n, void *b, int c, int d);
/** @sly2-annot addr=0x2284E0
 * Purpose: Add an RSMG entry to the EXIT 4-slot list (+0x164 count/+0x168 array) via 0x161F10
 * Info: proposed name AddExitRsmg (T1); TU exit_warp
 * Notes: names_v2 C proposed SetStepguardSgsExternal__FP9STEPGUARD3SGSP4ASEG (0.200) | v3: superseded
 *   AddWarpRsmgSm | CONFIRMED (optid 527 on EXIT; fields inside the 0x190-byte EXIT); v2 AddWarpRsmgSm
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_002284E0(char *p, int c, int d)
{
    func_00161F10(D_002DEB30, p + 0x164 + 4, 4, p + 0x164, c, d);
}
