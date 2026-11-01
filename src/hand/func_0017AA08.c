typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x48]; int n5C; };
void func_00174950(struct S *p);
void func_0020AB68(struct T *t, int a, int b, qword q);
void func_0020B210(struct T *t);
void func_0017B1A8(struct S *p, int n);
void SetBhvBusy(struct S *p);
void func_00154038(void *p, int a, int b);
void func_0017AA08(struct S *p) { func_00174950(p); func_0020AB68(p->pt, 0, 0, D_002A2DD0); func_0020B210(p->pt); func_0017B1A8(p, 0); SetBhvBusy(p); func_00154038(p->pt->pF64, p->n5C, 2); }
