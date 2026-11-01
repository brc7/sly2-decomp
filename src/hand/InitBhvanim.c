struct S { char pad[0x30]; int n30; int pad2[2]; int n3C; };
void InitBhv(struct S *p);
void InitBhvanim(struct S *p) { InitBhv(p); p->n3C = 0; p->n30 = -1; }
