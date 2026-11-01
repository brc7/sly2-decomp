struct E { int id; char pad[0x28]; };
struct S { char pad[0x150]; int c; struct E *a; struct E *pcur; };
void SelectBootyEntry(struct S *p, int id) {
    int i;
    for (i = 0; i < p->c; i++) {
        if (id == p->a[i].id) { p->pcur = &p->a[i]; return; }
    }
    p->pcur = p->a;
}
