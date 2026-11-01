struct VTE { short d; short pad; void (*pf)(void *); };
struct O { int a[3]; struct VTE *vt; char pad[0x14]; struct O *next; };
struct S { char pad[0x64]; struct O *first; int a[2]; struct O *end; };
void UpdateSoXfWorldMatrices(struct S *p);
void UpdateSoXfWorldHierarchy(struct S *p)
{
    struct O *o;
    UpdateSoXfWorldMatrices(p);
    for (o = p->first; o != p->end; o = o->next)
        o->vt[18].pf((char *)o + o->vt[18].d);
}
