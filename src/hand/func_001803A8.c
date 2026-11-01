struct T { char pad[0xEC]; int n; };
struct S { char pad[0x1C8]; struct T *pt; };
int FIgnoreSoIntersection(struct S *p, struct T *q);
int func_001803A8(struct S *p, struct T *q)
{
    if (p->pt && q->n == p->pt->n)
        return 3;
    return FIgnoreSoIntersection(p, q);
}
