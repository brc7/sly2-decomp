extern char D_002DDF40[];
void func_001C3FF0(void *p, int b);
void func_0014E328(void *g, int a, void *h, void *p, int z);
void func_0014E4C8(void *g, int a, int b, void *h, void *p, int z);
/** @sly2-annot addr=0x225AE8
 * Purpose: OnPoActive then SetCmPolicy(g_cm,9,g_cm+0x5F0,..) or RevokeCmPolicy
 * Info: proposed name OnTurretActive (T1); class TURRET slot78; TU turret
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00225AE8(void *p, int b)
{
    func_001C3FF0(p, b);
    if (b) func_0014E328(D_002DDF40, 9, D_002DDF40 + 0x5F0, p, 0);
    else func_0014E4C8(D_002DDF40, 0xF, 9, D_002DDF40 + 0x5F0, p, 0);
}
