struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
void PostStepLoad(struct O *o);
void func_00152578(struct O *o);
void func_00152100(struct O *o)
{
    struct VTE *e;
    PostStepLoad(o);
    func_00152578(o);
    e = &o->vt[79];
    e->pfn((char *)o + e->d, 0);
}
