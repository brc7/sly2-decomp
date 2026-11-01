struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x1310 - 0x10]; char sub[4]; int n1314; };
void OnPoActive(void *p, int a);
void ReleaseAsega(void *p);
void OnStepplayerActive(struct O *p, int a)
{
    struct VTE *e;
    OnPoActive(p, a);
    e = &p->vt[105];
    ((void (*)(void *))e->pfn)((char *)p + e->d);
    if (!a) {
        ReleaseAsega(p->sub);
        p->n1314 = -1;
    }
}
