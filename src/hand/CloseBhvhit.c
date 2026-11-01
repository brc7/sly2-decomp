typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct G { int a; };
extern struct G D_003DD490;
struct S { char pad[0x10]; char *p10; };
void CloseBhvanim(struct S *p);
void func_00156C80(struct G *g, int a, qword q, int b, void *pv);
void CloseBhvhit(struct S *p) { CloseBhvanim(p); func_00156C80(&D_003DD490, -1, D_002A2DD0, 0, p->p10 + 0x35C); }
