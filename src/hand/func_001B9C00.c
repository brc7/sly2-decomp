struct S { char pad[0x1034]; void *p1034; char pad1[0x1530 - 0x1038]; void *p1530; int n1534; };
void SetMurrayState(struct S *p, int n);
void SeekSma(void *p, int n);
void SetSmaGoal(void *p, int n);
void func_001B9C00(struct S *p, void *pv, int n, int m, int f)
{
    p->p1530 = pv;
    p->n1534 = m;
    SetMurrayState(p, 6);
    if (n == -1)
        return;
    if (p->p1530) {
        if (f)
            SeekSma(p->p1530, n);
        else
            SetSmaGoal(p->p1530, n);
    }
    if (f)
        SeekSma(p->p1034, n);
    else
        SetSmaGoal(p->p1034, n);
}
