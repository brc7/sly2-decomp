struct T { char pad[0x3CC]; void *p3CC; int pad2; int n3D4; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; int n40; };
int func_0020A7E0(void *p);
int FBhvcombatLostTarget(struct S *p) { int f; if (p->n40) f = 0; else if (p->pt->n3D4) f = 0; else f = func_0020A7E0(p->pt->p3CC); return f; }
