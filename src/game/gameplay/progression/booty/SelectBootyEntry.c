struct E { int id; char pad[0x28]; };
struct S { char pad[0x150]; int c; struct E *a; struct E *pcur; };
/** @sly2-annot addr=0x13C490
 * Purpose: make the entry with this id current (else the first)
 * Info: proposed name SelectBootyEntry (T1); TU booty
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void SelectBootyEntry(struct S *p, int id) {
    int i;
    for (i = 0; i < p->c; i++) {
        if (id == p->a[i].id) { p->pcur = &p->a[i]; return; }
    }
    p->pcur = p->a;
}
