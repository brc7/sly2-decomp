struct T { char pad[0xFE8]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; int pad2; void *p18; char pad3[0x14]; int a30[4]; };
void func_001EEF98(struct S *p);
int func_00174FB8(void *p);
int func_001C1FA0(void *pe, void *p, float g);
void func_0017E9B0(struct S *p);
/** @sly2-annot addr=0x17E918
 * Purpose: BHVRELOCATE Update: retry until within 25 of the point
 * Info: proposed name UpdateBhv191 (T2); class BHV191 slot10; TU guardbhv
 * Notes: names_v2 v1 hint not used: OnStepguardRemove__FP9STEPGUARD | BHV class word normalised:
 *   UpdateBhvrelocate -> UpdateBhv191
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E918(struct S *p) {
    func_001EEF98(p);
    if (p->pt->nFE8 & 1) {
        if (func_00174FB8(p->p18) && !func_001C1FA0(p->a30, p->pt, 25.0f)) func_0017E9B0(p);
    } else if (!func_001C1FA0(p->a30, p->pt, 25.0f)) func_0017E9B0(p);
}
