struct S { int pad[4]; int n10; int pad2[7]; int n30; };
void InitBhv(struct S *p);
void AddStepguardSgf(int n, int k);
/** @sly2-annot addr=0x155560
 * Purpose: BHV180 slot 7: base, guard flag 0x22, state -1
 * Info: proposed name InitBhv180 (T2); class BHV180 slot7; TU tu_1553F0
 * Notes: superseded: OnBhv180Push (r3b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv180Push
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_00155560(struct S *p)
{
    InitBhv(p);
    AddStepguardSgf(p->n10, 0x22);
    p->n30 = -1;
}
