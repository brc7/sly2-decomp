struct F { int a[0x1C]; };
void StartBhv(void *p);
void func_002333B4(void *p, int c, int n);
void func_00176750(void *p, struct F *pf);
void func_00176708(void *p) { struct F f; StartBhv(p); func_002333B4(&f, 0, sizeof(f)); f.a[0] = 0; func_00176750(p, &f); }
