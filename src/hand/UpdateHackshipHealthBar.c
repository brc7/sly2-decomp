struct S { char pad[0x180]; int c; int n; char p1[0x118]; void *p2a0; };
void func_00212E40(void *p, int n, float g);
void UpdateHackshipHealthBar(struct S *p)
{
    if (p->p2a0) {
        int n = p->n;
        float g;
        if (n <= 0) n = 0;
        g = 1.0f - (float)n / (float)p->c;
        if (g < 0.0f) g = 0.0f;
        else if (g > 1.0f) g = 1.0f;
        func_00212E40(p->p2a0, 0x34F, g);
    }
}
