extern char D_002DDF40[];
void OnPoActive(void *p, int b);
void SetCmPolicy(void *g, int a, void *h, void *p, int z);
void RevokeCmPolicy(void *g, int a, int b, void *h, void *p, int z);
/** @sly2-annot addr=0x225AE8
 * Purpose: OnPoActive then SetCmPolicy(g_cm,9,g_cm+0x5F0,..) or RevokeCmPolicy
 * Info: proposed name OnTurretActive (T1); class TURRET slot78; TU turret
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void OnTurretActive(void *p, int b)
{
    OnPoActive(p, b);
    if (b) SetCmPolicy(D_002DDF40, 9, D_002DDF40 + 0x5F0, p, 0);
    else RevokeCmPolicy(D_002DDF40, 0xF, 9, D_002DDF40 + 0x5F0, p, 0);
}
