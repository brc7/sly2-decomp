struct S { char pad[0x298]; int a; char pad2[0x90]; int b; };
int FStepguardHasSgf(struct S *p, int n);
int FInvulnerableStepguard(struct S *p) { if (p->b || p->a) return 1; return FStepguardHasSgf(p, 0x2A) != 0; }
