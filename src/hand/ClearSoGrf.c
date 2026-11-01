struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[49]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void ClearSoGrf(struct L *p, int f)
{
    struct L *q;
    int unused[4];
    int v = p->v40;
    if ((v ^ 1) & 1) *(int *)v &= ~f;
    else p->v40 = v & ~f;
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[48].pfn((char *)q + q->pvt->a[48].d, f);
}
