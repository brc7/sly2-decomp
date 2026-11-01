struct S { char pad[0x40]; int x40; char pad2[0x118]; int n15c; int a[2]; void *p168; };
void InitSo(struct S *p);
void InitPo(struct S *p)
{
    int v;
    InitSo(p);
    p->n15c = 1;
    p->p168 = p;
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->x40 = v | 2;
}
