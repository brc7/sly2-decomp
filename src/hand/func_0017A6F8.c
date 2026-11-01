struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
void func_0017A6F8(struct S *p) { InitBhv(p); AddStepguardSgf(p->p10, 0x16); p->n30 = -1; }
