struct S { char pad[0x10]; void *p10; char pad2[0x20]; int n34; };
void InitBhv(struct S *p);
void func_00209890(void *p, int n);
void func_001753D8(struct S *p) { InitBhv(p); func_00209890(p->p10, 0x18); p->n34 = -1; }
