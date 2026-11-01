typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct O { char pad[0x60]; struct O *next; };
struct G { char pad[0x20]; qword pos; float r; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
struct S { char pad[0x3D1C]; struct G *first; };
void *func_001AE318(struct MSG *pmsg);
void FlushSwBoundsUpdates(struct S *p);
void UpdateSwAaox(struct S *p);

void IntersectSwBoundingSphere(struct S *p, qword pos, float r, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    struct MSG msg;
    struct O **ap;
    struct G *g;
    int c;
    msg.cb = 0x400;
    msg.nKind = 0x10;
    ap = func_001AE318(&msg);
    c = 0;
    FlushSwBoundsUpdates(p);
    UpdateSwAaox(p);
    for (g = p->first; g; g = g->next) {
        float rr;
        qword d, e, tmp;
        float f;
        struct O *o;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(g->pos));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(e), "=&j"(tmp) : "0"(d));
        rr = r + g->r;
        rr *= rr;
        __asm__("mtc1 %1, %0" : "=f"(f) : "r"(e));
        if (rr < f)
            continue;
        for (o = g->first; o; o = o->next) {
            if (pfn && !pfn(ctx, o))
                continue;
            if (c < 0x100)
                ap[c++] = o;
        }
    }
    *pap = ap;
    *pc = c;
}
