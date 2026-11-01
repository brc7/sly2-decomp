typedef int qword __attribute__((mode(TI)));
struct Q { int pad0; int n4; int pad8; void *pC; int pad10; int n14; int pad18; void *p1C; };
struct S { char pad[0x38]; struct Q *pq; char pad2[0x10]; void *p4C; };
qword func_001A62E8(void *p);
int FCheckVolPoint(void *p, qword q);
int func_00155A68(struct S *p)
{
    if (FCheckVolPoint(p->pq->pC, func_001A62E8(p->p4C)))
        return p->pq->n4;
    if (FCheckVolPoint(p->pq->p1C, func_001A62E8(p->p4C)))
        return p->pq->n14;
    return p->pq->n4;
}
