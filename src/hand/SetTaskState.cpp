class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
    virtual void V4(int msgid, void *pv);
};
class O : public OBase {
public:
    int f10;
    int f14;
    int f18;
    char pad[0x54 - 0x1C];
    int n54;
    float f58;
    char pad1[0x64 - 0x5C];
    O *f64;
    int f68;
    char pad2[0x70 - 0x6C];
    int f70;
    int f74;
    char pad3[0x7C - 0x78];
    O *f7c;
    char pad4[0xA8 - 0x80];
    int fa8, fac, fb0, fb4;
    char pad5[0xF4 - 0xB8];
    int ff4;
};
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct CLOCK { float t; };
extern CLOCK D_002DDED0;
extern DL D_003E0F90;
extern char D_002DEB30[];
extern int D_002DEB44;
extern "C" int FTaskLive(O *p);
extern "C" void BroadcastGameEvent(void *pv, int n, O *p);
extern "C" void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
extern "C" void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
extern "C" void ApplyStateLoVisibility(O *p, int n, int c, int a);
extern "C" void TriggerStateRsmgs(O *p, int n, int c, int a, int b);
extern "C" void UpdateTaskSaveVars(O *p);
extern "C" void ActivateTask(O *p);
extern "C" void DeactivateTask(O *p);
/** @sly2-annot addr=0x1657D0
 * Purpose: Low-level task state set: events 0x26/0x25, maintains the available-task list g_sw+0x3B00, counts,
 *   save vars, SendMessage 0x16/0x15, follow-up activate/deactivate
 * Info: proposed name SetTaskState (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void SetTaskState(O *p, int n)
{
    if (p->n54 == n)
        return;
    if (FTaskLive(p)) {
        BroadcastGameEvent(D_002DEB30, 0x26, p);
        p->Msg(0x16, 0, 0, 0, 0);
    }
    if (p->n54 == 1)
        RemoveDlEntry__FP2DLPv(&D_003E0F90, p);
    if (FTaskLive(p)) {
        ApplyStateLoVisibility(p, n, p->fa8, p->fac);
        TriggerStateRsmgs(p, n, p->fb0, p->fb4, p->ff4);
    }
    switch (n) {
    case 0:
        if (p->ff4 == 4) {
            p->f64 = 0;
            p->f68 = 0;
            if (p->f7c && p->f7c->f64 == p)
                p->f7c->f7c = 0;
        }
        break;
    case 1:
        AppendDlEntry__FP2DLPv(&D_003E0F90, p);
        break;
    case 2:
        if (p->ff4 == 0)
            p->f68++;
        break;
    }
    p->n54 = n;
    p->f58 = D_002DDED0.t;
    UpdateTaskSaveVars(p);
    if (FTaskLive(p)) {
        BroadcastGameEvent(D_002DEB30, 0x25, p);
        p->Msg(0x15, 0, 0, 0, 0);
        p->V4(0x11, p);
    }
    switch (p->ff4) {
    case 1:
        if (p->n54 == 1 && p->f74)
            D_002DEB44 = p->f18;
        break;
    case 0:
    case 2:
    case 4:
        switch (p->n54) {
        case 1:
            if (p->f70)
                ActivateTask(p);
            break;
        case 0:
        case 3:
            DeactivateTask(p);
            break;
        }
        break;
    case 3:
        break;
    }
}
