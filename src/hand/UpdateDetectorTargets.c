struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
int KCalculateDetectorTarget(struct S *p, int id, int n);
void SetDetectorTargetState(struct S *p, struct E *pe, int n);
void UpdateDetectorTargets(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        SetDetectorTargetState(p, pe, KCalculateDetectorTarget(p, pe->id, pe->n));
    }
}
