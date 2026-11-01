struct F { int a0; int pad; int n8; int nC; int n10; int pad2[0x13]; };
struct S { char pad[0x10]; int n10; };
void *func_002333B4(void *p, int c, int n);
void func_001F7598(struct F *pf);
void InflictBhvguardZap(struct S *p, int a, int b) { struct F f; func_002333B4(&f, 0, sizeof(f)); f.a0 = b; f.n10 = a; f.nC = 0x40000; f.n8 = p->n10; func_001F7598(&f); }
