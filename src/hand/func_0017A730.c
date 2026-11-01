typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; int n3C; };
void StartBhv(struct S *p);
void func_0017A8C8(struct S *p, int n);
void func_0020B210(struct T *t);
void func_0020AB68(struct T *t, int a, int b, qword q);
void func_001F4E28(struct T *t);
void SetBhvBusy(struct S *p);
void func_00153FF8(void *p);
void SetDetectorTargetStateById(void *p, int a, int b);
void func_0017A730(struct S *p) {
    StartBhv(p); func_0017A8C8(p, 0); func_0020B210(p->pt); func_0020AB68(p->pt, 0, 0, D_002A2DD0);
    func_001F4E28(p->pt); SetBhvBusy(p); func_00153FF8(p->pt->pF64);
    if (p->n3C) SetDetectorTargetStateById(p->pt->pF64, p->n3C, 2);
}
