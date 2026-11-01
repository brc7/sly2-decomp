typedef int qword __attribute__((mode(TI)));
struct A { char pad[0x124]; int k124; };
struct S { char pad[0xF80]; int kF80; struct A *pF84; int xF88; int nF8C; void *pF90; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct A *pa, int n);
qword func_001A62E8(void *p);
void FindShapeClosestPoint(struct A *pa, qword q, int a, int b, int c, int d, int *pn);
void func_00204F78(struct S *p, struct A *pa, int n);

void func_0020B2A8(struct S *p, struct A *pa, int n)
{
    p->pF84 = pa;
    p->nF8C = n;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pa, 0xB)) {
        p->kF80 = 1;
        FindShapeClosestPoint(p->pF84, func_001A62E8(p->pF90), 0, 0, 0, 0, &p->xF88);
        func_00204F78(p, p->pF84, 1);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->pF84, 0xD)) {
        p->kF80 = 0;
        FindShapeClosestPoint(p->pF84, func_001A62E8(p), 0, 0, 0, 0, &p->xF88);
        func_00204F78(p, p->pF84, 1);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->pF84, 0x13) && p->pF84->k124 == 3) {
        p->kF80 = 2;
        func_00204F78(p, p->pF84, 1);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->pF84, 0x27)) {
        p->kF80 = 3;
        func_00204F78(p, p->pF84, 1);
    } else {
        p->pF84 = 0;
    }
}
