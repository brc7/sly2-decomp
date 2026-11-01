struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
void func_00130B18(struct O *);
void ReleaseAsega(struct O **pp) {
    if (*pp) {
        struct O *p; struct VTE *vt;
        func_00130B18(*pp);
        p = *pp; vt = p->vt; ((void (*)(void *))vt[2].pfn)((char *)p + vt[2].d); *pp = 0;
    }
}
