struct T { char pad[0xF84]; void *pF84; float gF88; int nF8C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; float g38; };
float SMaxShape(void *p);
float SMinShape(void *p);
void SetBhvpipeState(struct S *p, int n);
/** @sly2-annot addr=0x17D110
 * Purpose: Goal = pipe max or min parameter
 * Info: proposed name SetBhvpipeGoal (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloPositionSmoothDetail (names_v2:B(unverified))
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvpipeGoal(struct S *p, int f) {
    void *pv = p->pt->pF84; if (f) p->g38 = SMaxShape(pv); else p->g38 = SMinShape(pv);
    p->pt->nF8C = p->pt->gF88 < p->g38;
    SetBhvpipeState(p, 0);
}
