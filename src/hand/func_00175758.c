struct S { char pad[0x10]; void *p10; char pad2[0x20]; int n34; };
void func_001EEF68(struct S *p);
void func_00209890(void *p, int n);
/** @sly2-annot addr=0x175758
 * Purpose: BHVCOMBAT Init: raise flag 0x1D (engaged/alerted), state -1
 * Info: proposed name InitBhvcombat (T1); class BHVCOMBAT slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00175758(struct S *p) { func_001EEF68(p); func_00209890(p->p10, 0x1D); p->n34 = -1; }
