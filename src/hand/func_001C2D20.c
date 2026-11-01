struct S { char pad[0x168]; void *p168; int a16c; };
extern char D_002C0CB0[];
void PostSoLoad(struct S *p);
void BindLoOidTable(struct S *p, int n, void *q);
void ReplaceSma(void *a, struct S *p, int b, int c, int *pn);
void func_001C2D20(struct S *p)
{
    PostSoLoad(p);
    BindLoOidTable(p, 4, D_002C0CB0);
    ReplaceSma(p->p168, p, -1, 0, &p->a16c);
}
