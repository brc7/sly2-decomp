struct VTE { short d; short pad; void *pfn; };
struct O { int x0, x4, x8; struct VTE *vt; };
/** @sly2-annot addr=0x1A6450
 * Purpose: LO vt37: call vt37(arg) on each child from vt28
 * Info: proposed name ApplyLoProxyHierarchy (T2); class LO slot37; TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6450(struct O *o, void *arg)
{
    struct O **list;
    int n, i;
    struct VTE *e = &o->vt[0xE0 / 8];
    n = ((int (*)(void *, struct O ***))e->pfn)((char *)o + e->d, &list);
    for (i = 0; i < n; i++) {
        struct O *p = list[i];
        struct VTE *f = &p->vt[0x128 / 8];
        ((void (*)(void *, void *))f->pfn)((char *)p + f->d, arg);
    }
}
