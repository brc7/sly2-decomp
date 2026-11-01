
struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x58]; struct O *po; int n5C; };
void UnsubscribeLoObject(struct O *po, struct S *p);
void func_001E5410(struct S *p)
{
    if (p->po) {
        UnsubscribeLoObject(p->po, p);
        { struct O *po = p->po; po->vt[2].pf((char *)po + po->vt[2].d); }
        p->po = 0;
        p->n5C = 0;
    }
}
