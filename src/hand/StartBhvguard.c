struct F { int a[0x1C]; };
void StartBhv(void *p);
void func_002333B4(void *p, int c, int n);
void SetBhvguardMode(void *p, struct F *pf);
void StartBhvguard(void *p) { struct F f; StartBhv(p); func_002333B4(&f, 0, sizeof(f)); f.a[0] = 0; SetBhvguardMode(p, &f); }
