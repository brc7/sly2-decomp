typedef struct { int cb; int cbAlign; } AREQ;
typedef struct {
    int x0; int x4; void *x8; int pad0C; int x10; int pad14[3]; int x20; int pad24[3]; int x30;
    char p34[0xC8 - 0x34]; int xC8; int xCC; int xD0; int xD4;
} X;
typedef struct { char p0[0xE4]; X *px; } SO;
extern char D_003DD490[];
extern char D_002CE4B8[];
void PostLoLoad(SO *p);
void *func_002333B4(void *p, int c, int n);
int func_00233178(const void *a, const void *b, int n);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, SO *p);
void func_002590F0(SO *p, int f);
void *func_001AE238(AREQ *preq, void *pv);
void func_00259170(SO *p)
{
    void *a[5];
    AREQ req;
    AREQ *preq;
    X *x;
    int o1;
    int o2 = -1;

    PostLoLoad(p);
    {
        X *x0 = p->px;
        o1 = x0->x20;
        if (x0->x10 == 2)
            o2 = x0->x30;
    }
    func_002333B4(a, 0, 0x14);
    if (o1 != -1)
        a[0] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o1, p);
    if (o2 != -1) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o2, p);
        a[0] = 0;
        a[1] = q;
    }
    x = p->px;
    switch (x->x4) {
    case 1: {
        int c8 = x->xC8;
        int cc = x->xCC;
        int d0 = x->xD0;
        if (c8 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, c8, p);
        if (cc != -1)
            a[3] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, cc, p);
        if (d0 != -1)
            a[4] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, d0, p);
        break;
    }
    case 0:
        if (x->xD4 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, x->xD4, p);
        break;
    }
    if (p->px->x8 == 0)
        p->px->x8 = D_002CE4B8;
    if (func_00233178(p->px->x8, a, 0x14)) {
        func_002590F0(p, 1);
        preq = &req;
        req.cb = 0x14;
        preq->cbAlign = 0x10;
        p->px->x8 = func_001AE238(preq, a);
    }
}
