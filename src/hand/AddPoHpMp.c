struct VTE { short d; short pad; int (*pfn)(void *, int, int, int, void **, int); };
struct O { int a[3]; struct VTE *vt; };
void AddPoHpMp(struct O *p, int a, int b)
{
    void *apv[2];
    int n0, n1;
    /*B*/n0 = a; n1 = b; apv[0] = &n0; apv[1] = &n1;/*E*/
    { struct VTE *e = &p->vt[3]; e->pfn((char *)p + e->d, 0xC, 0, 2, apv, 0); }
}
