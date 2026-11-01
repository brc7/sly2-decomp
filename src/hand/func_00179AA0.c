typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x2DC]; int n2DC; char pad2[8]; int n2E8; int n2EC; char pad3[0xFE8 - 0x2F0]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; int n40; };
void StartBhv(struct S *p);
void func_00179D30(struct S *p, int n);
qword func_001A62E8(void *p);
void func_00170E90(int n, qword q, void *pv);
int func_00209098(struct T *t);
void func_00179AA0(struct S *p) {
    struct T *t;
    StartBhv(p);
    t = p->pt;
    if (t->n2E8) p->n40 = t->n2E8;
    else if (t->nFE8 & 2) p->n40 = t->n2EC;
    else { p->n40 = 0; func_00179D30(p, 2); return; }
    if (p->pt->n2DC) func_00170E90(p->n40, func_001A62E8(p->pt), &p->pt->n2DC);
    if (func_00209098(p->pt)) func_00179D30(p, 3); else func_00179D30(p, 0);
}
