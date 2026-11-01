class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
struct LO { char pad[0x40]; int fl; };
class O : public OBase {
public:
    char pad[0x5C - 0x10];
    int f5c;
    int f60;
    int f64;
    char pad1[0x7C - 0x68];
    O *f7c;
    char pad2[0xA8 - 0x80];
    int fa8, fac, fb0, fb4, fb8;
    char pad3[0xC4 - 0xBC];
    LO *fc4;
    char pad4[0xF4 - 0xC8];
    int ff4;
};
struct G;
extern G D_003DD490;
extern char D_002DEB30[];
extern char D_003DA0C0[];
extern "C" void SetSwTaskCur(G *pg, O *p);
extern "C" void SetSwTaskPending(G *pg, O *p);
extern "C" int FIsCurrentJob(O *p);
extern "C" void ActivateJob(O *p);
extern "C" int FTaskLive(O *p);
extern "C" void UpdateTaskSaveVars(O *p);
extern "C" void BroadcastGameEvent(void *pv, int n, O *p);
extern "C" void ApplyStateLoVisibility(O *p, int n, int c, int a);
extern "C" void TriggerStateRsmgs(O *p, int n, int c, int a, int b);
extern "C" void func_001DA4B8(void *pv, int n, float g);
extern "C" void func_002283D0(LO *plo);
/** @sly2-annot addr=0x163D18
 * Purpose: Makes p the current task (and pending task if it has a warp), activates its job, if live: ++count,
 *   event 0x23, state-4 visuals, starts its music (+0x5C, 0x1DA4B8), SendMessage 0x13, triggers LO +0xC4
 * Info: proposed name BeginTask (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void BeginTask(O *p)
{
    G *pg = &D_003DD490;
    SetSwTaskCur(pg, p);
    if (p->fb8 != -1)
        SetSwTaskPending(pg, p);
    if (p->f7c && !FIsCurrentJob(p->f7c))
        ActivateJob(p->f7c);
    if (FTaskLive(p)) {
        p->f64++;
        UpdateTaskSaveVars(p);
        BroadcastGameEvent(D_002DEB30, 0x23, p);
        ApplyStateLoVisibility(p, 4, p->fa8, p->fac);
        TriggerStateRsmgs(p, 4, p->fb0, p->fb4, p->ff4);
        if (p->f5c != -1 && p->f60)
            func_001DA4B8(D_003DA0C0, p->f5c, -1.0f);
        p->Msg(0x13, 0, 0, 0, 0);
        if (p->fc4) {
            LO *plo = p->fc4;
            int v = plo->fl;
            if ((v ^ 1) & 1)
                *(int *)v |= 0x100;
            else
                plo->fl = v | 0x100;
            func_002283D0(p->fc4);
        }
    }
}
