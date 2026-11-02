struct RUMBLE { int nPort; int nSlot; };
extern "C" int scePadSetActDirect(int, int, unsigned char *);
/** @sly2-annot addr=0x1D5468
 * Purpose: Zero both motors
 * Info: proposed name StopRumbleActuators__FP6RUMBLE (EXISTING); TU rumble
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
void StopRumbleActuators(RUMBLE *prumble)
{
    unsigned char abShockDirect[6];
    abShockDirect[0] = 0;
    abShockDirect[1] = 0;
    scePadSetActDirect(prumble->nPort, prumble->nSlot, abShockDirect);
}
