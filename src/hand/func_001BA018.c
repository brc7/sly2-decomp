typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x30]; qword q30; char pad1[0xA0 - 0x40]; int nA0; };
struct L { char pad[0x30]; qword q30; };
struct S { char pad[0x1020]; void *p1020; char pad1[0x1030 - 0x1024]; void *p1030; struct T *p1034;
           char pad2[0x1040 - 0x1038]; qword q1040; char pad3[0x1060 - 0x1050]; int n1060; int pad4[2]; int n106C; };
extern char D_002C0A50[];
void func_002061D8(struct S *p);
void BindLoOidTable(struct S *p, int n, void *pv);
float *func_0012F938(void *p, int n);
void func_001300B8(void *p, struct S *ps, void *p1, int n, struct L *pl, int m, float a, float b);
void SetSoMass(struct S *p, float g);
void func_001BA018(struct S *p)
{
    struct L l;
    qword a, b, d;
    float *pg;
    func_002061D8(p);
    BindLoOidTable(p, 3, D_002C0A50);
    pg = func_0012F938(p->p1020, 0x5DB);
    func_001300B8(p->p1020, p, p->p1030, p->p1034->nA0, &l, 0, *pg, 1.0f);
    a = l.q30;
    b = p->p1034->q30;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    p->n1060 = 0;
    p->q1040 = d;
    p->n106C = 1;
    SetSoMass(p, 0.25f);
}
