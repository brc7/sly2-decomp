struct S { char pad[0x10]; void *p10; char pad2[0x28]; int n3C; int n40; };
void StartBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
int func_00209BC0(void *p);
void SetBhvanimState(struct S *p, int n);
void StartBhvanim(struct S *p) {
    StartBhv(p);
    AddStepguardSgf(p->p10, p->n3C);
    if (!p->n40) {
        p->n40 = func_00209BC0(p->p10);
        if (!p->n40) { SetBhvanimState(p, 1); return; }
    }
    SetBhvanimState(p, 0);
}
