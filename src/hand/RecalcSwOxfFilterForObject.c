struct O { char pad[0x48]; struct O *pnext; };
struct L { char pad[0xec]; struct O *fec; };
struct G { char pad[0x3d10]; struct O *pfirst; };
void func_001F4BF8(struct L *);
void UpdateSwPox(struct G *, struct O *, struct O *, int);
void RecalcSwOxfFilterForObject(struct G *pg, struct L *pl) {
    struct O *po;
    if (pl->fec) {
        func_001F4BF8(pl);
        for (po = pg->pfirst; po; po = po->pnext)
            if (po != pl->fec) UpdateSwPox(pg, pl->fec, po, 0);
    }
}
