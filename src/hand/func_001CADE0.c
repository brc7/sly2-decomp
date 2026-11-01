struct T { int x0; int x4; int c; int i0; };
struct S { char pad[0xE0]; struct T *pt; int xE4; void *xE8; };
int func_001CA978(struct S *p, void *pa);
void *PspareFromIcopy(struct S *p, int i);
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
            pv = PspareFromIcopy(p, i);
            v = func_001CA978(p, pv);
            if (v >= vBest) {
                pvBest = pv;
                vBest = v;
            }
        }
    }
    return pvBest;
}
