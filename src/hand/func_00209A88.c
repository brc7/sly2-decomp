struct CRef { int tagk; int tag; };
struct S { char pad[0x2C0]; long n; };
void *CsgfFromGrf(long n, char *ab);
void func_00200F58(struct CRef *ret, void *p, int n, char *ab);
struct CRef *func_00209A88(struct CRef *ret, struct S *p) { char ab[0xC0]; func_00200F58(ret, CsgfFromGrf(p->n, ab), 0x1010, ab); return ret; }
