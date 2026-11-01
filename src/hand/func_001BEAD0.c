struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; char pad[0x15C - 0x10]; int f15c; int f160; };
struct S { char pad[0x104]; int c; struct O *a[1]; };
/** @sly2-annot addr=0x1BEAD0
 * Purpose: Append object, set flags, call its vt9
 * Info: proposed name AddCid45ObjectShown (T2); class CID45; TU tu_1BE398
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void func_001BEAD0(struct S *p, struct O *po)
{
    if (po) {
        struct VTE *e;
        p->a[p->c++] = po;
        po->f160 = 1;
        po->f15c = 1;
        e = &po->vt[9];
        ((void (*)(void *))e->pfn)((char *)po + e->d);
    }
}
