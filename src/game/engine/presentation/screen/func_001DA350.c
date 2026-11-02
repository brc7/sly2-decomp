extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct { char pad[0x58]; int x58; } W;
typedef struct {
    char pad[0x10];
    int state;
    float t;
    int x18;
    float x1C;
    int x20;
    float x24;
    W *pw;
} OBJ;
void func_001DA5C8(OBJ *, int);
int func_002132D0(W *);
int func_002121E8(W *, int);
/** @sly2-annot addr=0x1DA350
 * Purpose: Notice banner per-frame: showing -> shown when widget state 2; time out (only while widget hidden)
 *   or swap to the queued text (anim 0x312); hide
 * Info: proposed name UpdateNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA350(OBJ *p)
{
    if (p->pw == 0)
        return;
    switch (p->state) {
    case 0:
        break;
    case 1:
        if (p->pw->x58 == 2)
            func_001DA5C8(p, 2);
        break;
    case 2:
        if (p->x20 != -1) {
            func_001DA5C8(p, 4);
        } else if (p->x1C == 0.0f) {
            func_001DA5C8(p, 3);
        } else if (0.0f < p->x1C) {
            if (p->pw && !func_002132D0(p->pw))
                p->t += D_002DDED0.dt;
        }
        break;
    case 3:
        if (p->pw->x58 < 2)
            func_001DA5C8(p, 0);
        break;
    case 4:
        if (func_002121E8(p->pw, 0x312))
            break;
        if (p->x20 == -1)
            func_001DA5C8(p, 0);
        else
            func_001DA5C8(p, 2);
        break;
    }
}
