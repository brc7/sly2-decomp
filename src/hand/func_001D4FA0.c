extern struct CLOCK { float t; } D_002DDED0;
extern int D_002C2248[];
struct S { char p0[0x150]; int n150; float g154; char p1[0x164-0x158]; void *p164; };
void SetSmaGoalIfReachable(void *p, int n);
void func_001D4FA0(struct S *p, int n) {
    if (n == p->n150) return;
    p->n150 = n;
    p->g154 = D_002DDED0.t;
    if (p->p164 && n != -1) SetSmaGoalIfReachable(p->p164, D_002C2248[n]);
}
