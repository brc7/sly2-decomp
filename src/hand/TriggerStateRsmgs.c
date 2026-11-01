struct E { int a[6]; };
void TriggerStateRsmg(void *p, struct E *pe, void *q, int n);
void TriggerStateRsmgs(void *p, void *q, int c, struct E *ae, int n)
{
    int i;
    for (i = 0; i < c; i++)
        TriggerStateRsmg(p, &ae[i], q, n);
}
