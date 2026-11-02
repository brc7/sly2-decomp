struct WRE { char pad[0x64]; float x; };
struct WRE *func_0022C4D8(void *p, int a);
/** @sly2-annot addr=0x22C570
 * Purpose: 1/circle frequency (0 if 0)
 * Info: proposed name GetWrWavelength (T1); TU wr
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+verdicts_A2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
float GetWrWavelength(void *p)
{
    struct WRE *pwre = func_0022C4D8(p, 1);
    if (pwre->x == 0.0f) return 0.0f;
    return 1.0f / pwre->x;
}
