struct S { int a; int b; int c; void *vt; char pad[0x6C]; void *p7c; int n80; };
extern char D_002CA240[];
struct SL;
extern struct SL D_003E133C;
void ResetPoctl(struct S *p);
void func_0011A070(struct SL *psl, void *pv);
struct S *POCTL_ctor(struct S *p, void *q)
{
    p->a = 0;
    p->b = 0;
    p->vt = D_002CA240;
    ResetPoctl(p);
    p->p7c = q;
    p->n80 = -1;
    func_0011A070(&D_003E133C, p);
    return p;
}
