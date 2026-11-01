struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct VT { struct VTE e[8]; };
struct S { int x0, x4, x8; struct VT *vt; char pad[0x18]; unsigned long x28; };
extern char D_003DD490[];
void RemoveSwPlacesOf(void *g, struct S *s, int n);
void RemoveLoHierarchy(struct S *s)
{
    s->x28 &= ~0x1000000000000000UL;
    if (s->x28 & 0x0800000000000000UL)
        RemoveSwPlacesOf(D_003DD490, s, 0);
    s->vt->e[4].pfn((char *)s + s->vt->e[4].d, 1, s);
}
