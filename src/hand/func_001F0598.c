struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[4]; };
struct Q { char pad[0x68]; char dl[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x24 - 0x10]; struct Q *pq; char pad3[0x30 - 0x28]; void *p30; };
struct DL;
extern struct DL D_003E126C;
int func_001F0638(struct P *);
void func_00130B18(void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
void func_001F0598(struct P *p)
{
    if (!func_001F0638(p)) {
        void *pv = p->p30;
        if (pv) {
            p->p30 = 0;
            func_00130B18(pv);
        }
        RemoveDlEntry__FP2DLPv(p->pq->dl, p);
        RemoveDlEntry__FP2DLPv(&D_003E126C, p);
        ((void (*)(void *, int, int, int, int, int))p->pvt->a[3].pfn)((char *)p + p->pvt->a[3].d, 3, 0, 0, 0, 0);
        ((void (*)(void *))p->pvt->a[2].pfn)((char *)p + p->pvt->a[2].d);
    }
}
