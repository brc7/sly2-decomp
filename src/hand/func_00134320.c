struct O { char pad[0x48]; struct O *pnext; };
struct L { char pad[0xec]; struct O *fec; };
struct G { char pad[0x3d10]; struct O *pfirst; };
void func_001F4BF8(struct L *);
void func_00133A38(struct G *, struct O *, struct O *, int);
/** @sly2-annot addr=0x134320
 * Purpose: If the object has a broadphase node (+0xEC): refresh (0x1F4BF8) and UpdateSwPox against every
 *   other node
 * Info: proposed name RecalcSwOxfFilterForObject (T1); TU bbmark
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570,
 *   docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_00134320(struct G *pg, struct L *pl) {
    struct O *po;
    if (pl->fec) {
        func_001F4BF8(pl);
        for (po = pg->pfirst; po; po = po->pnext)
            if (po != pl->fec) func_00133A38(pg, pl->fec, po, 0);
    }
}
