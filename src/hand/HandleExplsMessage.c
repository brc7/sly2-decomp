struct O { char pad[0x100]; void *p100; void *p104; };
void UnsubscribeLoObject(void *pv, void *po);
/** @sly2-annot addr=0x15C5C8
 * Purpose: EXPLS::HandleMessage: on msg 1 (removed) forgets cached p100/p104 references
 * Info: proposed name HandleExplsMessage (T1); class EXPLS slot5; TU emitter
 * Notes: names_v2 B proposed BindExplg__FP5EXPLG (0.856)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void HandleExplsMessage(struct O *o, int msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == o->p100) {
            UnsubscribeLoObject(pv, o);
            o->p100 = 0;
        } else if (pv == o->p104) {
            UnsubscribeLoObject(pv, o);
            o->p104 = 0;
        }
    }
}
