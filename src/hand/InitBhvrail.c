struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
void InitBhvrail(struct S *p) { InitBhv(p); p->n30 = -1; p->g38 = 1.0f; AddStepguardSgf(p->p10, 0x1B); }
