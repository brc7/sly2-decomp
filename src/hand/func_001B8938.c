struct S { char pad[0x1030]; void *p; char pad2[0x190C - 0x1034]; void *q; };
extern char D_002C09F8[];
void func_00131B60(void *a, struct S *p, int n, void *s);
void SetMurrayState(struct S *p, int n);
void func_001B8938(struct S *p)
{
    func_00131B60(p->p, p, 5, D_002C09F8);
    p->q = p->p;
    p->p = 0;
    SetMurrayState(p, 0);
}
