struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int v40; char pad3[0x130 - 0x44]; float g; };
void SetSoFadeRate(struct P *p, float g)
{
    if (g != 0.0f) {
        int v;
        p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
        v = p->v40;
        if ((v ^ 1) & 1) *(int *)v |= 2;
        else p->v40 = v | 2;
    }
    p->g = g;
}
