struct EMITTER { char pad[0x14C]; void *p14C; void *p150; };
enum MSGID { MSGID_0, MSGID_1 };
void func_00133448(void *pv, void *po);
/** @sly2-annot addr=0x1599A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void HandleEmitterMessage(EMITTER *pemitter, MSGID msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == pemitter->p14C) {
            func_00133448(pv, pemitter);
            pemitter->p14C = 0;
        } else if (pv == pemitter->p150) {
            func_00133448(pv, pemitter);
            pemitter->p150 = 0;
        }
    }
}
