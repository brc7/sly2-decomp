struct S { char pad[0x18]; void *p18; char pad2[0x18]; int n34; char pad3[8]; int n40; };
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int FBhvanimDone(void *p);
void SetBhvattackState(struct S *p, int n);
/** @sly2-annot addr=0x175600
 * Purpose: BHVATTACK Update: after the strike, go to follow-through if the victim confirmed the hit (+0x40),
 *   else finish
 * Info: proposed name UpdateBhvattack (T1); class BHVATTACK slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVATTACK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvattack(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        if (FBhvBusyAbove(p)) return;
        s = p->n34;
        switch (s) {
        case 0:
            if (p->n40) SetBhvattackState(p, 1);
            else if (FBhvanimDone(p->p18)) SetBhvattackState(p, -1);
            break;
        case 1:
            if (FBhvanimDone(p->p18)) SetBhvattackState(p, -1);
            break;
        }
    } while (s != p->n34);
}
