struct S { char pad[0xC0]; int nC0; float gC4; char pad2[0x18]; float gE0; };
extern float D_002BE694;
void InitBhvpath(struct S *p);
/** @sly2-annot addr=0x178E98
 * Purpose: BHVWALKPATH Init: BHVPATH Init, anim flag 9 (walk), rate 1
 * Info: proposed name InitBhvwalkpath (T1); class BHVWALKPATH slot7; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   InvalidateAloLighting (names_v2:B(unverified)) | runtime2/4 confirmed the BHVWALKPATH role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#3-the-behaviour-stack-and-the-guard-modes,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvwalkpath(struct S *p) { InitBhvpath(p); p->gE0 = D_002BE694; p->gC4 = 1.0f; p->nC0 = 9; }
