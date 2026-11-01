struct VTE { short d; short pad; void *(*pfn)(void *, int); };
struct MSG { int cb; int nKind; };
struct Q { void *p0; void *p4; void *p8; void *pc; char pad[0x90]; char xa0[0x10]; char xb0[0x10]; };
struct S { char pad[0xC]; struct VTE *vt; char p1[0x138]; unsigned long fl; };
int func_00117BD0(void *pbis);
int func_00117D50(void *pbis);
void func_00117EB0(void *pbis, void *pv);
void *func_001AE2A0(struct MSG *pmsg);
void func_00186930(struct Q *q);
void func_0012DD28(struct S *p, void *pv, int n, struct Q *q);
static __inline__ void *PvLookup(struct S *p, void *pbis)
{
    struct VTE *e = &p->vt[51];
    return e->pfn((char *)p + e->d, func_00117D50(pbis));
}
void LoadSoIkFromBrx(struct S *p, void *pbis)
{
    int c = func_00117BD0(pbis);
    int i;
    if (!c) return;
    p->fl |= 1UL << 36;
    for (i = 0; i < c; i++) {
        struct MSG msg;
        struct Q *q;
        msg.cb = 0xE0;
        msg.nKind = 0x10;
        q = (struct Q *)func_001AE2A0(&msg);
        q->p4 = PvLookup(p, pbis);
        q->p8 = PvLookup(p, pbis);
        q->pc = PvLookup(p, pbis);
        func_00117EB0(pbis, q->xb0);
        q->p0 = PvLookup(p, pbis);
        func_00117EB0(pbis, q->xa0);
        func_00186930(q);
        func_0012DD28(p, q->p4, -1, q);
        func_0012DD28(p, q->p8, -1, q);
    }
}
