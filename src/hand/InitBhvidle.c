struct S { char pad[0x3C]; int n3C; };
void InitBhvanim(struct S *p);
void InitBhvidle(struct S *p) { InitBhvanim(p); p->n3C = 2; }
