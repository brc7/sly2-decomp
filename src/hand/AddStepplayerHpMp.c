void AddPoHpMp(void *p, int a, int b);
void SetStepplayerHp(void *p, int a);
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int a);
/** @sly2-annot addr=0x144268
 * Purpose: vt81: PO base then hp += dhp, mp += dmp (clamped)
 * Info: proposed name AddStepplayerHpMp (T1); class STEPPLAYER slot81; TU stepplayer
 * Notes: mangled dropped (base does not match final name): AddHpMp__10STEPPLAYERii
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void AddStepplayerHpMp(char *p, int a, int b)
{
    AddPoHpMp(p, a, b);
    SetStepplayerHp(p, **(int **)(p + 0xE00) + a);
    SetStepplayerMp(p, MpStepplayerCur(p) + b);
}
