struct F { int a[0x1C]; };
void *func_00205458(void *p, int n);
void *func_002333B4(void *p, int c, int n);
void SetBhvguardMode(void *p, struct F *pf);
void func_00171CF8(void *p, int n) { struct F f; void *o = func_00205458(p, 0x9C); if (o) { func_002333B4(&f, 0, sizeof(f)); f.a[4] = n; f.a[0] = 5; SetBhvguardMode(o, &f); } }
