int FLeapDone(char *p);
void SetLeapState(char *p, int n);
void func_001305B8(int a, int b, int c, void *pv, float g, float h);
void func_0020C080(char *p) { if (!FLeapDone(p)) { SetLeapState(p, 4); func_001305B8(0, 0, 0, p + 0x38, 0.0f, 1.0f); } }
