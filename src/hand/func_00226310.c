struct T { char pad[0x3C]; int k; };
struct S { char pad[0x1C0]; struct T *t; };
void *FIgnoreSoIntersection(struct S *p);
void *func_00134310(void *v, int a);
void *func_00226310(struct S *p, struct T *q)
{
    void *v = FIgnoreSoIntersection(p);
    if (p->t && q->k == p->t->k) v = func_00134310(v, 1);
    return v;
}
