struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; char pad[0x18]; unsigned long fl; };
struct S { char pad[0x1AD8]; struct O *po; void *pv; };
void func_0012F9B8(void *a, struct O *po, int c, int d, float f, float g);
/** @sly2-annot addr=0x1B9DB8
 * Purpose: Same without the power-up check (event 0x40000 from Murray)
 * Info: proposed name SpawnMurrayHitEffect2 (T2); TU murray
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B9DB8(struct S *p, int x)
{
    if (!((int)(p->po->fl >> 60) & 1)) {
        struct VTE *e = &p->po->vt[29];
        ((void (*)(void *, int))e->pfn)((char *)p->po + e->d, x);
        func_0012F9B8(p->pv, p->po, 0, 0, 0.0f, 1.0f);
    }
}
