struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct Q { char pad[0x16C]; void *p16C; };
struct P { char pad[0x3F4]; struct X *px3F4; struct X *px3F8; int n3FC; float g400; char pad2[0x494 - 0x404]; struct Q *pq; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002C3738[];
void SetSmaGoalIfReachable(void *, int);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
void func_001F17F8(struct P *p, int k)
{
    void *pv;
    if (k == p->n3FC) return;
    p->n3FC = k;
    p->g400 = D_002DDED0.t;
    switch (k) {
    case -1: case 0:
        if (p->px3F4) VF(p->px3F4, 9);
        if (p->px3F8) VF(p->px3F8, 9);
        break;
    case 1: case 2:
        if (p->px3F4) VF(p->px3F4, 8);
        if (p->px3F8) VF(p->px3F8, 8);
        break;
    }
    pv = p->pq->p16C;
    if (pv) SetSmaGoalIfReachable(pv, D_002C3738[p->n3FC + 1]);
}
