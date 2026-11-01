struct VTE { short d; short pad; void (*pf)(void *, int); };
struct S { int a[3]; struct VTE *vt; char pad[0x158]; char y[4]; };
void RemoveSoHierarchy(struct S *p);
void ReleaseSma(void *d);
void func_00224068(struct S *p)
{
    RemoveSoHierarchy(p);
    p->vt[76].pf((char *)p + p->vt[76].d, -1);
    ReleaseSma(p->y);
}
