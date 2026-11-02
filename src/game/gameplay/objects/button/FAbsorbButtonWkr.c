struct Q { int x0; char pad[0x40]; int x44; };
struct P { char pad[0x1E0]; int x1E0; float x1E4; float x1E8; char pad2[0x2A8 - 0x1EC]; int x2A8; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void *func_001F7588(struct P *p, struct Q *q);
int FCheckButtonObject(struct P *p, int x);
void SetButtonButtons(struct P *p, int f);
/** @sly2-annot addr=0x148F20
 * Purpose: vt75: zap/hit by an allowed object presses the button (by btnt mode)
 * Info: proposed name FAbsorbButtonWkr (T1); class BUTTON slot75; TU button
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void *FAbsorbButtonWkr(struct P *p, struct Q *q)
{
    void *r = func_001F7588(p, q);
    if (r && !(q->x44 & 4) && FCheckButtonObject(p, q->x0)) {
        switch (p->x2A8) {
        case 1:
            if (p->x1E0 == 0) SetButtonButtons(p, 1);
            break;
        case 2:
            if (p->x1E0 == 0) {
                if (p->x1E8 <= D_002DDED0.t - p->x1E4) SetButtonButtons(p, 1);
            } else if (p->x1E0 == 1) {
                SetButtonButtons(p, 0);
            }
            break;
        }
    }
    return r;
}
