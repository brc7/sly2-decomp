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
void SetNoticeState(OBJ *, int);
int FWidgetVisible(W *);
int FWidgetAnimPlaying(W *, int);
/** @sly2-annot addr=0x1DA350
 * Purpose: Notice banner per-frame: showing -> shown when widget state 2; time out (only while widget hidden)
 *   or swap to the queued text (anim 0x312); hide
 * Info: proposed name UpdateNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateNotice(OBJ *p)
{
    if (p->pw == 0)
        return;
    switch (p->state) {
    case 0:
        break;
    case 1:
        if (p->pw->x58 == 2)
            SetNoticeState(p, 2);
        break;
    case 2:
        if (p->x20 != -1) {
            SetNoticeState(p, 4);
        } else if (p->x1C == 0.0f) {
            SetNoticeState(p, 3);
        } else if (0.0f < p->x1C) {
            if (p->pw && !FWidgetVisible(p->pw))
                p->t += D_002DDED0.dt;
        }
        break;
    case 3:
        if (p->pw->x58 < 2)
            SetNoticeState(p, 0);
        break;
    case 4:
        if (FWidgetAnimPlaying(p->pw, 0x312))
            break;
        if (p->x20 == -1)
            SetNoticeState(p, 0);
        else
            SetNoticeState(p, 2);
        break;
    }
}
