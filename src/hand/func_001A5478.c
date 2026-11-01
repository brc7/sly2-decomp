struct VTE { short d; short pad; void *pfn; };
struct S {
    int x0, x4, x8; struct VTE *vt; struct S *x10; int x14; char pad[0xC]; struct S *x24;
    union { unsigned long u; struct { unsigned long f0 : 52; long st : 4; unsigned long f56 : 4; unsigned long f60 : 1; unsigned long f61 : 3; } b; } x28;
};
extern char D_003DD490[];
void func_001A5448(struct S *p, void **a, struct S ***b);
void func_0011A2E8(void *a, struct S *s);
void func_0011A280(void *a, struct S *b, struct S *s);
void func_00147E88(void *g, int n);
int func_001A61A0(struct S *s);
void func_00148300(void *g, struct S *s);
/** @sly2-annot addr=0x1A5478
 * Purpose: LO vt8 Add: if marked removed (bit56) clear it, move into in-world part of parent child DL, call
 *   vt11 AddHierarchy if parent in world, root not-busyk2 -> g_sw busy list, add children (vt8), then vt23 if
 *   in world else 0x148300
 * Info: proposed name AddLo (T1); class LO slot8; TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/engine/object-model.md#4-activation-onadd--onremove-slots-23--22,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
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
        func_0011A2E8(a, s);
        func_0011A280(a, *b, s);
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
