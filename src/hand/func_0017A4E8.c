typedef int qword __attribute__((mode(TI)));
struct G { int a; };
extern struct G D_003DD490;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; };
void StartBhvanim(struct S *p);
void func_0017A5B0(struct S *p, int n);
void func_00153FF8(void *p);
qword func_001A62E8(void *p);
void func_00156C80(struct G *g, int a, qword q, void *b, void *pv);
void func_0017A4E8(struct S *p) { StartBhvanim(p); func_0017A5B0(p, 0); func_00153FF8(p->pt->pF64); func_00156C80(&D_003DD490, 1, func_001A62E8(p->pt), p->pt, (char *)p->pt + 0x35C); }
