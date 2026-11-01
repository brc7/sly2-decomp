struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; };
void CloseBhv(struct S *p);
void func_0017E278(struct S *p, int n);
void func_00153FF8(void *p);
void SetStepguardTarget(struct T *t, int n);
void RemoveStepguardSgf(struct T *t, int n);
void func_0017DDE8(struct S *p) { CloseBhv(p); func_0017E278(p, -1); func_00153FF8(p->pt->pF64); SetStepguardTarget(p->pt, 0); RemoveStepguardSgf(p->pt, 0x2B); }
