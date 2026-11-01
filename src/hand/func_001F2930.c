typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct E { char pad[0x10]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P;
struct Q { char pad[0x34]; char dl[0x2C]; struct P *p60; };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0xE0 - 0x10]; int nE0; struct E *aE4; int nE8; struct Q *pq; };
struct Q2 { char pad[0xEC]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
int FFindDlEntry__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
int FIsDlEmpty__FP2DL(void *);
void func_001A00D0(struct G *, int, struct E *);
void func_001F4E28(struct P *);
void func_001337D0(struct G *, struct Q *);
void func_001F5078(struct P *);
void func_001F2930(struct P *p, struct E *pe)
{
    int i;
    if (!p->pq) return;
    if (!FFindDlEntry__FP2DLPv(p->pq->dl, pe)) return;
    RemoveDlEntry__FP2DLPv(p->pq->dl, pe);
    if (pe->fl & (1L << 51)) func_001A00D0(&D_003DD490, 0, pe);
    func_001F4E28(p);
    if (FIsDlEmpty__FP2DL(p->pq->dl)) {
        struct Q *pq = p->pq;
        ((struct Q2 *)pq->p60)->pq = 0;
        p->pq = 0;
        func_001337D0(&D_003DD490, pq);
    } else if (p->pq->p60 != p) {
        i = p->nE0;
        while (--i >= 0) {
            if (FFindDlEntry__FP2DLPv(p->pq->dl, (struct E *)(i * 0xB0 + (int)p->aE4))) break;
        }
        if (i < 0) p->pq = 0;
    }
done:
    if (!p->pvt[62].pfn((char *)p + p->pvt[62].d)) func_001F5078(p);
}
