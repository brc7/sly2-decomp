struct P { char pad[0x24]; void *p24; char pad2[0x30 - 0x28]; int n30; int n34; int n38; char pad3[0x48 - 0x3C]; int n48; };
int FSmaRetracted(struct P *);
int func_001F0400(void *, int);
int PsmtChooseTowardGoal(void *, int, int, int, int);
void SetSmaGoal(struct P *, int);
void SetSmaGoalIfReachable(struct P *p, int n)
{
    if (!FSmaRetracted(p) && n != -1) {
        int r = func_001F0400(p->p24, n);
        if (r != -1 && PsmtChooseTowardGoal(p->p24, p->n48, !p->n30 ? p->n34 : p->n38, r, 0))
            SetSmaGoal(p, n);
    }
}
