class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
class O : public OBase {
public:
    char pad[0x7C - 0x10];
    O *f7c;
};
struct Q { int f0, f4, f8; };
extern Q D_002DEB30;
extern "C" int func_001637E8(O *p);
extern "C" int func_00162670(O *p);
extern "C" void func_001619A0(void *pv, int n, O *p);
/** @sly2-annot addr=0x163E88
 * Purpose: Refused state change: event 0x27 (arg in g_game+8), SendMessage 0x12(n) to the task and to its job
 * Info: proposed name RejectTaskState (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00163E88(O *p, int n)
{
    void *apv[1];
    apv[0] = &n;
    if (func_001637E8(p)) {
        D_002DEB30.f8 = n;
        func_001619A0(&D_002DEB30, 0x27, p);
        p->Msg(0x12, 0, 1, apv, 0);
    }
    if (p->f7c && func_00162670(p->f7c))
        p->f7c->Msg(0x12, 0, 1, apv, 0);
}
