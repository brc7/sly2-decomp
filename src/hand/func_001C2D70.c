struct S { char pad[0x150]; char *p150; int x; void *p158; };
void RenderSoSelf(struct S *p);
void func_001A5EE8(void *p, void *q, float a, float b);
void func_001C2D70(struct S *p)
{
    RenderSoSelf(p);
    if (p->p158) func_001A5EE8(p->p158, p->p150 + 0x40, 1.0f, 1.0f);
}
