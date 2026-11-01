struct S { char pad[0x38]; float g; char p2[0x154]; void *p190; };
extern float D_002BE8B4;
void PostLoLoad(struct S *p);
void func_00221268(void *p, int n);
float func_001E7D90(struct S *p);
void func_00182F28(struct S *p)
{
    PostLoLoad(p);
    if (p->p190) func_00221268(p->p190, 0);
    if (p->g == 1e10f) p->g = func_001E7D90(p) + D_002BE8B4;
}
