struct R { int n0; void *p4; void *p8; int nC; };
struct S { char pad[0x194]; void *p194; int a198[1]; };
void ReleaseSma(void *p);
void func_00130658(void *pp);
void func_001305B8(void *paseg, struct S *p, float t, float svt, int grf, void *pp);
void ReplaceSma(void *a, struct S *p, int b, int c, void *d);
void func_0017F4C0(struct S *p, struct R *r) {
    ReleaseSma(p->a198);
    func_00130658(&p->p194);
    if (r) {
        func_001305B8(r->p4, p, 0.0f, 1.0f, r->nC, &p->p194);
        ReplaceSma(r->p8, p, -1, r->nC, p->a198);
    }
}
