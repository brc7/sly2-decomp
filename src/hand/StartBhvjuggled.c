typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x48]; int n5C; };
void StartBhvanim(struct S *p);
void func_0020AB68(struct T *t, int a, int b, qword q);
void func_0020B210(struct T *t);
void SetBhvjuggledState(struct S *p, int n);
void SetBhvBusy(struct S *p);
void SetDetectorTargetStateById(void *p, int a, int b);
void StartBhvjuggled(struct S *p) { StartBhvanim(p); func_0020AB68(p->pt, 0, 0, D_002A2DD0); func_0020B210(p->pt); SetBhvjuggledState(p, 0); SetBhvBusy(p); SetDetectorTargetStateById(p->pt->pF64, p->n5C, 2); }
