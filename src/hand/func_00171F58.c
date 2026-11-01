struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; int pad4; float g40; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
/** @sly2-annot addr=0x171F58
 * Purpose: BHV171 slot 7: base, guard flag 0x1F, state -1, timers = FLT_MAX
 * Info: proposed name InitBhv171 (T2); class BHV171 slot7; TU tu_171F58
 * Notes: superseded: OnBhv171Push (r3b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv171Push
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_00171F58(struct S *p) { InitBhv(p); AddStepguardSgf(p->p10, 0x1F); p->n30 = -1; p->g40 = p->g38 = 3.40282346638528859812e+38f; }
