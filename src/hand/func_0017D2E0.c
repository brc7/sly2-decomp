struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct S { char pad[0x10]; void *p10; };
void StartBhv(struct S *p);
struct O *func_00205438(void *p, int n);
void func_0017D2E0(struct S *p) { struct O *o; struct VTE *e; StartBhv(p); o = func_00205438(p->p10, 0x9D); o->n3C = 2; e = &o->vt[8]; e->pfn((char *)o + e->d); }
