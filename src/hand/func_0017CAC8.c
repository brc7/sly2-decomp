struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; };
void InitBhv(struct S *p);
void func_00209890(void *p, int n);
void func_0017CAC8(struct S *p) { InitBhv(p); p->n30 = -1; p->g38 = 1.0f; func_00209890(p->p10, 0x1B); }
