typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x18]; ulong fl; };
void AddLoHierarchy(struct O *p)
{
    struct VTE *e;
    p->fl |= (ulong)1 << 60;
    e = &p->vt[4];
    e->pfn((char *)p + e->d, 0, p);
}
