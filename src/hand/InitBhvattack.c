struct S { char pad[0x10]; void *p10; char pad2[0x20]; int n34; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
void InitBhvattack(struct S *p) { InitBhv(p); AddStepguardSgf(p->p10, 0x18); p->n34 = -1; }
