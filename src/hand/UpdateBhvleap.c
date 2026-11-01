struct U { char pad[0x10]; int n10; };
struct T { int a, b; struct U *p8; };
struct S { char pad[0x10]; char *p10; char pad1[0x1C]; int n30; char pad2[0xC]; struct T *p40; char pad3[0x28]; int n6C; int a70[4]; };
void UpdateBhvanim(struct S *p);
void func_00209840(void *p, void *q);
/** @sly2-annot addr=0x17A400
 * Purpose: BHVLEAP Update: BHVANIM update; after the jump phase lock heading to the attach basis
 * Info: proposed name UpdateBhvleap (T1); class BHVLEAP slot10; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloVelocityVec (names_v2:B(unverified)) | runtime2/4 confirmed the BHVLEAP role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvleap(struct S *p) { UpdateBhvanim(p); if (p->n6C && !p->n30 && p->p40->p8->n10 >= 2) func_00209840(p->p10 + 0xFD0, p->a70); }
