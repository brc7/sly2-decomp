struct N { int f0; int pad[2]; int fc; struct N *pnext; int pad2[4]; int f24; };
struct S { char pad[0x90]; struct N *pn; };
void func_002301D0(int, struct S *);
/** @sly2-annot addr=0x131B10
 * Purpose: Finds a channel by id and resets it (-1) via 0x2301D0
 * Info: proposed name DetachAsegaChannel (T2); TU asega
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131B10(int id, int a, struct S *p) {
    struct N *pn = p->pn;
    for (;;) {
        if (!pn) return;
        if (pn->f0 == 0 && pn->f24 == id) { pn->fc = -1; func_002301D0(a, p); return; }
        pn = pn->pnext;
    }
}
