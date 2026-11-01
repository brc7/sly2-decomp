typedef int qword __attribute__((mode(TI)));
struct T { int a, b; void *p8; };
struct S { char pad[0x38]; void *p38; int pad2; struct T *p40; char pad3[0xC]; int n50; };
void UpdateBhvanim(struct S *p);
qword func_001A62E8(void *p);
void func_0020C6F8(void *p, qword q);
void UpdateBhvstrike(struct S *p) { UpdateBhvanim(p); if (!p->p40 || !p->p40->p8) p->n50 = 0; if (p->n50) { struct T *t = p->p40; func_0020C6F8(t->p8, func_001A62E8(p->p38)); } }
