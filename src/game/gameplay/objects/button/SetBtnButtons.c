struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct P { char pad[0x90]; int state; float tState; };
void TriggerBtn(struct P *p);
void UntriggerBtn(struct P *p);
/** @sly2-annot addr=0x148A78
 * Purpose: BTN state change; 0->1 triggers, any->0 untriggers; stamps time
 * Info: proposed name SetBtnButtons (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void SetBtnButtons(struct P *p, int state)
{
    int f1, f2; int old = p->state; if (old == state) return; f1 = old == 0 && state == 1; f2 = old != -1 && state == 0;
    p->state = state;
    p->tState = D_002DDED0.t;
    if (f1) TriggerBtn(p);
    if (f2) UntriggerBtn(p);
}
