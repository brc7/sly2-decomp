struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; int pad4; float g40; };
void InitBhv(struct S *p);
void func_00209890(void *p, int n);
void func_00171F58(struct S *p) { InitBhv(p); func_00209890(p->p10, 0x1F); p->n30 = -1; p->g40 = p->g38 = 3.40282346638528859812e+38f; }
