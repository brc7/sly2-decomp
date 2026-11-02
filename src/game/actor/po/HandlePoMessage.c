struct S { char pad[0x174]; void *p174; };
void HandleBasicMessage(struct S *p, int n);
void StopSnd(void *p, float g);
void RcPtr_Assign(void **pp, int n);
/** @sly2-annot addr=0x1C3E20
 * Purpose: PO::HandleMessage (vt slot 5): HandleLoMessage base, then on msgid 5 stop the wake ambient sound
 *   at +0x174 and release it
 * Info: proposed name HandlePoMessage (T1); class PO slot5; TU po
 * Notes: names_v2 C proposed RovtsNextRov__FP3ROV (0.638)
 * Confidence: high [T1 0.88] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void HandlePoMessage(struct S *p, int n)
{
    HandleBasicMessage(p, n);
    if (n == 5) {
        void **pp = &p->p174;
        if (pp && *pp) {
            StopSnd(*pp, 0.0f);
            RcPtr_Assign(pp, 0);
        }
    }
}
