struct S8 { int a, b; };
struct N { int a; struct S8 s; int d; };
struct N *PvAllocSlotheapClear(void *p);
void func_0011A070(void *p, struct N *n);
void func_00149A50(char *p, int a, struct S8 s, int d)
{
    struct N *n = PvAllocSlotheapClear(p + 0x3C90);
    n->a = a;
    n->s = s;
    n->d = d;
    func_0011A070(p + 0x3CA8, n);
}
