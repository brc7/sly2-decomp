struct R { int id; int res; int x; };
struct E { int a; struct R ar[3]; };
struct S { char pad[0x58]; void *p58; void *p5C; char pad2[0x150 - 0x60]; void *p150; char pad3[0x2C4 - 0x154]; int c; struct E a[8]; };
int POCTL_FConsumePressed(void *p, int a);
int func_00152520(struct S *p, int id);
void func_002303A0(struct S *p, void *q, int n);
void ApplyDancerAseg(struct S *p, int a, int b);
void func_00152390(struct S *p, int id, int idNew, int j)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        if ((id == -1 && POCTL_FConsumePressed(p->p150, pe->a)) || pe->a == id) {
            if (idNew != -1) {
                int r = func_00152520(p, idNew);
                if (r)
                    pe->ar[0].res = r;
            }
            func_002303A0(p, p->p58, 2);
            func_002303A0(p, p->p5C, 2);
            ApplyDancerAseg(p, pe->ar[j].res, pe->ar[j].x);
            return;
        }
    }
}
