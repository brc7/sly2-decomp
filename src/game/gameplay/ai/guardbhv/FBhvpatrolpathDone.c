struct S { char pad[0x18]; void *p18; };
int FBhvpatrolpathAtEnd(struct S *p);
int FBhvgotoArrived(void *p);
/** @sly2-annot addr=0x17EE78
 * Purpose: At the end of an open shape and arrived
 * Info: proposed name FBhvpatrolpathDone (T1); TU guardbhv
 * Notes: names_v2 v1 hint not used: FUN_001c8d40 | runtime2/4 confirmed the BHVPATROLPATH role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvpatrolpathDone(struct S *p) { if (!FBhvpatrolpathAtEnd(p)) return 0; return FBhvgotoArrived(p->p18); }
