struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct T { char pad[0x2D4]; float g2D4; float g2D8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x20]; float g34; };
float GRandInRange__Fff(float a, float b);
/** @sly2-annot addr=0x179EF8
 * Purpose: Arm the pause timer from +0x2D4..+0x2D8
 * Info: proposed name ArmBhvpatrolPauseTimer (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void ArmBhvpatrolPauseTimer(struct S *p) {
    if (p->g34 < 3.40282346638528859812e+38f) return;
    if (p->pt->g2D4 == 3.40282346638528859812e+38f) p->g34 = 3.40282346638528859812e+38f;
    else { float t = D_002DDED0.t; p->g34 = t + GRandInRange__Fff(p->pt->g2D4, p->pt->g2D8); }
}
