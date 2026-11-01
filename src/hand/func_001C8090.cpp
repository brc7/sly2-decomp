struct X { int c; short *ps; };
struct B { int a, b; int c; short Find(char *s) ; };
extern B D_003E0EA8;
extern "C" {
int IlevelCur__Fv(void);
void *func_002109B8(int n);
int func_001D8CC8(void *p, X *px);
int func_001C8090(void);
short func_001D8720(int *p, char *s);
}
inline short B::Find(char *s) { return func_001D8720(&c, s); }
int func_001C8090(void)
{
    int n = IlevelCur__Fv();
    X x;
    short s;
    if (n == -1) return 0;
    s = D_003E0EA8.Find("fVault");
    x.c = 1;
    x.ps = &s;
    return func_001D8CC8(func_002109B8(n), &x);
}
