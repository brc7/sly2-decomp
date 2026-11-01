struct S { char pad[0x30]; int n30; };
void InitBhv(struct S *p);
void InitBhvshoot(struct S *p) { InitBhv(p); p->n30 = -1; }
