struct T { char pad[0x3CC]; void *p3CC; int pad2; int n3D4; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; int n40; };
int func_0020A7E0(void *p);
/** @sly2-annot addr=0x176060
 * Purpose: Target lost: not hit-confirmed and the shared target record is stale (FTargetRecordStale)
 * Info: proposed name FBhvcombatLostTarget (T1); TU guardbhv
 * Notes: names_v2 C hint not used: FIsZeroV__FP6VECTOR | runtime2/4 confirmed the BHVCOMBAT role on live
 *   guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvcombatLostTarget(struct S *p) { int f; if (p->n40) f = 0; else if (p->pt->n3D4) f = 0; else f = func_0020A7E0(p->pt->p3CC); return f; }
