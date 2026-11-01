typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[7]; struct M m; char pad3[0xA8 - 0x60]; int nA8; int nAC; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct Q { char pad[0x34]; char dl[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct Q *pq; };
void func_001F27F0(struct P *);
int FFindDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_001F4BF8(struct P *);
void func_001F38A8(struct E *);
void func_001F5078(struct P *);
void func_001F2870(struct P *p, struct E *pe)
{
    struct O *po;
    func_001F27F0(p);
    if (FFindDlEntry__FP2DLPv(p->pq->dl, pe)) return;
    AppendDlEntry__FP2DLPv(p->pq->dl, pe);
    po = pe->po;
    pe->nA8 = -1;
    pe->m = po->m;
    pe->nAC = po->nA4;
    func_001F4BF8(p);
    func_001F38A8(pe);
    if (!p->pvt->a[62].pfn((char *)p + p->pvt->a[62].d)) func_001F5078(p);
}
