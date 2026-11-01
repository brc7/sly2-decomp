class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
class O : public OBase {
public:
    char pad[0x50 - 0x10];
    int f50;
    int c54;
    int *a58;
    int c5C;
    int *a60;
};
struct G;
extern G D_003DD490;
extern "C" int FJobLive(O *p);
extern "C" int func_0020E040(G *pg);
extern "C" void func_00162DD0(O *p);
extern "C" void func_00162E38(O *p);
extern "C" void func_00162DF8(O *p);
/** @sly2-annot addr=0x162BB8
 * Purpose: Job's game-event hook: if live SendMessage 0x11(ev,arg) and check its complete-event list (->
 *   0x162DD0) and fail-event list (-> FailJob); if not live, event 0x10 restarts it (RestartJob)
 * Info: proposed name HandleJobGameEvent (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest+verdicts_B
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void HandleJobGameEvent(O *p, int n, int m)
{
    if (FJobLive(p)) {
        void *apv[2];
        int i;
        apv[0] = &n;
        apv[1] = &m;
        p->Msg(0x11, 0, 2, apv, 0);
        for (i = 0; i < p->c54; i++) {
            if (n == p->a58[i]) {
                func_00162DD0(p);
                break;
            }
        }
        for (i = 0; i < p->c5C; i++) {
            if (n == p->a60[i]) {
                func_00162E38(p);
                break;
            }
        }
    } else if (n == 0x10 && p->f50 && !func_0020E040(&D_003DD490)) {
        func_00162DF8(p);
    }
}
