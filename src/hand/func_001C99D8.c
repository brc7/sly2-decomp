struct DL { void *head; void *tail; int ibDle; };
struct N { int id; struct N *pnParent; int x8; int xC; int x10; struct N *pnNext; struct DL dl; int pad; };
struct AL { int cb; int cbAlign; };
extern char D_003DD490[];
void *func_001AE2A0(const struct AL *pal);
void func_0011A050(struct DL *pdl, int ib);
int OidFromIdProxy(int id);
struct DL *PdlFromSwPxrOid(void *p, int id);
void func_0011A070(struct DL *pl, void *pv);
struct N *func_001C99D8(struct N *p, int c, const int *aid)
{
    struct N *pn, *pnParent, *pnNew;
    struct AL al;
    int i;
again:
    for (pn = (struct N *)p->dl.head; pn; pn = pn->pnNext) {
        if (pn->id == *aid) {
            if (c == 1) return pn;
            p = pn;
            c--;
            aid++;
            goto again;
        }
    }
    al.cb = c * sizeof(struct N);
    al.cbAlign = 0x10;
    pnNew = (struct N *)func_001AE2A0(&al);
    pnParent = p;
    for (i = 0; i < c; i++) {
        pn = &pnNew[i];
        func_0011A050(&pn->dl, 0x14);
        pn->id = aid[i];
        pn->x8 = OidFromIdProxy(pn->id);
        func_0011A070(PdlFromSwPxrOid(D_003DD490, pn->x8), pn);
        pn->pnParent = pnParent;
        func_0011A070(&pnParent->dl, pn);
        pnParent = pn;
    }
    return pn;
}
