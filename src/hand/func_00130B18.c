struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct E { float f; int pad[3]; };
struct Q { char pad[0x50]; char a50[0xA8 - 0x50]; int cA8; struct E *aAC; };
struct L1 { int x0; void *x4; void *x8; char pad[0x28 - 0xC]; struct L1 *pnext; };
struct L2 { int x0; int x4; struct O *po; int xC; struct L2 *pnext; };
struct P { int pad[3]; struct VTE *vt; int x10; struct Q *pq; struct O *x18; float g1C; char pad2[0x34 - 0x20]; struct L1 *pl1; struct L2 *pl2;
    char pad3[0x54 - 0x3C]; int i54; int x58; char a5C[4]; };
struct G { struct P *p; };
extern char D_003E1218[];
int func_00131968(struct P *p);
void func_00131290(struct P *p);
void func_00131990(struct P *p, int);
void RemoveDlEntry__FP2DLPv(void *, struct P *);
void func_00230100(void *, void *, struct L1 *);
void func_0012E210(struct L1 *);
void func_0012CE70(struct L2 *);
void func_00258040(void *, int);
void func_00133508(struct P *, int, struct P *);
static __inline__ void End(struct G *pg) { struct P *po = pg->p; ((void (*)(void *))po->vt[2].pfn)((char *)po + po->vt[2].d); }
void func_00130B18(struct P *p)
{
    if (func_00131968(p))
        return;
    {
        struct G g;
        struct L1 *pl1, *pl1Next;
        struct L2 *pl2, *pl2Next;
        g.p = p;
        __asm__ __volatile__("" : : "r"(&g) : "memory");
        ((void (*)(void *))p->vt[1].pfn)((char *)p + p->vt[1].d);
        for (; p->i54 < p->pq->cA8; p->i54++) {
            { register struct E *pe __asm__("$5") = p->pq->aAC; pe += p->i54; pe->f = p->g1C; }
            func_00131290(p);
            if (func_00131968(p))
                { End(&g); return; }
        }
        func_00131990(p, 0);
        RemoveDlEntry__FP2DLPv(D_003E1218, p);
        RemoveDlEntry__FP2DLPv(p->pq->a50, p);
        for (pl1 = p->pl1; pl1; pl1 = pl1Next) {
            pl1Next = pl1->pnext;
            func_00230100(pl1->x4, pl1->x8, pl1);
            func_0012E210(pl1);
        }
        p->pl1 = 0;
        for (pl2 = p->pl2; pl2; pl2 = pl2Next) {
            struct O *po = pl2->po;
            pl2Next = pl2->pnext;
            ((void (*)(void *, int))po->vt[45].pfn)((char *)po + po->vt[45].d, pl2->xC);
            func_0012CE70(pl2);
        }
        p->pl2 = 0;
        func_00258040(p->a5C, 0);
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 3, 0, 0, 0, 0);
        if (p->x18)
            ((void (*)(void *, int, struct P *))p->x18->vt[5].pfn)((char *)p->x18 + p->x18->vt[5].d, 2, p);
        func_00133508(p, 2, p);
        ((void (*)(void *))p->vt[2].pfn)((char *)p + p->vt[2].d);
        End(&g);
    }
}
