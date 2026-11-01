struct T { char pad[0x28]; unsigned long fl; };
struct U { char pad[0xF68]; struct T *pt; };
struct S { char pad[0x10]; struct U *pu; };
int FBhvguardEscortActive(struct S *p) { struct T *t = p->pu->pt; int f = 0; if (t) f = (int)(t->fl >> 60) & 1; return f; }
