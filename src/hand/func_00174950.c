struct S { char pad[0x10]; void *p10; char pad2[0x28]; int n3C; int n40; };
void StartBhv(struct S *p);
void func_00209890(void *p, int n);
int func_00209BC0(void *p);
void func_001749C0(struct S *p, int n);
void func_00174950(struct S *p) {
    StartBhv(p);
    func_00209890(p->p10, p->n3C);
    if (!p->n40) {
        p->n40 = func_00209BC0(p->p10);
        if (!p->n40) { func_001749C0(p, 1); return; }
    }
    func_001749C0(p, 0);
}
