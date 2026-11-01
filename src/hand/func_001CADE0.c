struct T { int x0; int x4; int c; int i0; };
struct S { char pad[0xE0]; struct T *pt; int xE4; void *xE8; };
int func_001CA978(struct S *p, void *pa);
void *func_001CA958(struct S *p, int i);
/** @sly2-annot addr=0x1CADE0
 * Purpose: pick the best-rated spare >= vMin (or the current one)
 * Info: proposed name PspareChoose (T2); TU proxy
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void *func_001CADE0(struct S *p, int vMin)
{
    int i, v, vBest;
    void *pv, *pvBest;
    if (p->xE4 == 0 && p->xE8) {
        if (func_001CA978(p, p->xE8) < 3) return 0;
        pvBest = p->xE8;
    } else {
        vBest = vMin;
        pvBest = 0;
        for (i = p->pt->i0; i < p->pt->c; i++) {
            pv = func_001CA958(p, i);
            v = func_001CA978(p, pv);
            if (v >= vBest) {
                pvBest = pv;
                vBest = v;
            }
        }
    }
    return pvBest;
}
