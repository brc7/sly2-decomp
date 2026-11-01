struct VTE { short d; short pad; void (*pfn)(); };
struct S { int a, b, c; struct VTE *vt; };
void PostSoLoad(struct S *p);
void func_001D7268(struct S *p) {
    struct VTE *e;
    PostSoLoad(p);
    e = &p->vt[79];
    ((void (*)(void *, int))e->pfn)((char *)p + e->d, 0);
}
