struct VTE { short d; short pad; void (*pfn)(void *); };
struct S { char pad[0xC]; struct VTE *vt; };
void PostSoLoad(struct S *p);
void func_0017F138(struct S *p) { struct VTE *e; PostSoLoad(p); e = &p->vt[0x53]; e->pfn((char *)p + e->d); }
