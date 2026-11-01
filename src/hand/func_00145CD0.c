void InitBhv(void *p);
/** @sly2-annot addr=0x145CD0
 * Purpose: BHV195 slot 7: base, state -1
 * Info: proposed name InitBhv195 (T2); class BHV195 slot7; TU tu_144B28
 * Notes: names_v2 C proposed InitCamsen__FP6CAMSEN (0.438) | superseded: OnBhv195Push (r2b_rest) | BHV vtable
 *   slot 7 role per r9 (CONFLICTS #15); was OnBhv195Push
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00145CD0(char *p)
{
    InitBhv(p);
    *(int *)(p + 0x30) = -1;
}
