typedef int qword __attribute__((mode(TI)));
struct F { int a[4]; qword q; };
void *func_002333B4(void *, int, int);
void ReturnJtToSafePlace(void *, struct F *);
void func_0019F540(void *p, qword q) { struct F f; func_002333B4(&f, 0, sizeof f); f.q = q; ReturnJtToSafePlace(p, &f); }
