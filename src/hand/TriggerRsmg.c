typedef struct QW { int a[4]; } QW;
void func_0020FFF0(void *p, QW *pq, int a, int b, int c, int d);
void TriggerRsmg(void *p, int c, QW *pq, int a, int b)
{
    int i;
    for (i = 0; i < c; i++) {
        func_0020FFF0(p, &pq[i], a, b, 0, 0);

    }
}
