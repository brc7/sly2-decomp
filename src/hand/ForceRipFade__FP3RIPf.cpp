struct RIP { char pad[0x14]; float t14; float g18; };
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
/** @sly2-annot addr=0x1CEAA8
 * Purpose: start fade
 * Info: proposed name ForceRipFade__FP3RIPf (EXISTING); TU rip
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void ForceRipFade(RIP *prip, float dt)
{
    float t = prip->g18 - (D_002DDED0.t - prip->t14);
    if (dt < t) {
        float u = t / prip->g18;
        prip->g18 = dt / u;
        prip->t14 = D_002DDED0.t - (1.0f - u) * prip->g18;
    }
}
