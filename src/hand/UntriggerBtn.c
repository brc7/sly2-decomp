struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void RunBtnAsegs(void *p, int n);
void UntriggerBtn(struct O **p)
{
    struct O *o = *p;
    struct VTE *e = &o->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)o + e->d, 6, 0, 0, 0, 0);
    RunBtnAsegs(p, 1);
    {
        struct O *o2 = *p;
        struct VTE *e2 = &o2->vt[4];
        ((void (*)(void *, int, void *))e2->pfn)((char *)o2 + e2->d, 7, p);
    }
}
