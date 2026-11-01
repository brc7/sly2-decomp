typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD020;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0xA8 - 0x10]; int nA8; int nAC; char pad3[0xC]; int nBC; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; int pad4; char a40[0x10]; int n50; int n54; int n58; };
void StartBhv(struct S *p);
qword func_001A62E8(void *p);
void FindShapeClosestPointFromS(int a, float g, qword q, struct G *pg, void *pv, int b, int c, float *pf);
void FindShapeClosestPoint(int a, qword q, struct G *pg, void *pv, int b, int c, float *pf);
struct O *func_00205438(void *p, int n);
void func_0017ECF0(struct S *p, int n);
void func_0017EB88(struct S *p) {
    struct O *o;
    StartBhv(p);
    if (p->g38 >= 0.0f)
        FindShapeClosestPointFromS(p->n30, p->g38, func_001A62E8(p->p10), &D_003DD020, p->a40, 0, 0, &p->g38);
    else
        FindShapeClosestPoint(p->n30, func_001A62E8(p->p10), &D_003DD020, p->a40, 0, 0, &p->g38);
    o = func_00205438(p->p10, 0xBA);
    o->nA8 = p->n50;
    o->nAC = p->n54;
    o->nBC = p->n58;
    func_0017ECF0(p, 1);
    o->vt[8].pfn((char *)o + o->vt[8].d);
}
