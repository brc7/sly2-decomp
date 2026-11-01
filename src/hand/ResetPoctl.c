struct S { int a[4]; float g10, g14, g18, g1c, g20, g24, g28, g2c; int n30; };
void ClearPoctlJoyPressed(struct S *p, int grf);
void ResetPoctl(struct S *p)
{
    p->n30 = 0;
    p->g20 = p->g24 = p->g28 = p->g2c = p->g10 = p->g14 = p->g18 = p->g1c = 0.0f;
    ClearPoctlJoyPressed(p, 0xFFFF);
}
