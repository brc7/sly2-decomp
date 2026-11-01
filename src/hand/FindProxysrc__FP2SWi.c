struct E { int pad[4]; int id; char pad2[0x58]; };
struct S { char pad[0x3F04]; int c; struct E *a; };
int FindProxysrc__FP2SWi(struct S *p, int id)
{
    int i;
    for (i = 0; i < p->c; i++)
        if (p->a[i].id == id) return i;
    return -1;
}
