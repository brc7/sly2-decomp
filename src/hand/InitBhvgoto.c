struct T { char pad[0x28]; int n28; };
struct G { char pad[0x45CC]; struct T *p45CC; };
extern struct G D_003DD490;
struct S { char pad[0x30]; int n30; int pad2; int n38; char pad3[0x64]; float gA0; int nA4; };
void InitBhv(struct S *p);
/** @sly2-annot addr=0x17D360
 * Purpose: BHVGOTO Init: flag param +0x38=-1, state -1, path-graph version stamp
 * Info: proposed name InitBhvgoto (T1); class BHVGOTO slot7; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloRestoreRotationAck (names_v2:B(unverified)) | runtime2/4 confirmed the BHVGOTO role on live guards
 *   (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#3-the-behaviour-stack-and-the-guard-modes,
 *   docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvgoto(struct S *p) { InitBhv(p); p->n30 = -1; p->n38 = -1; p->gA0 = -3.40282346638528859812e+38f; p->nA4 = D_003DD490.p45CC->n28; }
