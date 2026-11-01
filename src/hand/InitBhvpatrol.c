struct S { char pad[0x10]; void *p10; char pad2[0x1C]; float g30; float g34; int n38; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
void InitBhvpatrol(struct S *p) { InitBhv(p); p->n38 = -1; p->g30 = p->g34 = 3.40282346638528859812e+38f; AddStepguardSgf(p->p10, 0x1E); }
