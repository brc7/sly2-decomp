typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002E1E40;
extern qword D_002A2DD0;
struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; char pad1[0x38 - 0x10]; int x38; };
struct S { char pad[0x1018]; int x1018; float f101C; int x1020; struct O *x1024; int x1028; int x102C; int x1030; int x1034; };
#define VCALL0(o, n) ((void (*)())(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
void func_00130B18(struct O *o);
void ReleaseAct(int *p);
int func_00209A68(struct S *p, int n);
struct O *func_00205458(struct S *p, int n);
void func_00204D48(struct S *p, float f);
void func_0012F9B8(int a, struct S *p, int n, struct O **ppo, float f1, float f2);
int func_0012DC18(struct S *p, int a, int b, int c, int d, qword q);
void func_001BA408(struct S *p, int st)
{
    if (st == p->x1018) return;
    if (p->x1018 == 1) {
        func_00130B18(p->x1024);
        VCALL0(p->x1024, 2);
        p->x1024 = 0;
        ReleaseAct(&p->x102C);
        p->x102C = 0;
        p->x1028 = 0;
        if (func_00209A68(p, 2)) func_00205458(p, 0x9E)->x38 = D_002E1E40;
        func_00204D48(p, 1.0f);
    }
    p->x1018 = st;
    p->f101C = D_002DDED0.t;
    if (st != 1) return;
    func_0012F9B8(p->x1020, p, 1, &p->x1024, 0.0f, 1.0f);
    VCALL0(p->x1024, 1);
    p->x102C = func_0012DC18(p, p->x1034, 5, 8, 3, D_002A2DD0);
    if (func_00209A68(p, 2)) {
        func_00205458(p, 0x9E)->x38 = p->x1028;
        func_00204D48(p, 4.0f);
    }
}
