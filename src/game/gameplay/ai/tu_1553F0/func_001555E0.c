struct T { char pad[0x33C]; int n33C; char pad2[0x1018 - 0x340]; int n1018; };
struct S { int pad[4]; struct T *pt; int pad2[9]; int n38; int f3C; };
void StartBhv(struct S *p);
void SetStepguardTarget(struct T *pt, int n);
int func_00155868(struct S *p);
void func_00155788(struct S *p, int f);
/** @sly2-annot addr=0x1555E0
 * Purpose: BHV180 slot 8: picks a route after enough frames and starts fleeing (state 0) or watching (state
 *   1)
 * Info: proposed name StartBhv180 (T2); class BHV180 slot8; TU tu_1553F0
 * Notes: superseded: OnBhv180Resume (r3b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv180Resume
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_001555E0(struct S *p)
{
    StartBhv(p);
    SetStepguardTarget(p->pt, p->n38);
    if (p->pt->n1018 - p->pt->n33C >= 90)
        p->f3C = func_00155868(p);
    if (p->f3C)
        func_00155788(p, 0);
    else
        func_00155788(p, 1);
}
