struct T { char pad[0xFE8]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; int pad2; void *p18; char pad3[0x14]; int a30[4]; };
int FBhvanimDone(void *p);
int func_001C1FA0(void *pe, void *p, float g);
int func_0017EAE8(struct S *p) { if (p->pt->nFE8 & 1) { if (!FBhvanimDone(p->p18)) return 0; } return func_001C1FA0(p->a30, p->pt, 25.0f); }
