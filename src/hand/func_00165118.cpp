class O {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
extern "C" int FTaskLive(O *p);
/** @sly2-annot addr=0x165118
 * Purpose: If live, SendMessage 0x17(n)
 * Info: proposed name SendTaskTimeout (T2); TU game
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00165118(O *p, int n)
{
    if (FTaskLive(p)) {
        void *apv[1];
        apv[0] = &n;
        p->Msg(0x17, 0, 1, apv, 0);
    }
}
