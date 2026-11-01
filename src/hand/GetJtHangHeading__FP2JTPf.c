struct VTE { short d; short pad; float *(*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct S { char pad[0x18C4]; struct O *o; };
float func_002458F0(float, float);
void GetJtHangHeading__FP2JTPf(struct S *p, float *pg) {
    float *a, *b;
    { struct O *o = p->o; struct VTE *e = &o->vt[31]; a = e->pfn((char *)o + e->d); }
    { struct O *o = p->o; struct VTE *e = &o->vt[31]; b = e->pfn((char *)o + e->d); }
    *pg = func_002458F0(a[1], b[0]);
}
