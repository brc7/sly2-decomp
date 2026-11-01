struct VTE { short d; short pad; void *pfn; };
struct S { int x0, x4, x8; struct VTE *vt; void *x10; int x14; char pad[0x10]; unsigned long x28; };
extern char D_003DD4BC[], D_003DD490[];
int func_001A61A0(struct S *s);
void RemoveDlEntry__FP2DLPv(void *a, void *b);
void func_00147E20(void *a, int b);
void func_001A5448(void *p, void **a, struct S ***b);
void AppendDlEntry__FP2DLPv(void *a, struct S *s);
void func_001A5630(struct S *s)
{
    struct VTE *e;
    unsigned long f;
    void *a;
    struct S **b;
    struct S **list;
    int n, i;
    if (s->x28 & 0x0100000000000000UL)
        return;
    if (func_001A61A0(s)) {
        e = &s->vt[0xB0 / 8];
        ((void (*)(void *))e->pfn)((char *)s + e->d);
        if (!s->x10)
            RemoveDlEntry__FP2DLPv(D_003DD4BC, (void *)s->x14);
    }
    if (!s->x10)
        func_00147E20(D_003DD490, s->x14);
    f = (s->x28 >> 60) & 1;
    func_001A5448(s->x10, &a, &b);
    RemoveDlEntry__FP2DLPv(a, s);
    AppendDlEntry__FP2DLPv(a, s);
    if (!*b)
        *b = s;
    s->x28 |= 0x0100000000000000UL;
    e = &s->vt[0xE0 / 8];
    n = ((int (*)(void *, struct S ***))e->pfn)((char *)s + e->d, &list);
    for (i = 0; i < n; i++) {
        struct S *p = list[i];
        struct VTE *g = &p->vt[0x48 / 8];
        ((void (*)(void *))g->pfn)((char *)p + g->d);
    }
    if (f) {
        e = &s->vt[0x60 / 8];
        ((void (*)(void *))e->pfn)((char *)s + e->d);
    }
}
