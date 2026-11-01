struct S { char pad[0x150]; int k; };
void *FIgnoreSoIntersection(struct S *p, void *q);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *q, int a);
void *func_00134310(void *v, int a);
void *func_002242E0(struct S *p, void *q)
{
    void *v = FIgnoreSoIntersection(p, q);
    int k = p->k;
    if (k == 0 || k == 4) { if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34)) v = func_00134310(v, 1); }
    return v;
}
