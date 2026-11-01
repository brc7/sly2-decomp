struct VTE { short d; short pad; void *pfn; };
struct LO { int pad[3]; struct VTE *vt; };
struct X;
struct L { void *pv; struct L *pnext; struct L *pother; struct LO *po; };
struct LX { char pad[0xEC]; struct X *px; };
struct X { char pad[0x58]; struct X *pnext; int x5C; struct LO *plo; struct L *pl; void *pv68; int x6C; unsigned long grf; };
struct P { char pad[0x3D34]; struct X *pxFirst; };
struct C { int a[4]; };
void func_00133E30(struct P *p);
void func_00133E80(struct P *p);
struct C *func_001AE478(struct C *);
void func_001AE4A0(struct C *, int);
void func_001A7E10(struct P *, void *);
void *func_001AA1D8(struct P *, struct X *, struct X *);
void func_001F5078(struct LO *);
void RemoveDlEntry__FP2DLPv(void *, struct X *);
#define VCALL(po, n, T) ((T)(po)->vt[n].pfn)((char *)(po) + (po)->vt[n].d
void func_00134040(struct P *p)
{
    struct C ctx;
    struct X *apx[0x300];
    int c = 0;
    struct X *px;
    int i;
    func_00133E30(p);
    func_00133E80(p);
    func_001AE478(&ctx);
    for (px = p->pxFirst; px; px = px->pnext) {
        unsigned long grf = px->grf;
        struct L *pl;
        if (grf & 4)
            continue;
        grf |= 0x10;
        px->grf = grf;
        if (!(grf & 8)) {
            px->grf = grf | 8;
            apx[c++] = px;
        }
        for (pl = px->pl; pl; pl = pl->pnext) {
            struct X *px2 = ((struct LX *)pl->po)->px;
            unsigned long grf2 = px2->grf;
            void *pv;
            if (grf2 & 0x10)
                continue;
            if (grf2 & 4)
                continue;
            if (!(grf2 & 8)) {
                px2->grf = grf2 | 8;
                apx[c++] = px2;
            }
            VCALL(px->plo, 54, void (*)(void *, struct LO *)), px2->plo);
            VCALL(px2->plo, 54, void (*)(void *, struct LO *)), px->plo);
            pv = pl->pv;
            func_001A7E10(p, pv);
            { void *pvNew = func_001AA1D8(p, px, px2); pl->pv = pvNew; pl->pother->pv = pvNew; }
            if (pv || pl->pv) {
                if (!VCALL(px->plo, 62, int (*)(void *))))
                    func_001F5078(px->plo);
                if (!VCALL(px2->plo, 62, int (*)(void *))))
                    func_001F5078(px2->plo);
            }
        }
    }
    for (px = p->pxFirst; px; px = p->pxFirst) {
        px->grf &= ~0x10L;
        VCALL(px->plo, 54, void (*)(void *, struct LO *)), 0);
        func_001A7E10(p, px->pv68);
        px->pv68 = 0;
        VCALL(px->plo, 59, void (*)(void *)));
        px->grf &= ~2L;
        RemoveDlEntry__FP2DLPv(&p->pxFirst, px);
    }
    for (i = 0; i < c; i++) {
        px = apx[i];
        px->grf &= ~8L;
        VCALL(px->plo, 58, void (*)(void *)));
    }
    func_001AE4A0(&ctx, 2);
}
