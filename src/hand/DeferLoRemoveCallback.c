struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
void DeferLoRemoveCallback(struct O *p) { struct VTE *e = &p->vt[9]; e->pfn((char *)p + e->d); }
