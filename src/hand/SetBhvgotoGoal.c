typedef int qword __attribute__((mode(TI)));
struct Q2 { qword a, b; };
struct S { char pad[0x40]; struct Q2 q; char pad2[0x40]; float gA0; };
void PlanBhvgoto(struct S *p);
void SetBhvgotoGoal(struct S *p, struct Q2 *pq) { p->q = *pq; p->gA0 = -3.40282346638528859812e+38f; PlanBhvgoto(p); }
