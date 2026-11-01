struct S { char pad[0x3C]; int n3C; };
void InitBhvanim(struct S *p);
/** @sly2-annot addr=0x1751C0
 * Purpose: BHVSTRIKE Init: BHVANIM Init with flag 3 (attacking; flag 3 broadcasts game event 0x14)
 * Info: proposed name InitBhvstrike (T1); class BHVSTRIKE slot7; TU guardbhv
 * Notes: runtime2: BHV159 Init sets flag 3 (observed S09) | runtime2/4 confirmed the BHVSTRIKE role on live
 *   guards (S02-S41)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime+verdicts_followups+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvstrike(struct S *p) { InitBhvanim(p); p->n3C = 3; }
