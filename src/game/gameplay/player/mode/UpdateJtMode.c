typedef int qword __attribute__((mode(TI)));
struct V { char pad[0x10]; float x10, x14, x18, x1c, x20, x24, x28, x2c; char pad2[0x80-0x30]; int x80; };
struct O { char pad[0x150]; struct V *x150; char pad2[0x190-0x154]; qword x190; };
struct P { struct O *x0; int x4; int pad[3]; void *x14; int pad2[2]; int x20; char x24[0x3C-0x24]; float x3c; };
struct D { char pad[0x460]; float x460; int p1[3]; float x470; char pad2[0x5C4-0x474]; float x5c4; int p2; float x5cc; };
extern struct D D_002DDF40;
extern qword D_002A2DD0;
int FAsegaRetracted(void *p);
void SetJtModeState__FP6JTMODEi(struct P *p, int n);
void func_00204660(struct O *o, float g);
void func_0019DA78(void *p);
/** @sly2-annot addr=0x19D330
 * Purpose: Per-frame mode state machine: 0 waits for start clip then 1, 1 turns toward look direction, 2
 *   waits for end clip then -1; updates HUD (0x19DA78)
 * Info: proposed name UpdateJtMode (T1); TU mode
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void UpdateJtMode(struct P *p)
{
    int n;
    do {
        n = p->x4;
        switch (n) {
        case 0:
            p->x0->x190 = D_002A2DD0;
            if (!p->x14 || FAsegaRetracted(p->x14))
                SetJtModeState__FP6JTMODEi(p, 1);
            break;
        case 1:
            p->x0->x190 = D_002A2DD0;
            switch (p->x20) {
            case 0:
                func_00204660(p->x0, D_002DDF40.x460);
                p->x3c = D_002DDF40.x470;
                func_0019DA78(p->x24);
                break;
            case 1:
                func_00204660(p->x0, D_002DDF40.x5c4);
                p->x3c = D_002DDF40.x5cc;
                func_0019DA78(p->x24);
                break;
            }
            break;
        case 2:
            p->x0->x190 = D_002A2DD0;
            if (!p->x14 || FAsegaRetracted(p->x14))
                SetJtModeState__FP6JTMODEi(p, -1);
            break;
        }
    } while (n != p->x4);
    if (p->x20 == 0) {
        struct V *v = p->x0->x150;
        if (v->x80 == 1) {
            v->x10 = v->x14 = v->x18 = v->x1c = 0.0f;
            v->x24 = v->x28 = v->x2c = 0.0f;
            v->x20 = 0.0f;
        }
    }
}
