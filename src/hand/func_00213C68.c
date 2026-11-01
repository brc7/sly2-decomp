typedef int qword __attribute__((mode(TI)));
struct M { qword a, b; };
struct VE { short d; short pad; struct M *(*pf)(void *, int, int); };
struct A { struct VE *vt; };
struct S { char pad[0xC4]; struct A *pa; char pad2[0x30]; struct M *pm; };
/** @sly2-annot addr=0x213C68
 * Purpose: set widget 2D transform override (alloc 0x20 M23 at +0xF8)
 * Info: proposed name SetWidgetXf (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ad6a8 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00213C68(struct S *p, struct M *src)
{
    if (!p->pm) {
        struct A *a = p->pa;
        p->pm = a->vt[1].pf((char *)a + a->vt[1].d, 0x20, 0x1F);
    }
    *p->pm = *src;
}
