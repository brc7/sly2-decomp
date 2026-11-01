typedef int qword __attribute__((mode(TI)));
struct S { void *p0; void *p4; int pad[2]; qword q10; };
extern char D_003DD020[];
void FindShapeClosestPoint(void *p, qword a, void *b, qword *pq, int c, int d, int e);
qword func_001A62E8(void *p);
qword func_001C1EC0(struct S *p, qword a)
{
    qword q;
    if (p->p0) {
        FindShapeClosestPoint(p->p0, a, D_003DD020, &q, 0, 0, 0);
    } else if (p->p4) {
        q = func_001A62E8(p->p4);
    } else {
        q = p->q10;
    }
    return q;
}
