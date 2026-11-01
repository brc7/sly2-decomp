struct S { char pad[0x38]; float g; char p2[0xA4]; void *pe0; char p3[0x28]; float g10c; };
extern float D_002BE8C0;
void PostLoLoad(struct S *p);
void func_00221268(void *p, int n);
void func_001833E0(struct S *p)
{
    PostLoLoad(p);
    if (p->pe0) func_00221268(p->pe0, 0);
    if (p->g == 1e10f) p->g = p->g10c + D_002BE8C0;
}
