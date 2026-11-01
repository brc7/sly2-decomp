struct E { int a; int b; };
struct T { char pad[0x54]; struct E *ae; };
struct S { char pad[0x24]; struct T *pt; char pad2[0xC]; int x34; int x38; };
int GetSmaNext(struct S *p)
{
    int i = p->x38;
    if (i >= 0) return p->pt->ae[i].a;
    i = p->x34;
    if (i >= 0) return p->pt->ae[i].a;
    return -1;
}
