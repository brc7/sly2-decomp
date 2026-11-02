struct T { char pad[0x58]; int n58; int n5C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; int n38; };
void CloseBhvanim(struct S *p);
void func_002303A0(struct T *t, int a, int b);
/** @sly2-annot addr=0x175090
 * Purpose: BHVIDLE Close: BHVANIM Close and restore XF roles
 * Info: proposed name CloseBhvidle (T1); class BHVIDLE slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVIDLE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvidle(struct S *p) { CloseBhvanim(p); func_002303A0(p->pt, p->pt->n58, 2); if (!p->n38) func_002303A0(p->pt, p->pt->n5C, 2); }
