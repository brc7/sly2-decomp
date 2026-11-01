class O {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
extern "C" int FTaskLive(O *p);
/** @sly2-annot addr=0x165170
 * Purpose: If live, SendMessage 0x11(event, arg)
 * Info: proposed name SendTaskGameEvent (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void SendTaskGameEvent(O *p, int n, int m)
{
    if (FTaskLive(p)) {
        void *apv[2];
        apv[0] = &n;
        apv[1] = &m;
        p->Msg(0x11, 0, 2, apv, 0);
    }
}
