struct VTE { short d; short pad; void *pfn; };
struct S {
    int x0, x4, x8; struct VTE *vt; struct S *x10; int x14; char pad[0xC]; struct S *x24;
    union { unsigned long u; struct { unsigned long f0 : 52; long st : 4; unsigned long f56 : 4; unsigned long f60 : 1; unsigned long f61 : 3; } b; } x28;
};
extern char D_003DD490[];
void func_001A5448(struct S *p, void **a, struct S ***b);
void RemoveDlEntry__FP2DLPv(void *a, struct S *s);
void InsertDlEntryBefore__FP2DLPvT1(void *a, struct S *b, struct S *s);
void func_00147E88(void *g, int n);
int func_001A61A0(struct S *s);
void func_00148300(void *g, struct S *s);
void func_001A5478(struct S *s)
{
    void *a;
    struct S **b;
    struct S **list;
    struct VTE *e;
    int n, i;
    if (!(s->x28.u & 0x0100000000000000UL))
        return;
    s->x28.u &= ~0x0100000000000000UL;
    func_001A5448(s->x10, &a, &b);
    if (s == *b) {
        *b = s->x24;
    } else {
        RemoveDlEntry__FP2DLPv(a, s);
        InsertDlEntryBefore__FP2DLPvT1(a, *b, s);
    }
    if (s->x10) {
        unsigned long t = (s->x10->x28.u >> 60) & 1;
        if (!t)
            goto skip;
    }
    e = &s->vt[0x58 / 8];
    ((void (*)(void *))e->pfn)((char *)s + e->d);
skip:
    if (!s->x10 && s->x28.b.st != 2)
        func_00147E88(D_003DD490, s->x14);
    e = &s->vt[0xE0 / 8];
    n = ((int (*)(void *, struct S ***))e->pfn)((char *)s + e->d, &list);
    for (i = 0; i < n; i++) {
        struct S *p = list[i];
        struct VTE *g = &p->vt[0x40 / 8];
        ((void (*)(void *))g->pfn)((char *)p + g->d);
    }
    if (func_001A61A0(s)) {
        e = &s->vt[0xB8 / 8];
        ((void (*)(void *))e->pfn)((char *)s + e->d);
    } else if (!s->x10) {
        func_00148300(D_003DD490, s);
    }
}
