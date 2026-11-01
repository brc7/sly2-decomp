struct EMITTER { char pad[0x14C]; void *p14C; void *p150; };
enum MSGID { MSGID_0, MSGID_1 };
void UnsubscribeLoObject(void *pv, void *po);
void HandleEmitterMessage(EMITTER *pemitter, MSGID msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == pemitter->p14C) {
            UnsubscribeLoObject(pv, pemitter);
            pemitter->p14C = 0;
        } else if (pv == pemitter->p150) {
            UnsubscribeLoObject(pv, pemitter);
            pemitter->p150 = 0;
        }
    }
}
