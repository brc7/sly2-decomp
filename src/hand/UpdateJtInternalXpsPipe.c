typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *, int, int, qword, qword, qword); };
struct O { int a, b, c; struct VTE *vt; };
struct S { struct O *o; int pad[2]; int c; float g; int pad2[7]; qword q30; };
void EvaluateShapeS(int, qword *, qword *, float);
qword ConvertAloPos(struct O *, int, qword);
typedef union { qword q; float f[4]; } V;
void UpdateJtInternalXpsPipe(struct S *p) {
    V a, b, u;
    EvaluateShapeS(p->c, &a.q, &b.q, p->g);
    u.q = ConvertAloPos(p->o, 0, p->q30);
    { struct O *o = p->o; struct VTE *e = &o->vt[82]; e->pfn((char *)o + e->d, 7, p->c, a.q, b.q, u.q); }
}
