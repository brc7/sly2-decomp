struct N { float t; int f4; int *pc; int fc; struct N *pnext; };
struct G { char pad[0x3d6c]; int c3d6c; };
struct S { char pad[0x39fc]; int pool39fc; };
extern struct G D_003DD490;
struct DL { struct N *head; struct N *tail; int ibDle; };
extern struct DL D_003E0E80;
struct N *PvAllocSlotheapClear(void *);
void InsertSlEntryAfter(struct DL *, struct N *, struct N *);
struct N *func_00133040(struct S *p, int a, int *pc, float t) {
    struct N *pn;
    struct N *pnPrev;
    struct N *pnCur;
    (*pc)++;
    pn = PvAllocSlotheapClear(&p->pool39fc);
    pn->f4 = a;
    if (!a) pn->fc = ++D_003DD490.c3d6c;
    else pn->fc = -1;
    pn->pc = pc;
    pn->t = t;
    pn->pnext = 0;
    pnPrev = 0;
    for (pnCur = D_003E0E80.head; pnCur; pnCur = pnCur->pnext) {
        if (pnCur->t > pn->t) break;
        pnPrev = pnCur;
    }
    InsertSlEntryAfter(&D_003E0E80, pnPrev, pn);
    return pn;
}
