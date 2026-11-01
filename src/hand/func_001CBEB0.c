struct S { char pad[0x4A4]; int f; };
void InitStepguard(struct S *p);
void func_001CBEB0(struct S *p) { InitStepguard(p); p->f = 1; }
