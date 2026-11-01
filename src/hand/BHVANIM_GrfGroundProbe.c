struct U { char pad[0x10]; int n10; };
struct T { int n0; int b; struct U *p8; };
struct S { char pad[0x40]; struct T *p40; };
int func_001EEFA0(struct S *p);
/** @sly2-annot addr=0x174EA0
 * Purpose: BHVANIM ground-probe flags: when the binding drives a kind-2 payload use its mode (7 for modes
 *   0/3/4 else 1), else base
 * Info: proposed name BHVANIM_GrfGroundProbe (T1); class BHVANIM slot12; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41) | BHV slot 12 (_GrfGroundProbe):
 *   canonical form would be BHVLEAP_GrfGroundProbe | BHV slot 12 (_GrfGroundProbe): canonical form would be
 *   BHV175_GrfGroundProbe
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int BHVANIM_GrfGroundProbe(struct S *p) {
    if (p->p40->n0 == 2) {
        switch (p->p40->p8->n10) { case 0: case 3: case 4: return 7; default: return 1; }
    }
    return func_001EEFA0(p);
}
