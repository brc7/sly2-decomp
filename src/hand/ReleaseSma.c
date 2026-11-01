struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a[3]; struct VTE *vt; };
struct S { struct O *po; };
void func_001F0598(struct O *);
void ReleaseSma(struct S *p)
{
    if (p->po) {
        func_001F0598(p->po);
        { struct O *po = p->po; po->vt[2].pfn((char *)po + po->vt[2].d); }
        p->po = 0;
    }
}
