typedef int qword __attribute__((mode(TI)));
void FindShapeClosestPoint(void *a, int b, int c, qword *pq, int d, int e, int f);
qword func_001E77B0(void *a, int b)
{
    qword q;
    FindShapeClosestPoint(a, b, 0, &q, 0, 0, 0);
    return q;
}
