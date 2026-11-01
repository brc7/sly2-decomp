struct LO { char pad[0x28]; unsigned long x28; };
struct T { int c; struct LO **a; };
struct A { int x0; int x4; int x8; };
struct S { char pad[0xE0]; struct T *pt; };
int func_001CA930(struct S *p, struct A *pa);
int func_001A61A0(struct LO *plo);
/** @sly2-annot addr=0x1CA978
 * Purpose: Score a spare 0..5 (locked=0, free=4, objects in world/visible levels)
 * Info: proposed name NRatePspare (T2); TU proxy
 * Notes: names_v2 C proposed RetractAloDrive__FP3ALO (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int func_001CA978(struct S *p, struct A *pa)
{
    int r, i, j, v;
    if (pa->x8) return 0;
    if (!pa->x0) return 4;
    r = 5;
    i = func_001CA930(p, pa);
    for (j = 0; j < p->pt->c; j++) {
        struct LO *plo = p->pt->a[i * p->pt->c + j];
        if (!((int)(plo->x28 >> 60) & 1)) v = 3;
        else if (func_001A61A0(plo)) { r = 1; goto out; }
        else v = 2;
        if (v < r) r = v;
    }
out:
    return r;
}
