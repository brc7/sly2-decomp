struct T { char pad[0xAE]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
void func_001F8830(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
