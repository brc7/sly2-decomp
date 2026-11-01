struct RUMBLE;
struct RUMPAT { char ab[0x104]; };
enum RUMK { RUMK_Nil = -1 };
extern RUMPAT D_002C2260[];
void TriggerRumbleRumpat(RUMBLE *prumble, RUMPAT *prumpat, float dt);
/** @sly2-annot addr=0x1D5310
 * Purpose: Trigger predefined pattern D_002C2260[rumk]
 * Info: proposed name TriggerRumbleRumk__FP6RUMBLE4RUMKf (EXISTING); TU rumble
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
void TriggerRumbleRumk(RUMBLE *prumble, RUMK rumk, float dt)
{
    TriggerRumbleRumpat(prumble, &D_002C2260[rumk], dt);
}
