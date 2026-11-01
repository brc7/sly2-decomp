struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
struct E *PdteFindDetectorTarget(struct S *p, int id);
void SetDetectorTargetState(struct S *p, struct E *pe, int n);
void func_002332B8(void *dst, void *src, int cb);
void RemoveDetectorTarget(struct S *p, int id)
{
    struct E *pe = PdteFindDetectorTarget(p, id);
    if (pe) {
        SetDetectorTargetState(p, pe, 0);
        p->c--;
        func_002332B8(pe, pe + 1, (&p->a[p->c] - pe) * sizeof(struct E));
    }
}
