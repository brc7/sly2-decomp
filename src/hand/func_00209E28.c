struct F { int a[0x1C]; };
void *func_00205458(void *p, int oid);
void *func_002333B4(void *pv, int c, int cb);
void SetBhvguardMode(void *p, struct F *pf);
void func_00209E28(void *p, int n) { void *o = func_00205458(p, 0x9C); struct F f; func_002333B4(&f, 0, sizeof f); f.a[0] = 6; f.a[4] = n; SetBhvguardMode(o, &f); }
