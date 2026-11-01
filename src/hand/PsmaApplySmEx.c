void ApplySmCore(int, int, int, int, int, int *);
int PsmaApplySmEx(int a, int b, int c, int d, int e) { int r; ApplySmCore(a, b, c, d, e, &r); return r; }
