struct T { char pad[0x33C]; int n33C; char pad2[0x1018 - 0x340]; int n1018; };
struct S { int pad[4]; struct T *pt; int pad2[9]; int n38; int f3C; };
void StartBhv(struct S *p);
void func_00208540(struct T *pt, int n);
int func_00155868(struct S *p);
void func_00155788(struct S *p, int f);
void func_001555E0(struct S *p)
{
    StartBhv(p);
    func_00208540(p->pt, p->n38);
    if (p->pt->n1018 - p->pt->n33C >= 90)
        p->f3C = func_00155868(p);
    if (p->f3C)
        func_00155788(p, 0);
    else
        func_00155788(p, 1);
}
