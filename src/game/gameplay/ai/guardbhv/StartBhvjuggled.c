typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x48]; int n5C; };
void StartBhvanim(struct S *p);
void func_0020AB68(struct T *t, int a, int b, qword q);
void func_0020B210(struct T *t);
void SetBhvjuggledState(struct S *p, int n);
void SetBhvBusy(struct S *p);
void SetDetectorTargetStateById(void *p, int a, int b);
/** @sly2-annot addr=0x17AA08
 * Purpose: BHVHELD Start: BHVANIM Start, cancel attack, detach, state 0 (attached to the holder's hold
 *   point), busy
 * Info: proposed name StartBhvjuggled (T1); class BHVJUGGLED slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvheld -> StartBhvjuggled | runtime2/4 confirmed the BHVJUGGLED
 *   role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvjuggled(struct S *p) { StartBhvanim(p); func_0020AB68(p->pt, 0, 0, D_002A2DD0); func_0020B210(p->pt); SetBhvjuggledState(p, 0); SetBhvBusy(p); SetDetectorTargetStateById(p->pt->pF64, p->n5C, 2); }
