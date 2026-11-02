struct S { char pad[0xFC]; int fl; };
/** @sly2-annot addr=0x184040
 * Purpose: Map player class to mask: STEPPLAYER 0x34 -> 7, JT 0x35
 * Info: proposed name SetGrabspotPlayerClass (T2); TU grabspot
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184040(struct S *p, int n)
{
    switch (n) {
    case 0x34: p->fl = 7; break;
    case 0x35: p->fl |= 1; break;
    case 0x37: p->fl |= 2; break;
    case 0x36: p->fl |= 4; break;
    }
}
