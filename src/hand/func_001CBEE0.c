struct R { float lo, hi; };
struct T { char pad[0x50]; int x50; float x54; struct R r; };
struct U { char pad[0x18]; int x18; };
struct S { char pad[0x14]; struct T *pt; char p1[0x40 - 0x18]; struct U *pu; };
int FStepguardSenseStimulus(void *p, struct S *ps);
int func_001CBEE0(void *p, struct S *ps)
{
    struct T *pt = ps->pt;
    struct R r = pt->r;
    float f = pt->x54;
    int ret;
    if (pt->x50 == 0 && ps->pu && ps->pu->x18 == 0x246) {
        float d, c;
        d = (r.hi - r.lo) * 0.5f;
        c = r.lo + d;
        pt->x54 = f + f;
        d = d + d;
        pt->r.hi = c + d;
        pt->r.lo = c - d;
    }
    ret = FStepguardSenseStimulus(p, ps);
    pt->r = r;
    pt->x54 = f;
    return ret;
}
