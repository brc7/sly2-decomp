struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct T { char pad[0x2CC]; float g2CC; float g2D0; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x1C]; float g30; };
float GRandInRange__Fff(float a, float b);
/** @sly2-annot addr=0x179E80
 * Purpose: Arm the walk-duration timer from the guard range +0x2CC..+0x2D0 (FLT_MAX = forever)
 * Info: proposed name ArmBhvpatrolWalkTimer (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void ArmBhvpatrolWalkTimer(struct S *p) {
    if (p->g30 < 3.40282346638528859812e+38f) return;
    if (p->pt->g2CC == 3.40282346638528859812e+38f) p->g30 = 3.40282346638528859812e+38f;
    else { float t = D_002DDED0.t; p->g30 = t + GRandInRange__Fff(p->pt->g2CC, p->pt->g2D0); }
}
