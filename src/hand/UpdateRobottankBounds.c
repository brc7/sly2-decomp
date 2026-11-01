typedef int qword __attribute__((mode(TI)));
struct E { int a[8]; float g20; int b[11]; qword q50; };
struct S { char pad[0x190]; int c; int pad2[3]; struct E ae[8]; };
void UpdateSoTreeBounds(struct S *p);
qword ConvertAloPos(struct S *p, int n, qword q);
void ExtendSoTreeBounds(struct S *p, qword q, float g);
void UpdateRobottankBounds(struct S *p) {
    int i;
    UpdateSoTreeBounds(p);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        ExtendSoTreeBounds(p, ConvertAloPos(p, 0, pe->q50), pe->g20);
    }
}
