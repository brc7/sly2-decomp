struct T { char pad[0xF84]; void *pF84; float gF88; int nF8C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; float g38; };
float SMaxShape(void *p);
float SMinShape(void *p);
void SetBhvpipeState(struct S *p, int n);
void SetBhvpipeGoal(struct S *p, int f) {
    void *pv = p->pt->pF84; if (f) p->g38 = SMaxShape(pv); else p->g38 = SMinShape(pv);
    p->pt->nF8C = p->pt->gF88 < p->g38;
    SetBhvpipeState(p, 0);
}
