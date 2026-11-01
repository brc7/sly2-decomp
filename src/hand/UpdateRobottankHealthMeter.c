struct S { char p0[0x55C]; int n55C, n560, f564; void *p568; };
void func_00212E40(void *p, int n, float g);
void UpdateRobottankHealthMeter(struct S *p) {
    if (p->p568) {
        float g = (float)p->n560 / (float)p->n55C;
        if (p->f564) g = 1.0f - g;
        func_00212E40(p->p568, 0x34F, g);
    }
}
