struct N { int f0; struct N *pnext; int f8; int key; };
struct L { char pad[0x64]; struct N *phead; };
struct K { char pad[0x60]; int key; };
/** @sly2-annot addr=0x133CB8
 * Purpose: Unlink and return the OX whose other matches
 * Info: proposed name PoxRemoveSw (T1); TU bbmark
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0
 * @end-sly2-annot */
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
