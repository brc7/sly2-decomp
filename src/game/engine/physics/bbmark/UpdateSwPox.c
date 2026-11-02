struct VTE { short d; short pad; void *pfn; };
struct LO { int pad[3]; struct VTE *vt; };
struct L { void *pv; struct L *pnext; struct L *pother; struct LO *po; int f; };
struct X { char pad[0x60]; struct LO *plo; struct L *pl; };
struct P { char pad[0x3CF8]; char aCF8[4]; };
int func_00133D08(struct P *, struct X *, struct X *);
struct L *PoxAddSw(struct P *, struct X *, struct X *);
struct L *PoxRemoveSw(struct P *, struct X *, struct X *);
void FreeSwXpList__FP2SWP2XP(struct P *, void *);
void func_001F5078(struct LO *);
void func_0011CD98(void *, struct L *);
#define VCALL(po, n, T) ((T)(po)->vt[n].pfn)((char *)(po) + (po)->vt[n].d
/** @sly2-annot addr=0x133A38
 * Purpose: Create or destroy the OX pair between two OXAs: k=1 force on, 2 force off, 0 keep current; pair if
 *   forced or overlapping (0x133D08); on removal call vt54 on both and free XP list
 * Info: proposed name UpdateSwPox (T1); TU bbmark
 * Notes: names_v2 C proposed UpdateTank__FP4TANKf (0.438)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0
 * @end-sly2-annot */
void UpdateSwPox(struct P *p, struct X *px1, struct X *px2, int k)
{
    struct L *pl;
    struct L *plT;
    int f;
    for (plT = px1->pl; plT; plT = plT->pnext) {
        if (plT->po == px2->plo)
            break;
    }
    switch (k) {
    case 1:
        f = 1;
        break;
    case 2:
        f = 0;
        break;
    case 0:
        f = 0;
        if (plT)
            f = plT->f;
        break;
    }
    pl = plT;
    if (f || func_00133D08(p, px1, px2)) {
        if (!pl) {
            struct L *pl2;
            plT = PoxAddSw(p, px1, px2);
            pl2 = PoxAddSw(p, px2, px1);
            plT->pother = pl2;
            pl2->pother = plT;
            pl = plT;
        }
    } else if (pl) {
        struct L *pl1, *pl2;
        VCALL(px1->plo, 54, void (*)(void *, struct LO *)), px2->plo);
        VCALL(px2->plo, 54, void (*)(void *, struct LO *)), px1->plo);
        pl1 = PoxRemoveSw(p, px1, px2);
        pl2 = PoxRemoveSw(p, px2, px1);
        if (pl1->pv) {
            FreeSwXpList__FP2SWP2XP(p, pl1->pv);
            if (!VCALL(pl1->po, 62, int (*)(void *))))
                func_001F5078(pl1->po);
            if (!VCALL(pl2->po, 62, int (*)(void *))))
                func_001F5078(pl2->po);
        }
        func_0011CD98(p->aCF8, pl1);
        func_0011CD98(p->aCF8, pl2);
        pl = 0;
    }
    if (pl) {
        pl->pother->f = f;
        pl->f = f;
    }
}
