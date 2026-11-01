struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void InitBhv(struct S *p);
void func_00209890(void *p, int n);
void func_0017BED0(struct S *p) { InitBhv(p); func_00209890(p->p10, 0x19); p->n30 = -1; }
