struct T { char pad[0xF3C]; int nF3C; };
struct S { char pad[0x10]; struct T *pt; };
void CloseBhvanim(struct S *p);
void CloseBhvstrike(struct S *p) { CloseBhvanim(p); p->pt->nF3C = 0; }
