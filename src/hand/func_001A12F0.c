struct G { char pad[0x1030]; int x1030; };
struct S { struct G *pg; int n; };
int FAsegaRetracted(int);
void RunJtHurtCallback(struct S *);
void func_001A12F0(struct S *p)
{
    int n;
    do {
        n = p->n;
        if (FAsegaRetracted(p->pg->x1030))
            RunJtHurtCallback(p);
    } while (p->n != -1 && p->n != n);
}
