struct T { char pad[0xF84]; void *pF84; float gF88; int nF8C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; float g3C; };
float SMaxShape(void *p);
float SMinShape(void *p);
void SetBhvrailState(struct S *p, int n);
/** @sly2-annot addr=0x17CDB8
 * Purpose: Goal = rail max or min parameter; set direction and walk
 * Info: proposed name SetBhvrailGoal (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloRotationSpring (names_v2:B(unverified))
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvrailGoal(struct S *p, int f) {
    void *pv = p->pt->pF84; if (f) p->g3C = SMaxShape(pv); else p->g3C = SMinShape(pv);
    p->pt->nF8C = p->pt->gF88 < p->g3C;
    SetBhvrailState(p, 0);
}
