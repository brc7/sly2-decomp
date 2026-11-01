struct A { char pad[0x184]; int n184; };
struct B { char pad[0xC]; struct A *pa; };
struct Q { struct A *pa; void *p; };
struct S { char pad[0x26C]; char x26C[0x49C - 0x26C]; struct B *pb; };
void func_001C3E20(struct S *p, int k, struct Q *q);
void func_0020D460(struct B *pb, struct A *pa);
void func_001EED78(void *p, int k, struct Q *q);
/** @sly2-annot addr=0x2075D0
 * Purpose: STEPGUARD vt5: HandlePoMessage; msgs 4/5 attach/detach an object on component +0x49C; forward
 *   every message to the BHV stack (0x1EED78)
 * Info: proposed name HandleStepguardMessage (T1); class STEPGUARD slot5; TU stepguard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002075D0(struct S *p, int k, struct Q *q)
{
    func_001C3E20(p, k, q);
    switch (k) {
    case 4:
        if (q->p == p && q->pa->n184)
            func_0020D460(p->pb, q->pa);
        break;
    case 5:
        if (q->p == p && q->pa == p->pb->pa)
            func_0020D460(p->pb, 0);
        break;
    }
    func_001EED78(p->x26C, k, q);
}
