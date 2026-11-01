struct VTE { short d; short pad; void (*pf)(void *); };
struct O { int a[3]; struct VTE *vt; char pad[0x14]; struct O *next; };
struct S { char pad[0x64]; struct O *first; int a[2]; struct O *end; };
void func_0022FAA0(struct S *p);
/** @sly2-annot addr=0x22FBE0
 * Purpose: 0x22FAA0 then children
 * Info: proposed name UpdateSoXfWorldHierarchy (T1); class SO slot18; TU xf
 * Notes: names_v2 C proposed SetAloCastShadow__FP3ALOi (0.200)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022FBE0(struct S *p)
{
    struct O *o;
    func_0022FAA0(p);
    for (o = p->first; o != p->end; o = o->next)
        o->vt[18].pf((char *)o + o->vt[18].d);
}
