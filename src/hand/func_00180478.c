struct T { char pad[0x60]; void *p60; };
struct S { char pad[0xEC]; struct T *pt; char p1[0xD0]; float t1c0; float g1c4; int n; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
void RecalcSwOxfFilterForObject(void *g, struct S *p);
void func_00180478(struct S *p, int n)
{
    if (n != p->n) {
        p->n = n;
        if (p->pt->p60 == p) RecalcSwOxfFilterForObject(D_003DD490, p);
    }
    p->t1c0 = (p->g1c4 == 0.0f) ? 3.40282346638528859812e+38f : D_002DDED0.t + p->g1c4;
}
