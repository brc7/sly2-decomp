struct S { char pad[0x38]; float g; char p2[0xA4]; void *pe0; };
extern float D_002BE8C8;
void PostLoLoad(struct S *p);
void func_00221268(void *p, int n);
void func_00183768(struct S *p)
{
    PostLoLoad(p);
    if (p->pe0) func_00221268(p->pe0, 0);
    if (p->g == 1e10f) p->g = D_002BE8C8;
}
