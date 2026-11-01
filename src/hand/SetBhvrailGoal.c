struct T { char pad[0xF84]; void *pF84; float gF88; int nF8C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; float g3C; };
float SMaxShape(void *p);
float SMinShape(void *p);
void SetBhvrailState(struct S *p, int n);
void SetBhvrailGoal(struct S *p, int f) {
    void *pv = p->pt->pF84; if (f) p->g3C = SMaxShape(pv); else p->g3C = SMinShape(pv);
    p->pt->nF8C = p->pt->gF88 < p->g3C;
    SetBhvrailState(p, 0);
}
