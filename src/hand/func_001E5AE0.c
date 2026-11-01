struct VTE { short d; short pad; void (*pfn)(void *); };
struct E { int k; char pad[0x4C]; };
struct A { char pad[0x50]; int c; struct E *ae; char pad2[4]; char dl[1]; };
struct C { char pad[0x18]; int x18; };
struct S { int x0; int x4; int pad8; struct VTE *pvt; int pad10; struct A *pa; int x18; int x1C; int pad20[2]; struct E *pe28; struct E *pe2C; char pad30[0xEC - 0x30]; int xEC; };
extern char D_003E1560[];
extern char D_003DD490[];
extern struct VTE D_002D8C08[];
struct S *PvAllocSlotheapClear(void *);
void *func_002333B4(void *, int, int);
int func_0015E938(void *, int, int, int);
void PrependDlEntry__FP2DLPv(void *, void *);
struct S *func_001E5AE0(struct A *pa, struct E *pe, struct C *pc)
{
    struct S *p = PvAllocSlotheapClear(D_003E1560);
    struct E *peLast, *pe2;
    int depth;
    func_002333B4(p, 0, 0xF0);
    p->x0 = 0;
    p->x4 = 0;
    p->pvt = D_002D8C08;
    p->xEC = 0;
    p->pa = pa;
    if (pc)
        p->x18 = pc->x18;
    else
        p->x18 = (int)pa;
    if (((int *)pe)[4] != -1)
        p->x1C = func_0015E938(D_003DD490, 5, ((int *)pe)[4], 0);
    p->pe28 = pe;
    peLast = &pa->ae[pa->c - 1];
    depth = 1;
    for (pe2 = pe + 1; pe2 <= peLast; pe2++) {
        switch (pe2->k) {
        case 0:
            depth++;
            break;
        case 1:
            depth--;
            if (depth == 0) {
                p->pe2C = pe2;
                goto done;
            }
            break;
        }
    }
    p->pe2C = p->pe28;
done:
    PrependDlEntry__FP2DLPv(pa->dl, p);
    p->pvt[1].pfn((char *)p + p->pvt[1].d);
    return p;
}
