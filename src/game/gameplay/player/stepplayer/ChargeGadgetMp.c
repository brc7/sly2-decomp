struct E { int a, b, c, d; int pad[4]; };
extern struct E D_002BCD00[];
extern float D_002DDED0;
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int a);
/** @sly2-annot addr=0x141CD0
 * Purpose: If id != -1 subtract the gadget's MP cost (D_002BCD00[id].d) from MpCur; stamp tGadgetUsed
 *   (+0x1304)
 * Info: proposed name ChargeGadgetMp (T1); TU stepplayer
 * Notes: runtime4: D_002BCD00[id]+0xC MP cost charged (100->90 for id 15)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void ChargeGadgetMp(char *p, int i)
{
    if (i != -1) SetStepplayerMp(p, MpStepplayerCur(p) - D_002BCD00[i].d);
    *(float *)(p + 0x1304) = D_002DDED0;
}
