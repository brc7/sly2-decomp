struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void StartBhv(void *p);
void func_00146E18(void *p, int n);
void func_001466F0(char *p)
{
    struct O *o;
    struct VTE *e;
    StartBhv(p);
    func_00146E18(p, 0);
    o = *(struct O **)(p + 0x10);
    e = &o->vt[95];
    ((void (*)(void *, int, int, int, int))e->pfn)((char *)o + e->d, 4, *(int *)(p + 0x38), 4, *(int *)(p + 0x38));
    *(int *)(*(char **)(p + 0x10) + 0x10E0) |= 1;
}
