struct VTE { short d; short pad; void *pfn; };
struct S { int x0, x4, x8; struct VTE *vt; void *x10; int x14; char pad[0x10]; unsigned long x28; };
extern char D_003DD4BC[], D_003DD490[];
int func_001A61A0(struct S *s);
void RemoveDlEntry__FP2DLPv(void *a, void *b);
void func_00147E20(void *a, int b);
void GetLoChildDl(void *p, void **a, struct S ***b);
void AppendDlEntry__FP2DLPv(void *a, struct S *s);
/** @sly2-annot addr=0x1A5630
 * Purpose: LO vt9 Remove: if in world vt22 + drop root from g_sw+0x2C; drop root from busy list; move to
 *   removed tail of child DL, set bit56, remove children (vt9), then vt12 RemoveHierarchy if it was in world
 * Info: proposed name RemoveLo (T1); class LO slot9; TU lo
 * Notes: names_v2 v1 proposed UpdateLightBeamGrfzon__FP5LIGHT (0.008)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/engine/object-model.md#4-activation-onadd--onremove-slots-23--22,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
void RemoveLo(struct S *s)
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
    GetLoChildDl(s->x10, &a, &b);
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
