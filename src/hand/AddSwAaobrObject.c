struct E { int k; void *p; int *pn; };
struct N { int a0[4]; int a10[4]; char pad[0x34 - 0x20]; int dl34[3]; char pad2[0x60 - 0x40]; void *f60; };
struct P { char pad[0x14]; void *f14; };
struct G { char pad[0x2c]; int dl2c[3]; char pad2[0x3cd0 - 0x38]; int ce; struct E *ape[3]; int pool3ce0;
    char pad3[0x3d10 - 0x3ce4]; int dl3d10[3]; int dl3d1c[3]; };
struct N *PvAllocSlotheapClear(void *);
void InitDl__FP2DLi(void *, int);
void AppendDlEntry__FP2DLPv(void *, struct N *);
int FFindDlEntry__FP2DLPv(void *, void *);
struct N *AddSwAaobrObject(struct G *pg, struct P *p) {
    int i;
    struct N *pn = PvAllocSlotheapClear(&pg->pool3ce0);
    InitDl__FP2DLi(pn->dl34, 0x60);
    pn->f60 = p;
    AppendDlEntry__FP2DLPv(pg->dl3d10, pn);
    if (FFindDlEntry__FP2DLPv(pg->dl2c, p->f14)) AppendDlEntry__FP2DLPv(pg->dl3d1c, pn);
    for (i = 0; i < 3; i++) {
        struct E *pe = &pg->ape[i][pg->ce];
        pe->k = 0;
        pe->p = pn;
        pe->pn = &pn->a0[i];
        pe++;
        pe->k = 1;
        pe->p = pn;
        pe->pn = &pn->a10[i];
    }
    pg->ce += 2;
    return pn;
}
