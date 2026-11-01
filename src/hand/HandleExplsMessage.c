struct O { char pad[0x100]; void *p100; void *p104; };
void UnsubscribeLoObject(void *pv, void *po);
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
