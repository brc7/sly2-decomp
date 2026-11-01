struct P { char pad[0x13D0]; int n13D0; char pad1[0x1430 - 0x13D4]; void *p1430; char pad2[0x1910 - 0x1434]; int a1910[1]; };
int FIgnoreIntersection__10STEPPLAYERP2SO(struct P *p, void *pv);
int FFindLoParent(void *pv, void *p1);
int func_00134310(int n, int k);
int func_00220498(int *pn, void *pv, int n);
int MURRAY_FIgnoreIntersection(struct P *p, void *pv)
{
    int n = FIgnoreIntersection__10STEPPLAYERP2SO(p, pv);
    if (p->p1430 && FFindLoParent(pv, p->p1430))
        n = func_00134310(n, 3);
    else if (p->n13D0 == 15)
        n = func_00220498(p->a1910, pv, n);
    return n;
}
