struct O { char pad[0x28]; unsigned long fl; char pad2[0x24]; int n54; char pad3[0x94]; struct O *pEC; char pad4[0x10]; struct O *p100; };
#define BIT60(o) ((int)((o)->fl >> 60) & 1)
/** @sly2-annot addr=0x203790
 * Purpose: visible and task state 1
 * Info: proposed name FReconTargetSelectable (T1); TU tu_201D78
 * Notes: names_v2 C proposed UpdateZapbreak__FP8ZAPBREAKf (0.200)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
int FReconTargetSelectable(struct O *p)
{
    struct O *o;
    if (!BIT60(p)) return 0;
    if (p->p100 && BIT60(p->p100)) return 0;
    o = p->pEC;
    if (o && o->n54 != 1) return 0;
    return 1;
}
