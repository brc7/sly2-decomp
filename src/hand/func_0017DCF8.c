struct S { char pad[0x18]; void *p18; char pad2[0x14]; int n30; };
int func_00178D80(void *p);
/** @sly2-annot addr=0x17DCF8
 * Purpose: Walking (states 1-2) and the walk path finished
 * Info: proposed name FBhvgotoPathFinished (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloLookAtPanFunction (names_v2:B(unverified)) | runtime2/4 confirmed the BHVGOTO role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int func_0017DCF8(struct S *p) { if (p->n30 >= 3) return 0; if (p->n30 <= 0) return 0; return func_00178D80(p->p18); }
