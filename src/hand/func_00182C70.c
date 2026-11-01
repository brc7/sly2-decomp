struct VTE { short d; short pad; void *(*pfn)(void *); };
struct S { char pad[8]; float g; char p1[8]; int x14; int p2; int x1c; struct VTE *vt; };
extern float D_002A5680;
void SetLoBusyk(void *p, int n);
void func_00182C70(struct S *p)
{
    struct VTE *e;
    p->x14 = -1;
    p->g = D_002A5680;
    p->x1c = 7;
    e = &p->vt[6];
    SetLoBusyk(e->pfn((char *)p + e->d), 3);
}
