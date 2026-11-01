unsigned long GrfFromSgf(int n);
struct S { char pad[0x2C0]; unsigned long m; };
int JthsCurrentStepguard(struct S *p) { if (p->m & GrfFromSgf(7)) return 2; if (p->m & (GrfFromSgf(5) | GrfFromSgf(6))) return 1; return 0; }
