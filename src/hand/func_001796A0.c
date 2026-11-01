struct S { char pad[0x3C]; int n3C; };
void InitBhvanim(struct S *p);
void func_001796A0(struct S *p) { InitBhvanim(p); p->n3C = 5; }
