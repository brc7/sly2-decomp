struct S { char p0[0x40]; int x40; char p1[0x148-0x44]; unsigned long fl; char p2[0x1C0-0x150]; float g1C0; };
void InitSensor(struct S *p);
void func_001DE400(struct S *p) {
    int v;
    InitSensor(p);
    p->g1C0 = 1.0f;
    p->fl &= ~0x800000000UL;
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 8;
    else p->x40 = v | 8;
}
