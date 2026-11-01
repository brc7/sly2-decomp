struct N { int f0; struct N *pnext; int f8; int key; };
struct L { char pad[0x64]; struct N *phead; };
struct K { char pad[0x60]; int key; };
struct N *PoxRemoveSw(void *unused, struct L *pl, struct K *pk) {
    int key = pk->key;
    struct N *pn = pl->phead;
    if (pn) {
        struct N **ppn = &pn->pnext;
        if (pn->key == key) {
            pl->phead = pn->pnext;
        } else {
            for (;;) {
                pn = pn->pnext;
                if (!pn) break;
                if (pn->key == key) { *ppn = pn->pnext; break; }
                ppn = &pn->pnext;
            }
        }
    }
    return pn;
}
