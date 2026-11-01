typedef struct QW { int a[4]; } QW;
struct O { char pad[0xFC]; int c; QW a[1]; };
struct S { char pad[0x3A4]; int n; char p2[0xF64 - 0x3A8]; struct O *o; };
QW *PdteFindDetectorTarget(struct O *o);
void func_001AE5A0(void *dst, void *src, int cb);
void func_00208860(struct S *p)
{
    QW *pq = PdteFindDetectorTarget(p->o);
    struct O *o = p->o;
    int c = o->c;
    QW *pqEnd = &o->a[c - p->n];
    if (pq && pq < pqEnd) {
        int i;
        o->c = c - 1;
        i = pq - o->a;
        func_001AE5A0(pq, pq + 1, (p->o->c - i) * 16);
    }
}
