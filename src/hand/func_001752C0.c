struct T { char pad[0xF3C]; int nF3C; };
struct S { char pad[0x10]; struct T *pt; };
void func_00174B28(struct S *p);
/** @sly2-annot addr=0x1752C0
 * Purpose: BHVSTRIKE Close: BHVANIM Close and clear engaged-busy +0xF3C
 * Info: proposed name CloseBhvstrike (T1); class BHVSTRIKE slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVSTRIKE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001752C0(struct S *p) { func_00174B28(p); p->pt->nF3C = 0; }
