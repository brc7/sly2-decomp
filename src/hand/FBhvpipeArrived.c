struct T { char pad[0xF88]; float gF88; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; float g38; };
int FBhvpipeArrived(struct S *p) { return __builtin_fabsf(p->pt->gF88 - p->g38) < 50.0f; }
