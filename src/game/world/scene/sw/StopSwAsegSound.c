struct O { char pad[0x60]; int id; };
void StopSnd(struct O *p, float f);
void func_00258040(struct O **pp, int a);
/** @sly2-annot addr=0x2108E8
 * Purpose: ASEG event 1: StopSnd stored continuous sound (optionally matching id) and release ref
 * Info: proposed name StopSwAsegSound (T1); TU sw
 * Notes: names_v2 v1 proposed InitVolbtn__FP6VOLBTN (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8+verify_side
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void StopSwAsegSound(void *a, int id, void *b, struct O **pp)
{
    struct O *p = *pp;
    if (!p)
        return;
    if (id != -1 && p->id != id)
        return;
    StopSnd(p, 0.0f);
    func_00258040(pp, 0);
}
