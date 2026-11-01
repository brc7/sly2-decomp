struct E { int a[8]; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; char *p30; int n34; };
int func_001C1FA0(void *pe, void *p, float g);
/** @sly2-annot addr=0x17E5F0
 * Purpose: Within 25 of the current segment endpoint
 * Info: proposed name FBhvpathlinkAtSegmentEnd (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvpathlinkAtSegmentEnd(struct S *p) { return func_001C1FA0(p->p30 + 0x60 + p->n34 * 32, p->p10, 25.0f); }
