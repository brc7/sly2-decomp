struct VT { short d; short pad; void *pfn; };
struct P;
struct E { int x0; int x4; struct P *p8; char pad[0x34]; };
struct A { struct E ae[2]; char pad[0xAC - 0x80]; struct A *pnext; };
struct L { struct A *pa; struct L *pnext; };
struct O { char pad[0x64]; struct L *pl; };
struct P { char pad[0xC]; struct VT *pvt; char pad1[0xEC - 0x10]; struct O *po; char pad2[0x17C - 0xF0]; struct P *p17C; };
static __inline__ int IEnd(struct A *pa, struct P *p) { return pa->ae[0].p8 == p ? 0 : 1; }
void SendBarrierImpactMessages(struct P *p)
{
    struct P *ap[16];
    int c = 0;
    struct L *pl;
    int i;
    for (pl = p->po->pl; pl; pl = pl->pnext) {
        struct A *pa;
        for (pa = pl->pa; pa; pa = pa->pnext) {
            struct P *pT = pa->ae[1 - IEnd(pa, p)].p8;
            for (i = 0; i < c; i++)
                if (pT == ap[i])
                    break;
            if (i >= c) {
                ap[c++] = pT;
                if (c >= sizeof(ap) / sizeof(ap[0]))
                    goto done;
            }
        }
    }
done:
    for (i = 0; i < c; i++) {
        p->p17C = ap[i];
        ((void (*)(void *, int, int, int, int, int))p->pvt[3].pfn)((char *)p + p->pvt[3].d, 5, 0, 0, 0, 0);
    }
}
