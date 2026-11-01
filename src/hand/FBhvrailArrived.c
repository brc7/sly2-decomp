struct T { char pad[0xF88]; float gF88; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; float g3C; };
int FBhvrailArrived(struct S *p) { return __builtin_fabsf(p->pt->gF88 - p->g3C) < 50.0f; }
