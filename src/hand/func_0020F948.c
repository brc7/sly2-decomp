typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { char pad[0x60]; struct O *po; char pad2[0xC]; long fl; };
struct B { char pad[0xEC]; struct S *ps; };
struct E { int pad; struct B *pb; int pad2[2]; long fl; };
int func_00134310(int a, int b);
#define VC61(o, a) ((int (*)(void *, struct O *))(o)->vt[61].pfn)((char *)(o) + (o)->vt[61].d, a)
/** @sly2-annot addr=0x20F948
 * Purpose: Intersection filter: reject self/flagged; accept unless relationship 0x134310(a,b) is 1 or 3
 * Info: proposed name FFilterSoPair (T1); TU sw
 * Notes: names_v2 v1 proposed PostBtnLoad__FP3BTN (0.007)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/player.md#throw-0x21f2e0-0x220d40,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020F948(struct S *p, struct E *e)
{
    struct S *ps = e->pb->ps;
    int a, b, r;
    if (!(e->fl & (1L << 48)) && (ps->fl & 4))
        return 0;
    if ((e->fl & (1L << 48)) || ps == p)
        return 0;
    a = VC61(p->po, ps->po);
    b = VC61(ps->po, p->po);
    r = func_00134310(a, b);
    return r != 1 && r != 3;
}
